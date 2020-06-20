// this is a silly solution
// just to show you how different
// components of this framework work
// please bring your wise to write
// a 'real' solution :)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "json/json.h"
#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"

using namespace Boost::Internal;
using namespace std;
class mydom{
	public:
	string name;
	int l, r;
};
class myvar{
	public:
	string name;
	vector<size_t>shape;
	int use;
}nowvar, LHSvar;
map<string, bool> usedom, LHSusedom;
vector<Expr>domlist, LHSdomlist;
map<string, myvar> myvarlist;
map<string, mydom> mydomlist;
vector<Stmt>mycode;
Type data_type;
Type index_type = Type::int_scalar(32);
int tempcnt = 0;
Expr *temp;
void build_Clist(const string &str, vector<size_t> *&ret);
template <class stnTP>
stnTP strToNum(const string& str)  
{  
    istringstream iss(str);  
    stnTP num;  
    iss >> num;  
    return num;      
}  

int findfirst(const string &str, char ch){
	int n = str.length();
	for(int i = 0; i < n; i++)
		if(str[i] == ch)return i;
	return -1;
}
int findlast(const string &str, char ch){
	int n = str.length();
	for(int i = n - 1; i >= 0; i--)
		if(str[i] == ch)return i;
	return -1;
}
int fix_expr_size(const string &str, int l, int r){
	//cout << "fix_expr "<< str << " " << l << " " << r << endl;
	int n = str.length(), cnt = 0, i;
	bool quo = true;
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')')cnt++;
		if(cnt == 0){
			quo = false;
			if(str[i] == '+' || str[i] == '-')break;
		}
		if(str[i] == '(')cnt--;
	}
	if(quo)
		return fix_expr_size(str.substr(1, n - 2), l, r);
	if(i >= 0){
		int sum, ssum;
		sum = fix_expr_size(str.substr(i + 1), l, r);
		//cout << sum << endl;
		if(str[i] == '-')sum = -sum;
		ssum = fix_expr_size(str.substr(0, i), l - sum, r - sum);
		if(ssum != 0 && str[i] == '+')
			fix_expr_size(str.substr(i + 1), l - ssum, r - ssum);
		return ssum + sum;
	}
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')')cnt++;
		if(cnt == 0)
			if(str[i] == '*' || str[i] == '/' || str[i] == '%')break;
		if(str[i] == '(')cnt--;
	}
	if(i >= 0){
		int sum, ssum;
		sum = fix_expr_size(str.substr(i + 1), l, r);
		if(str[i] == '*')ssum = fix_expr_size(str.substr(0, i), l / max((int)1, sum), (r - 1) / max((int)1, sum) + 1);
		else if(str[i] == '/')ssum = fix_expr_size(str.substr(0, i - 1), l * sum, r * sum);
		else ssum = fix_expr_size(str.substr(0, i), -1000000000, 1000000000);
		if(str[i] == '*')return ssum * sum;
		else if(str[i] == '/') return ssum / max((int)1, sum);
		else return ssum % max((int)1, sum);
	}
	if(str[n - 1] >= '0' && str[n - 1] <= '9')return strToNum<int>(str);
	if(mydomlist.find(str) == mydomlist.end()){
		mydom a({str, l, r});
		mydomlist[str] = a;
	}
	mydomlist[str].l = max(mydomlist[str].l, l);
	mydomlist[str].r = min(mydomlist[str].r, r);
	return 0;
}
void fix_list_size(const string &str, vector<size_t> *&_size){
	string st = str;
	for(auto i : *_size){
		int t = findfirst(st, ',');
		fix_expr_size(st.substr(0, t), 0, (int)i);
		st = st.substr(t + 1);
	}
}
void fix_size(const string &str){
	string st = str;
	for(int t = findlast(st, ']'); t >= 0; t = findlast(st, ']')){
		st = st.substr(0, t);
		int s = findlast(st, '[');
		int r = findlast(st, '<');
		vector<size_t> *res_Clist;
		build_Clist(st.substr(r + 1, s - r - 2), res_Clist);
		fix_list_size(st.substr(s + 1, t - s - 1), res_Clist);
		st = st.substr(0, r);
	}
}
void build_IdExpr(const string &str, Expr *&ret){
	//cout << "build_IdExpr " << str << endl;
	int n = str.length(), cnt = 0, i;
	bool quo = true;
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')')cnt++;
		if(cnt == 0){
			quo = false;
			if(str[i] == '+' || str[i] == '-')break;
		}
		if(str[i] == '(')cnt--;
	}
	if(quo){
		Expr *res_IdExpr;
		build_IdExpr(str.substr(1, n - 2), res_IdExpr);
		ret = new Expr(Unary::make(data_type, UnaryOpType::Not, *res_IdExpr));
		return;
	}
	if(i >= 0){
		Expr *LIdExpr, *RIdExpr;
		build_IdExpr(str.substr(0, i), LIdExpr);
		build_IdExpr(str.substr(i + 1), RIdExpr);
		if(str[i] == '+')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Add, *LIdExpr, *RIdExpr));
		else
			ret = new Expr(Binary::make(data_type, BinaryOpType::Sub, *LIdExpr, *RIdExpr));
		return;
	}
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')')cnt++;
		if(cnt == 0)
			if(str[i] == '*' || str[i] == '/' || str[i] == '%')break;
		if(str[i] == '(')cnt--;
	}
	if(i >= 0){
		Expr *LIdExpr, *RIdExpr;
		build_IdExpr(str.substr(0, i - (str[i - 1] == '/')), LIdExpr);
		build_IdExpr(str.substr(i + 1), RIdExpr);
		if(str[i] == '*')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mul, *LIdExpr, *RIdExpr));
		else if(str[i] == '/')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Div, *LIdExpr, *RIdExpr));
		else
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mod, *LIdExpr, *RIdExpr));
		return;
	}
	if(str[n - 1] >= '0' && str[n - 1] <= '9'){
		ret = new Expr(strToNum<int>(str));
		return;
	}
    Expr thisdom = Dom::make(index_type, max(0, mydomlist[str].l), max(0, mydomlist[str].r));
    ret = new Expr(Index::make(index_type, str, thisdom, IndexType::Spatial));
	if(!usedom[str]){
		usedom[str] = 1;
		domlist.push_back(*ret);
	}
	return;
}
void build_Clist(const string &str, vector<size_t> *&ret){
	//cout << "build_Clist " << str << endl;
	int t = findlast(str, ',');
	if(t < 0)
		ret = new vector<size_t>({strToNum<size_t>(str)});
	else{
		build_Clist(str.substr(0, t), ret);
		ret->push_back(strToNum<size_t>(str.substr(t + 1)));
	}
}
void build_Alist(const string &str, vector<Expr> *&ret){
	//cout << "build_Alist " << str << endl;
	int t = findlast(str, ',');
	if(t < 0){
		Expr *res_IdExpr;
		build_IdExpr(str, res_IdExpr);
		ret = new vector<Expr>({*res_IdExpr});
	}else{
		build_Alist(str.substr(0, t), ret);
		Expr *res_IdExpr;
		build_IdExpr(str.substr(t + 1), res_IdExpr);
		ret->push_back(*res_IdExpr);
	}
}

