#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <stack>
#include <sstream>
#include <iomanip>

using namespace std;


#define TNUM 7      // 终结符个数
#define LL_NNUM 5   // LL文法非终结符个数
#define LR_NNUM 4   // LR文法非终结符个数
#define STATENUM 16 // LR项目集规范族个数(状态个数)
#define LINEMAX 50  // 输出时一行缓冲区大小


typedef vector<string> Item;

// LL分析用到的数据类型
typedef map<string, Item> FIRST;
typedef map<string, Item> FOLLOW;
typedef map<string, map<string, string>> LL_Table;
typedef map<string, Item> Grammer;

// LR分析用到的数据类型
typedef map<int, map<string, string>> Action_Table;
typedef map<int, map<string, int>> Goto_Table;
typedef map<int, string> LR_Grammer;
typedef vector<int> State_Stack;

// 存储文法终结符和非终结符
const string LL_T[] = {"+","-","*","/","(",")","num","$"};
const string LR_T[] = {"+", "-", "*", "/", "(", ")", "num", "$"};
const string LL_N[] = {"E","T","F","A","B"};
const string LR_N[] = {"S", "E", "T", "F"};

// 初始化LL型去左递归文法
void Initial_Grammer(Grammer &);    
void Initial_FirstFollow(FIRST &, FOLLOW &);
void Build_LLTable(LL_Table &, Grammer &, FIRST &, FOLLOW &);

// 与打印相关的函数
void Print_Grammer(Grammer &);
void Print_First(FIRST &);
void Print_Follow(FOLLOW &);
void Print_Stack_Buffer(Item &, Item &, Item::iterator &);
void Print_LL_Table(LL_Table &);
void Print_LR_Table(Action_Table &, Goto_Table &);

// LL分析相关函数

// 判断终结符和非终结符并从参数返回标志,返回字符串
string Find_Pos(Item::iterator &, int *);
// 构建 LL 分析表
void Push_Table(LL_Table &, string ,string ,string , FIRST &, FOLLOW &, int);
// 寻找终结符位置
int Find_T_Pos(char );
// 初始化LL分析程序
void Initial_LL(string, LL_Table &, Item &, Item &);
// LL分析程序
void Analysis_LL(LL_Table &, Item &, Item &);


// LR分析相关函数

// 初始化LR文法
void Initial_LR_Grammer(LR_Grammer &);
// 构建LR文法分析表
void Build_LRTable(Action_Table &, Goto_Table &);
// LR分析程序
void Analysis_LR(LR_Grammer &, Action_Table &, Goto_Table &, State_Stack &, Item &, Item &);



int main()
{
    int i = 0;
    string input;


    /* LL Grammer Analysis var */
    Grammer LL_grammer;
    FIRST LL_first;
    FOLLOW LL_follow;
    LL_Table LL_table;
    Item in, LL_gram_stack;

    /* LR Grammer Analysis var */
    LR_Grammer LR_grammer;
    Action_Table action_table;
    Goto_Table goto_table;
    State_Stack state_stack;
    Item symbol_stack;
    
    /* LL Grammer Analysis */
    Initial_Grammer(LL_grammer);    
    Initial_FirstFollow(LL_first, LL_follow);
    Build_LLTable(LL_table, LL_grammer, LL_first, LL_follow);
    Print_LL_Table(LL_table);
    cout << "Input the sentence:";
    cin >> input;
    Initial_LL(input, LL_table, in, LL_gram_stack);
    cout << endl << "LL Analysis proc:" << endl;
    cout << "==============begin=================" << endl;
    Analysis_LL(LL_table, in, LL_gram_stack);
    cout << "================end=================" << endl << endl;

    /* LR Grammer Analysis */
    Initial_LR_Grammer(LR_grammer);
    Build_LRTable(action_table, goto_table);

    Print_LR_Table(action_table,goto_table);
    cout << "LR Analysis proc:" << endl;
    cout << "==============begin=================" << endl;
    Analysis_LR(LR_grammer, action_table, goto_table, state_stack, symbol_stack, in);
    cout << "================end=================" << endl;
    system("pause");
    return 0;
}

void Print_Grammer(Grammer &grammer)
{
    int i = 0;
    Item::iterator it;
    for (i = 0; i < 5; i++){
        for (it = grammer[LL_N[i]].begin(); it != grammer[LL_N[i]].end(); it++){
            cout << *it << " ";
        }
        cout << endl;
    }
}

