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
vector<Expr>domlist, LHSdomlist, *gradvarlist;
map<string, myvar> myvarlist;
map<string, mydom> mydomlist;
vector<Stmt>mycode;
vector<string>grad_to;
Type data_type;

int cntgrad;
int nowgrad;

string gradname;
Type index_type = Type::int_scalar(32);
int tempcnt = 0;
Expr *now_grad_to;
Expr *res_LHS;
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
void deluse(const string &str){
	string st = str;
	for(int t = findlast(st, ']'); t >= 0; t = findlast(st, ']')){
		st = st.substr(0, t);
		int r = findlast(st, '<');
		int s = r - 1;
		while((st[s] >= 'A' && st[s] <='Z') || (st[s] >= 'a' && st[s] <= 'z'))s--;
		string name = st.substr(s + 1, r - s - 1);
		myvarlist[name].use--;
	}
}
void build_IdExpr(const string &str, Expr *&ret, Expr *&varret){
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
		Expr *res_IdExpr, *res_varExpr;
		build_IdExpr(str.substr(1, n - 2), res_IdExpr, res_varExpr);
		ret = new Expr(Unary::make(data_type, UnaryOpType::Not, *res_IdExpr));
		varret = new Expr(Unary::make(data_type, UnaryOpType::Not, *res_varExpr));
		return;
	}
	if(i >= 0){
		Expr *LIdExpr, *RIdExpr, *LvarExpr, *RvarExpr;
		build_IdExpr(str.substr(0, i), LIdExpr, LvarExpr);
		build_IdExpr(str.substr(i + 1), RIdExpr, RvarExpr);
		if(str[i] == '+')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Add, *LIdExpr, *RIdExpr)),
			varret = new Expr(Binary::make(data_type, BinaryOpType::Add, *LvarExpr, *RvarExpr));	
		else
			ret = new Expr(Binary::make(data_type, BinaryOpType::Sub, *LIdExpr, *RIdExpr)),
			varret = new Expr(Binary::make(data_type, BinaryOpType::Sub, *LvarExpr, *RvarExpr));
		return;
	}
	for(i = n - 1; i >= 0; i--){
		if(str[i] == ')')cnt++;
		if(cnt == 0)
			if(str[i] == '*' || str[i] == '/' || str[i] == '%')break;
		if(str[i] == '(')cnt--;
	}
	if(i >= 0){
		Expr *LIdExpr, *RIdExpr, *LvarExpr, *RvarExpr;
		build_IdExpr(str.substr(0, i - (str[i - 1] == '/')), LIdExpr, LvarExpr);
		build_IdExpr(str.substr(i + 1), RIdExpr, RvarExpr);
		if(str[i] == '*')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mul, *LIdExpr, *RIdExpr)),
			varret = new Expr(Binary::make(data_type, BinaryOpType::Mul, *LvarExpr, *RvarExpr));
		else if(str[i] == '/')
			ret = new Expr(Binary::make(data_type, BinaryOpType::Div, *LIdExpr, *RIdExpr)),
			varret = new Expr(Binary::make(data_type, BinaryOpType::Div, *LvarExpr, *RvarExpr));
		else
			ret = new Expr(Binary::make(data_type, BinaryOpType::Mod, *LIdExpr, *RIdExpr)),
			varret = new Expr(Binary::make(data_type, BinaryOpType::Mod, *LvarExpr, *RvarExpr));
		return;
	}
	if(str[n - 1] >= '0' && str[n - 1] <= '9'){
		ret = new Expr(strToNum<int>(str));
		varret = new Expr(strToNum<int>(str));
		return;
	}
    Expr thisdom = Dom::make(index_type, max(0, mydomlist[str].l), max(0, mydomlist[str].r));
    ret = new Expr(Index::make(index_type, str, thisdom, IndexType::Spatial));
	varret = new Expr(Var::make(index_type, str, {}, {}));
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
void build_Alist(const string &str, vector<Expr> *&ret, vector<Expr> *&varret){
	//cout << "build_Alist " << str << endl;
	int t = findlast(str, ',');
	if(t < 0){
		Expr *res_IdExpr, *res_varExpr;
		build_IdExpr(str, res_IdExpr, res_varExpr);
		ret = new vector<Expr>({*res_IdExpr});
		varret = new vector<Expr>({*res_varExpr});
	}else{
		build_Alist(str.substr(0, t), ret, varret);
		Expr *res_IdExpr, *res_varExpr;
		build_IdExpr(str.substr(t + 1), res_IdExpr, res_varExpr);
		ret->push_back(*res_IdExpr);
		varret->push_back(*res_varExpr);
	}
	//cout << "build_Alist_finish " << str << endl;
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
void build_TRef(const string &str, Expr *&ret, bool d = false){
	//
	int n = str.length();
	int s = findfirst(str, '[');
	int r = findfirst(str, '<');
	string name = str.substr(0, r);
	bool e = false;
	if(!d){
		for(int i = 0; i < grad_to.size(); i++)
		if(name == grad_to[i]){
			cntgrad++;
			if(cntgrad == nowgrad)e = true;
		}
	}
	if(d || e)name = "d" + name;
	//cout << "build_TRef " << name << endl;
	vector<size_t> *res_Clist;
	vector<Expr> *res_Alist;
	vector<Expr> *res_varlist;
	build_Clist(str.substr(r + 1, s - r - 2), res_Clist);
	build_Alist(str.substr(s + 1, n - s - 2), res_Alist, res_varlist);
	nowvar.name = name;
	nowvar.shape = *res_Clist;
	if(myvarlist.find(name) == myvarlist.end())
		myvarlist[name] = nowvar;
	myvarlist[name].use++;
	ret = new Expr(Var::make(data_type, name, *res_Alist, *res_Clist));
	if(e){
		gradname = name;
		gradvarlist = res_varlist;
		ret = new Expr(1);
	}
}
void build_LHS(const string &str, Expr *&ret){
	//cout << "build_LHS " << str << endl;
	usedom.clear();
	domlist.clear();
	build_TRef(str, ret, true);
	LHSvar = nowvar;
	LHSvar.name = "d" + LHSvar.name;
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
		int tr = 0;
		if(cntgrad >= nowgrad)tr = 1;
		build_RHS(str.substr(0, i), res_LRHS);
		if(cntgrad >= nowgrad && (!tr))tr = 2;
		build_RHS(str.substr(i + 1), res_RRHS);
		if(cntgrad >= nowgrad && (!tr))tr = 3;
		if(tr == 2){
			deluse(str.substr(i + 1));
			ret = res_LRHS;
		}
		else if(tr == 3){
			ret = res_RRHS;
			deluse(str.substr(0, i));
		}
		else{
			if(str[i] == '+')
				ret = new Expr(Binary::make(data_type, BinaryOpType::Add, *res_LRHS, *res_RRHS));
			else
				ret = new Expr(Binary::make(data_type, BinaryOpType::Sub, *res_LRHS, *res_RRHS));
		}
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
	ret = new vector<Stmt>({});
	for(nowgrad = 1;; nowgrad++){
		usedom.clear();
		for(auto i : LHSusedom)
			usedom[i.first] = 1;
		domlist.clear();
		cntgrad = 0;
		Expr *res_RHS;
		build_RHS(str, res_RHS);
		//IRPrinter printer;
		vector<Expr>tempdomlist;
		string s = "a";
		//cout << graddomlist->size() << endl;
		for(int i = 0; i < gradvarlist->size(); i++){
			for(s[0]++; mydomlist.find(s) != mydomlist.end(); s[0]++);
			Expr thisdom = Dom::make(index_type, gradvarlist->at(i), Binary::make(data_type, BinaryOpType::Add, gradvarlist->at(i), 1));
			//Expr thisdom = Dom::make(index_type, 0, 10);
			tempdomlist.push_back(Index::make(index_type, s, thisdom, IndexType::Spatial));
		}
		
		now_grad_to = new Expr(Var::make(data_type, gradname, tempdomlist, myvarlist[gradname].shape));
		Stmt tem = LoopNest::make(tempdomlist, {Move::make(*now_grad_to, Binary::make(data_type, BinaryOpType::Add, *now_grad_to, 
		Binary::make(data_type, BinaryOpType::Mul, *res_LHS, *res_RHS)), MoveType::MemToMem)});
		/*Stmt tem = Move::make(*now_grad_to, Binary::make(data_type, BinaryOpType::Add, *now_grad_to, 
		Binary::make(data_type, BinaryOpType::Mul, *res_LHS, *res_RHS)), MoveType::MemToMem);*/
		if(domlist.size())
			ret->push_back(LoopNest::make(domlist, {tem}));
		else{
			ret->push_back(tem);
		}
		//cout << nowgrad << " " << cntgrad << endl;
		if(cntgrad == nowgrad)break;
	}
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
	build_LHS(str.substr(0, t), res_LHS);
	vector<Stmt> *res_RHS;
	build_RHS_list(str.substr(t + 1, n - t - 2), res_RHS);
	mycode.push_back(LoopNest::make(LHSdomlist, *res_RHS));
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
bool Input(int cnt, string &name, vector<string>&ins, vector<string>&outs, string &data_type, string &kernel, vector<string> &grad_to){
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
		object = root["grad_to"];
		for (int i = 0; i < object.size(); i++)
			grad_to.push_back(object[i].asString());
	}
	else return false;

	ifile.close();
	return true;

}

int main() {
	for(int i = 1; i <= 10; i++){
		string name, type, kernel;
		vector<string> ins, outs;
		grad_to.clear();
		if(!Input(i, name, ins, outs, type, kernel, grad_to)){
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
		//grad_to.clear();
		tempcnt = 0;
		build_P(st);
		for(auto t : grad_to){
			vector<Expr> domli;
			string str = "i";
			for(auto s : myvarlist["d" + t].shape){
				Expr thisdom = Dom::make(index_type, 0, (int)s);
				domli.push_back(Index::make(index_type, str, thisdom, IndexType::Spatial));
				str[0] = str[0] + 1;
			}
			mycode.insert(mycode.begin(), LoopNest::make(domli, {Move::make(Var::make(data_type, "d" + t, domli, myvarlist["d" + t].shape), 0, MoveType::MemToMem)}));
		}
		
		vector<Expr>input;
		vector<Expr>output;
		for(auto i : ins)if(myvarlist.find(i) != myvarlist.end() && myvarlist[i].use)
			input.push_back(Var::make(data_type, i, {}, myvarlist[i].shape));
		for(auto i : outs)
			output.push_back(Var::make(data_type, "d" + i, {}, myvarlist["d" + i].shape)); 
		for(auto i : grad_to)
			output.push_back(Var::make(data_type, "d" + i, {}, myvarlist["d" + i].shape));
		Group _kernel = Kernel::make(name, input, output, mycode, KernelType::CPU);
		IRPrinter printer;
		string code = printer.print(_kernel);
		//cout << code;
		if(i > 0){
			ofstream ofile("./kernels/grad_case" + to_string(i) + ".cc", std::ios::out);
			ofile << code;
			ofile.close();
		}else{
			ofstream ofile("./kernels/kernel_example.cc", std::ios::out);
			ofile << code;
			ofile.close();
		}
		//cout<< code;
		cout<< "case" + to_string(i) + " finished!" <<endl;
	}
    return 0;
}