void build_SRef(const string &str, Expr *&ret){
	//cout << "build_SRef " << str << endl;
	int r = findfirst(str, '<');
	string name = str.substr(0, r);
	nowvar.name = name;
	nowvar.shape.clear();
	if(myvarlist.find(name) == myvarlist.end())
		myvarlist[name] = nowvar;
	ret = new Expr(Var::make(data_type, name, {}, {}));
}
void build_TRef(const string &str, Expr *&ret){
	//cout << "build_TRef " << str << endl;
	int n = str.length();
	int s = findfirst(str, '[');
	int r = findfirst(str, '<');
	string name = str.substr(0, r);
	vector<size_t> *res_Clist;
	vector<Expr> *res_Alist;
	build_Clist(str.substr(r + 1, s - r - 2), res_Clist);
	build_Alist(str.substr(s + 1, n - s - 2), res_Alist);
	nowvar.name = name;
	nowvar.shape = *res_Clist;
	if(myvarlist.find(name) == myvarlist.end())
		myvarlist[name] = nowvar;
	ret = new Expr(Var::make(data_type, name, *res_Alist, *res_Clist));
}
void build_LHS(const string &str, Expr *&ret){
	//cout << "build_LHS " << str << endl;
	usedom.clear();
	domlist.clear();
	build_TRef(str, ret);
	LHSvar = nowvar;
	LHSdomlist.clear();
	for(auto i : domlist)
		LHSdomlist.push_back(i);
	LHSusedom.clear();
	for(auto i : usedom)
		LHSusedom[i.first] = 1;
}
void build_RHS(const string &str, Expr *&ret){
	//cout << "build_RHS " << str << endl;
	int n = str.length(), cnt = 0, i;
	bool quo = true;
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')' || str[i] == ']')cnt++;
		if(cnt == 0){
			quo = false;
			if(str[i] == '+' || str[i] == '-')break;
		}
		if(str[i] == '(' || str[i] == '[')cnt--;
	}
	if(quo){
		Expr *res_IdExpr;
		build_RHS(str.substr(1, n - 2), res_IdExpr);
		ret = new Expr(Unary::make(data_type, UnaryOpType::Not, *res_IdExpr));
		return;
	}
	if(i >= 0){
		Expr *res_LRHS, *res_RRHS;
		build_RHS(str.substr(0, i), res_LRHS);
		build_RHS(str.substr(i + 1), res_RRHS);
		if(str[i] == '+')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Add, *res_LRHS, *res_RRHS));
		else
			ret = new Expr(Binary::make(data_type, BinaryOpType::Sub, *res_LRHS, *res_RRHS));
		return;
	}
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')' || str[i] == ']')cnt++;
		if(cnt == 0)if(str[i] == '*' || str[i] == '/' || str[i] == '%')break;
		if(str[i] == '(' || str[i] == '[')cnt--;
	}
	if(i >= 0){
		Expr *res_LRHS, *res_RRHS;
		build_RHS(str.substr(0, i - (str[i - 1] == '/')), res_LRHS);
		build_RHS(str.substr(i + 1), res_RRHS);
		if(str[i] == '*')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mul, *res_LRHS, *res_RRHS));
		else if(str[i] == '/')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Div, *res_LRHS, *res_RRHS));
		else 
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mod, *res_LRHS, *res_RRHS));
		return;
	}
	if(str[n - 1] == ']'){
		build_TRef(str, ret);
		return;
	}
	if(str[n - 1] == '>'){
		build_SRef(str, ret);
		return;
	}
	if(data_type == Type::float_scalar(32))
		ret = new Expr(strToNum<float>(str));
	else ret = new Expr(strToNum<int>(str));
	return;
}
void build_RHS_list(const string &str, vector<Stmt> *&ret){
	//cout << "build_RHS_list " << str << endl;
	int n = str.length(), cnt = 0, i;
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')' || str[i] == ']')cnt++;
		if(cnt == 0)
			if(str[i] == '+' || str[i] == '-')break;
		if(str[i] == '(' || str[i] == '[')cnt--;
	}
	if(i >= 0){
		build_RHS_list(str.substr(0, i), ret);
		Expr *res_RHS;
		usedom.clear();
		for(auto i : LHSusedom)
			usedom[i.first] = 1;
		domlist.clear();
		build_RHS(str.substr(i + 1), res_RHS);
		Stmt tem = Move::make(*temp, Binary::make(data_type, (str[i] == '+' ? BinaryOpType::Add : BinaryOpType::Sub), *temp, *res_RHS), MoveType::MemToMem);
		if(domlist.size())ret->push_back(LoopNest::make(domlist, {tem}));
		else ret->push_back(tem);
		return;
	}
	Expr *res_RHS;
	usedom.clear();
	for(auto i : LHSusedom)
		usedom[i.first] = 1;
	domlist.clear();
	build_RHS(str, res_RHS);
	Stmt tem = Move::make(*temp, *res_RHS, MoveType::MemToMem);
	if(domlist.size())ret = new vector<Stmt>({LoopNest::make(domlist, {tem})});
	else ret = new vector<Stmt>({tem});
	return;
}
void build_S(const string &str){
	int n = str.length();
	//cout << "build_S " << str << endl;
	mydomlist.clear();
	fix_size(str);
	/*for(auto i : mydomlist)
		cout << i.second.name << " " << i.second.l << " " << i.second.r << endl;*/
	int t = findfirst(str, '=');
	Expr *res_LHS;
	build_LHS(str.substr(0, t), res_LHS);
	string _name = "temp" + to_string(tempcnt++);
	myvarlist[_name] = *(new myvar({_name, LHSvar.shape}));
    temp = new Expr(Var::make(data_type, _name, LHSdomlist, LHSvar.shape));
	vector<Stmt> *res_RHS;
	build_RHS_list(str.substr(t + 1, n - t - 2), res_RHS);
	mycode.push_back(Move::make(*temp, *temp, MoveType::LocalToLocal));
	mycode.push_back(LoopNest::make(LHSdomlist, *res_RHS));
	mycode.push_back(LoopNest::make(LHSdomlist, {Move::make(*res_LHS, *temp, MoveType::MemToMem)}));
}
void build_P(const string &str){
	//cout << "build_P " << str << endl;
	int n = str.length(), t = findfirst(str, ';');
	if(t == n - 1)
		build_S(str);
	else{
		build_S(str.substr(0, t + 1));
		build_P(str.substr(t + 1));
	}
}
bool Input(int cnt, string &name, vector<string>&ins, vector<string>&outs, string &data_type, string &kernel){
	string st;
	if(cnt == 0)st = "./cases/example.json";
	else st = "./cases/case" + to_string(cnt) + ".json";
    ifstream ifile(st, std::ios::in);
	if(!ifile.good())return false;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(ifile, root)) {
		name = root["name"].asString();
		data_type = root["data_type"].asString();
		kernel = root["kernel"].asString();
		Json::Value object = root["ins"];
		for (int i = 0; i < object.size(); i++)
			ins.push_back(object[i].asString());
		object = root["outs"];
		for (int i = 0; i < object.size(); i++)
			outs.push_back(object[i].asString());
	}
	else return false;

	ifile.close();
	return true;

}