void Print_First(FIRST &first)
{
    int i = 0;
    Item::iterator it;
    for (i = 0; i < 5; i++)
    {
        for (it = first[LL_N[i]].begin(); it != first[LL_N[i]].end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
    }
}

void Print_Follow(FOLLOW &follow)
{
    int i = 0;
    Item::iterator it;
    for (i = 0; i < 5; i++)
    {
        for (it = follow[LL_N[i]].begin(); it != follow[LL_N[i]].end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
    }
}

void Initial_Grammer(Grammer &grammer){
    Item item;
    item.push_back("TA");
    grammer[LL_N[0]] = item;
    item.clear();

    item.push_back("FB");
    grammer[LL_N[1]] = item;
    item.clear();

    item.push_back("(E)");item.push_back("num");
    grammer[LL_N[2]] = item;
    item.clear();

    item.push_back("+TA");item.push_back("-TA");item.push_back("NULL");
    grammer[LL_N[3]] = item;
    item.clear();

    item.push_back("*TB");item.push_back("/TB");item.push_back("NULL");
    grammer[LL_N[4]] = item;
    item.clear();
}

void Initial_FirstFollow(FIRST &first, FOLLOW &follow){
    /* make First set begin*/
    Item item;
    item.push_back("(");item.push_back("num");
    first[LL_N[0]] = item;
    first[LL_N[1]] = item;
    first[LL_N[2]] = item;
    item.clear();

    item.push_back("+");item.push_back("-");item.push_back("NULL");
    first[LL_N[3]] = item;
    item.clear();

    item.push_back("*");item.push_back("/");item.push_back("NULL");
    first[LL_N[4]] = item;
    item.clear();
    /* make First set finish*/

    /* make FOLLOW set begin*/
    item.push_back("$");
    item.push_back(")");
    follow[LL_N[0]] = item;
    follow[LL_N[3]] = item;
    item.clear();

    item.push_back("+");item.push_back("-");item.push_back("$");item.push_back(")");
    follow[LL_N[1]] = item;
    follow[LL_N[4]] = item;
    item.clear();

    item.push_back("*");item.push_back("/");item.push_back("+");item.push_back("-");item.push_back("$");item.push_back(")");
    follow[LL_N[2]] = item;
    item.clear();
    /* make FOLLOW set finish*/
}

void Build_LLTable(LL_Table &table, Grammer &grammer,FIRST &first, FOLLOW &follow){
    int i = 0,j = 0;
    int res = 0;
    Item::iterator it;
    string str;

    for(i = 0; i < grammer.size(); i++){
        // 扫描文法并且进行处理将其放入表table中
        for(it = grammer[LL_N[i]].begin(); it != grammer[LL_N[i]].end(); it++){
            
            if( *it == "NULL" )
                Push_Table(table, (string)LL_N[i], *it, "", first, follow, -1);
            else{
                str = Find_Pos(it, &res);
                if (str != ""){
                    Push_Table(table, (string)LL_N[i], *it, str, first, follow, res);
                }
            }
        }
    }
    for(i = 0; i < LL_NNUM; i++){
        // 错误synch放入表中
        for(it = follow[LL_N[i]].begin(); it != follow[LL_N[i]].end(); it++){
            if (table[LL_N[i]].find(*it) == table[LL_N[i]].end()){
                table[LL_N[i]][*it] = "synch";
            }
        }
    }
    return ;
}

string Find_Pos(Item::iterator &it, int *res){
    int i = 0;
    string tmp;

    if ('a' <= (*it).c_str()[0] && (*it).c_str()[0] <= 'z')
        tmp = *it;
    else
        tmp = (*it).c_str()[0];

    for(i = 0; i < LL_NNUM; i++){
        if(tmp == LL_N[i]){
            *res = 0;
            return LL_N[i];
        }
    }
    for(i = 0; i < TNUM; i++){
        if(tmp == LL_T[i]){
            *res = 1;
            return LL_T[i];
        }
    }
    
    return "";
}

void Push_Table(LL_Table &table, string N, string now, string str, FIRST &first, FOLLOW &follow, int tag){
    int i = 0, j = 0;
    Item::iterator it;
    if( tag == 0 ){
        for (it = first[str].begin(); it != first[str].end(); it++){
            table[N][*it] = now;
        }
    }else if(tag == 1){
        table[N][str] = now;
    }else{
        for(it = follow[N].begin(); it != follow[N].end(); it++){
            table[N][*it] = now;
        }
    }

}

int Find_T_Pos(char c){
    int i = 0;
    string tmp(1,c);
    for(i = 0; i < TNUM; i++){
        if(tmp == LL_T[i]){
            return i;
        }
    }
    return -1;
}

void Initial_LL(string input, LL_Table &table, Item &in, Item &gram_stack){
    int len, i, j, pos;
    string tmp;

    len = input.length();

    for(i = 0; i < len; i++){
        tmp.clear();
        if(input[i] <= '9' && input[i] >= '0'){
            while (input[i] <= '9' && input[i] >= '0'){
                tmp += input[i];
                i++;
            }
            in.push_back(tmp);
            i -= 1;
        }else{
            pos = Find_T_Pos(input[i]);
            if (pos == -1){
                cout << "error input" << endl;
                exit(0);
            }else{
                tmp = input[i];
                in.push_back(tmp);
            }
        }
    }
    in.push_back("$");
    gram_stack.push_back("$");
    gram_stack.push_back("E");
}

void Print_Stack_Buffer(Item &grammer, Item &in, Item::iterator &it){
    int i;
    Item::iterator tmp;
    stringstream out;

    for(tmp = grammer.begin(); tmp != grammer.end(); tmp++){
        out << *tmp << " ";
    }

    for(i = out.str().length();i < LINEMAX; i++){
        out << " ";
    }

    for(tmp = it; tmp != in.end(); tmp++){
        out << *tmp << " ";
    }
    cout << out.str() << endl;
}

void Analysis_LL(LL_Table &table, Item &in, Item &gram_stack){
    Item::iterator it, stack_it;
    string tmp, tmpc, it_str;
    int i = 0,len;
    it = in.begin();

    // 直到指针指向输入字符串结尾退出
    while(it != in.end()){
        // 若错误处理跳过过多报错
        if(gram_stack.size() == 0){
            cout << "LL Analysis ERROR" << endl;
            exit(0);
        }
        Print_Stack_Buffer(gram_stack, in, it);
        stack_it = gram_stack.end() - 1;

        
        if( *stack_it == "num" || Find_T_Pos((*stack_it)[0]) != -1 || *stack_it == "$"){
            if( *stack_it == "num" ){
                // 若为 num 弹出num, it前移
                if( (*it)[0] <= '9' && (*it)[0] >= '0' ){
                    gram_stack.pop_back();
                    it++;
                }
            }else if(Find_T_Pos((*stack_it)[0]) != -1 || *stack_it == "$"){
                if ( *stack_it == *it){
                    // 弹出终结符
                    gram_stack.pop_back();
                    it++;
                }else{
                    // 错误处理
                    cout << "error , pop from grammer stack" << endl;
                    gram_stack.pop_back();
                }
            }
        }else{
            if( (*it)[0] <= '9' && (*it)[0] >= '0' ) it_str = "num";
            else it_str = *it;

            if (table[*stack_it].find(it_str) != table[*stack_it].end()){
                // 错误处理
                if( gram_stack.size() != 0 ){
                    gram_stack.pop_back();
                }else{
                    cout << "LL Analysis ERROR" << endl;
                    return ;
                }

                // 非终结符弹出与压入
                tmp = table[*stack_it][it_str];
                if( tmp != "num" && tmp != "NULL" && tmp != "synch"){
                    len = tmp.length();
                    for (i = len - 1; i >=0 ; i--){
                        tmpc.push_back(tmp[i]);
                        gram_stack.push_back(tmpc);
                        tmpc.clear();
                    }
                }else if(tmp == "num"){
                    gram_stack.push_back(tmp);
                }else if(tmp == "synch"){
                    cout << "error , <synch> pop from grammer stack" << endl;
                }
            }else{
                cout << "error, move input ptr" << endl;
                it++;
            }
        }
        
    }
}

void Print_LL_Table(LL_Table &LL_table){
    int i = 0, j = 0;
    cout << "LL_Table:" << endl;
    for(i = 0; i < TNUM + 1; i++){
        cout << setw(10) << LL_T[i];
    }
    cout << endl;
    for(i = 0; i < LL_NNUM ; i++){
        cout << LL_N[i] << "        ";
        for(j = 0; j < TNUM + 1; j++){
            if (LL_table[LL_N[i]].find(LL_T[j]) != LL_table[LL_N[i]].end()){
                cout << setiosflags(ios::left) << setw(10) << LL_table[LL_N[i]][LL_T[j]];
            }else{
                cout << setw(10) << " ";
            }
        }
        cout << endl;
    }
}

void Initial_LR_Grammer(LR_Grammer &grammer){
    grammer[0] = "S->E";
    grammer[1] = "E->E+T";
    grammer[2] = "E->E-T";
    grammer[3] = "E->T";
    grammer[4] = "T->T*F";
    grammer[5] = "T->T/F";
    grammer[6] = "T->F";
    grammer[7] = "F->(E)";
    grammer[8] = "F->num";

}

void Build_LRTable(Action_Table &action_t, Goto_Table &goto_t){
    int i = 0,j = 0;
    string R_term[] = {"+","-","*","/",")","$"};
    map<int,string> tmp_map;

    tmp_map[2] = "3";
    tmp_map[3] = "6";
    tmp_map[5] = "8";
    tmp_map[7] = "7";
    tmp_map[10] = "2";
    tmp_map[13] = "4";
    tmp_map[14] = "5";
    tmp_map[15] = "1";

    for(i = 0; i < STATENUM; i++){
        if(tmp_map.find(i) != tmp_map.end()){
            for(j = 0; j < 6; j++){
                action_t[i][R_term[j]] = "R" + tmp_map[i];
            }
        }
    }
    
    action_t[0]["("] = "S4";action_t[0]["num"] = "S5";
    action_t[1]["+"] = "S8";action_t[1]["-"] = "S9";action_t[1]["$"] = "ACC";
    action_t[2]["*"] = "S11";action_t[2]["/"] = "S12";
    action_t[4]["("] = "S4";action_t[4]["num"] = "S5";
    action_t[6]["+"] = "S8";action_t[6]["-"] = "S9";action_t[6][")"] = "S7";
    action_t[8]["("] = "S4";action_t[8]["num"] = "S5";
    action_t[9]["("] = "S4";action_t[9]["num"] = "S5";
    action_t[10]["*"] = "S11";action_t[10]["/"] = "S12";
    action_t[11]["("] = "S4";action_t[11]["num"] = "S5";
    action_t[12]["("] = "S4";action_t[12]["num"] = "S5";
    action_t[15]["*"] = "S11";action_t[15]["/"] = "S12";

    goto_t[0]["E"] = 1;goto_t[0]["T"] = 2;goto_t[0]["F"] = 3;
    goto_t[4]["E"] = 6;goto_t[4]["T"] = 2;goto_t[4]["F"] = 3;
    goto_t[8]["T"] = 15;goto_t[8]["F"] = 3;
    goto_t[9]["T"] = 10;goto_t[9]["F"] = 3;
    goto_t[11]["F"] = 13;
    goto_t[12]["F"] = 14;
}

void Print_LR_Table(Action_Table &action_t, Goto_Table &goto_t){
    int i = 0, j = 0;
    cout << "LR_Table:" << endl;
    cout << setiosflags(ios::right);
    cout << "  ";
    for(i = 0; i < TNUM + 1; i++){
        cout << setw(6) << LR_T[i];
    }
    for(i = 1; i < LR_NNUM ; i++){
        cout << setw(6) << LR_N[i];
    }
    cout << endl;
    for(i = 0; i < STATENUM ; i++){
        cout << setiosflags(ios::right) << setw(2) << i;
        for(j = 0; j < TNUM + 1; j++){
            if (action_t[i].find(LR_T[j]) != action_t[i].end()){
                cout << setiosflags(ios::right) << setw(6) << action_t[i][LR_T[j]];
            }else{
                cout << "      " << setw(6);
            }
        }
        for(j = 1; j < LR_NNUM ; j++){
            cout << setw(6) << goto_t[i][LR_N[j]];
        }
        cout << endl;
    }
    cout << endl << endl;
}

void Analysis_LR(LR_Grammer &grammer, Action_Table &action_t, Goto_Table &goto_t, State_Stack &state_stack, Item &symbol_stack, Item &in){
    // initial state_stack
    state_stack.push_back(0);
    Item::iterator it = in.begin(), symbol_it;
    int tmp_num, i, len;
    string tmp_str, action_str, reduce_str;
    State_Stack::iterator state_it;


    while (1){
        symbol_it = symbol_stack.end() - 1;
        state_it = state_stack.end() - 1;

        for(i = 0; i < state_stack.size(); i++){
            cout << state_stack[i] << " ";
        }
        cout << endl;
        Print_Stack_Buffer(symbol_stack, in, it);
        cout << endl;
        if( (*it)[0] <= '9' && (*it)[0] >= '0' ){
            tmp_str = "num";
        }else{
            tmp_str = *it;
        }
        action_str = action_t[*state_it][tmp_str];
        if( action_str[0] == 'S' ){
            // 移进处理
            tmp_num = 0;
            for(i = 1; i < action_str.length(); i++){
                tmp_num = tmp_num * 10 + action_str[i] - '0';
            };
            state_stack.push_back(tmp_num);
            symbol_stack.push_back(tmp_str);
            it++;
        }else if( action_str[0] == 'R' ){
            // 规约处理
            tmp_num = 0;
            for(i = 1; i < action_str.length(); i++){
                tmp_num = tmp_num * 10 + action_str[i] - '0';
            };
            if( tmp_num != 8 ) len = grammer[tmp_num].length() - 3;
            else len = 1;

            while(len--){
                state_stack.pop_back();
                symbol_stack.pop_back();
            }
            reduce_str = grammer[tmp_num][0];
            symbol_stack.push_back(reduce_str);
            state_stack.push_back(goto_t[*(state_stack.end() - 1)][reduce_str]);
        }
        else if (action_str == "ACC"){
            // 接受
            cout << "success accept" << endl;
            return ;
        }else{
            // 错误
            cout << " error , exit" << endl;
            return ;
        }
    
    }
}