int main() {
	for(int i = 0; i <= 10; i++){
		string name, type, kernel;
		vector<string> ins, outs;
		if(!Input(i, name, ins, outs, type, kernel)){
			cout<< "case" + to_string(i) + " missed!" << endl;
			continue;
		}
		//cout << name <<endl;
		//cout << kernel <<endl;
		if(type[0] == 'f')data_type = Type::float_scalar(32);
		else data_type = Type::int_scalar(32);
		int n = kernel.length();
		string st = "";
		for(int i = 0; i < n; i++)
			if(kernel[i] != ' ')st += kernel[i];
		usedom.clear();
		LHSusedom.clear();
		domlist.clear();
		LHSdomlist.clear();
		myvarlist.clear();
		mydomlist.clear();
		mycode.clear();
		tempcnt = 0;
		build_P(st);
		vector<Expr>input;
		vector<Expr>output;
		for(auto i : ins){
			myvarlist[i].use = 1;
			input.push_back(Var::make(data_type, i, {}, myvarlist[i].shape)); 
		}
		for(auto i : outs)
			if(!myvarlist[i].use)
				output.push_back(Var::make(data_type, i, {}, myvarlist[i].shape)); 
		Group _kernel = Kernel::make(name, input, output, mycode, KernelType::CPU);
		IRPrinter printer;
		string code = printer.print(_kernel);
		//cout << code;
		if(i > 0){
			ofstream ofile("./kernels/kernel_case" + to_string(i) + ".cc", std::ios::out);
			ofile << code;
			ofile.close();
		}else{
			ofstream ofile("./kernels/kernel_example.cc", std::ios::out);
			ofile << code;
			ofile.close();
		}
		cout<< "case" + to_string(i) + " finished!" <<endl;
	}
    return 0;
}
