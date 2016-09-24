/*------------------------------------
 * @Created Time : 2016/5/31 9:19:12
 *-----------------------------------*/
#include "lexical_2.0.h"


const int P=20;
const int S=21;
const int D=22;
const int I=23;
const int A=24;
const int E=25;
const int G=26;
const int F=27;
const int N=28;
const int M=29;
const int O=30;

//存储已经声明的变量的信息
struct Var{
    char type;//变量类型
    char name[20];//变量ID名
    float value;//变量值
    bool assg;//变量是否已被赋值
};

//对应每个非终结符的结构体，用于语义分析
struct _m{
     float val;
};

struct _n{
    float inh;
    float syn;
};

struct _f{
     float val;
};

struct _g{
    float inh;
    float syn;
};

struct _e{
     float val;
};

struct _i{
    char inh;
};


_m M1;
_n N1;
_f F1;
_g G1;
_e E1;
_i I1;

Var v[MAXN];//用来保存已声明的变量
int var_p,tmp_var;//用来指示表v中的项
int anacode;//语义分析的错误码

void parseP();
void parseS();
void parseD();
void parseI();
void parseA();
void parseE();
void parseG();
void parseF();
void parseN();
void parseM();
void parseO();

//语法分析的错误处理，打印出三元组 < 第几个token token类型 token值 >
void ParseError() {
    int i=token_p;
    printf("Parsing error occured at token < %d ",i+1);
    if(t[i].attr==1||t[i].attr==2||t[i].attr==3) printf("KeyWord %s",(char*)t[i].value);
    else if(t[i].attr==4) printf("ID %s",(char*)t[i].value);
    else if(t[i].attr==5) { printf("Number %d",(int)t[i].num);}
    else printf("Symbol %c",*(char*)t[i].value);
    printf(" >\n");
    exit(1);
}

//语义分析的错误处理
void AnalyError() {
    printf("Semantic error occured. ");
    if(anacode==1) printf("Variable %s has to be declared firstly\n",(char*)t[token_p].value);
    else if(anacode==2) printf("Variable %s hasn't been assigned\n",(char*)t[token_p].value);
    else if(anacode==3) printf("Variable %s has been declared\n",(char*)t[token_p].value);
    exit(1);
}


//在Var类型数组v中查找是否有与a匹配的变量名
int findVar(char *a) {
    for(int i=0;i<var_p;i++) {
        if(strcmp(a,v[i].name)==0) {
            return i;
        }
    }
    return -1;
}
/*
void prints() {
     printf("%.2f %.2f %.2f %.2f %.2f %.2f ",
             M1.val,N1.inh,N1.syn,F1.val,G1.inh,G1.syn);
     printf("%.2f %c\n",
             E1.val,I1.inh);
}
void printaa() {
    for(int i=0;i<var_p;i++) {
        printf("%c %s %.2f\n",v[i].type,v[i].name,v[i].value);
    }
}
*/
int input;//用来指示在语法分析时取到了第几个token

//通过基于LL(1)表的递归下降方法处理每一个非终结符
//同时进行语义分析，即给非终结符相应结构体赋值
void parseP() {
    input=t[token_p].attr;
    if(input==FLOAT||input==INT||input==WRITE||input==ID) {parseS();parseP();return;}
    else if(input==END) {return;}
    else {ParseError();}
}

void parseS() {
    input=t[token_p].attr;
    if(input==FLOAT||input==INT) {parseD();return;}
    else if(input==ID) {parseA();return;}
    else if(input==WRITE) {parseO();return;}
    else {ParseError();}
}

void parseD() {
     input=t[token_p].attr;
     if(input==FLOAT) {I1.inh='f';token_p++;parseI();return;}
     else if(input==INT) {I1.inh='i';token_p++;parseI();return;}
     else {ParseError();}
}

void parseI() {
     input=t[token_p].attr;
     if(input==ID) {
         tmp_var=findVar((char*)t[token_p].value);
         if(tmp_var!=-1) {anacode=3;AnalyError();}
         memcpy(v[var_p].name,(char*)t[token_p].value,sizeof(t[token_p].value));
         v[var_p].type=I1.inh;
         var_p++;
         input=t[++token_p].attr;
         if(input!=SEMI) {ParseError();}
         token_p++;
         return;
     } else {ParseError();}
}

void parseA() {
     input=t[token_p].attr;
     if(input==ID) {
         int var_tmp=findVar((char*)t[token_p].value);
         if(var_tmp==-1) {anacode=1;AnalyError();}
         input=t[++token_p].attr;
         if(input!=EQU) {ParseError();}
         token_p++;
         parseE();
         v[var_tmp].value=E1.val;
         v[var_tmp].assg=true;
         if(tmp_var==-1) {anacode=1;AnalyError();}

         input=t[token_p].attr;
         if(input!=SEMI) {ParseError();}
         token_p++;
         return;
     } else {ParseError();}
}

void parseE() {
     input=t[token_p].attr;
     if(input==ID||input==NUMBER||input==LEFT) {parseF();G1.inh=F1.val;parseG();E1.val=G1.syn;return;}
     else {ParseError();}
}

void parseF() {
     input=t[token_p].attr;
     if(input==ID||input==NUMBER||input==LEFT) {parseM();N1.inh=M1.val;parseN();F1.val=N1.syn;return;}
     else {ParseError();}
}

void parseG() {
     input=t[token_p].attr;
     if(input==PLUS) {token_p++;parseF();G1.inh=G1.inh+F1.val;parseG();return;}
     else if(input==MINUS) {token_p++;parseF();G1.inh=G1.inh-F1.val;parseG();return;}
     else if(input==FLOAT||input==INT||input==ID||input==SEMI
             ||input==RIGHT||input==WRITE||input==END) {G1.syn=G1.inh;return;}
     else {ParseError();}
}

void parseN() {
     input=t[token_p].attr;
     if(input==MUL) {token_p++;parseM();N1.inh=N1.inh*M1.val;parseN();return;}
     else if(input==DIV) {token_p++;parseM();N1.inh=N1.inh/M1.val;parseN();return;}
     else if(input==FLOAT||input==INT||input==ID||input==SEMI
             ||input==RIGHT||input==WRITE||input==END
                ||input==PLUS||input==MINUS) {N1.syn=N1.inh;return;}
     else {ParseError();}
}

void parseM() {
     input=t[token_p].attr;
     if(input==ID) {
         tmp_var=findVar((char*)t[token_p].value);
         if(tmp_var!=-1) {M1.val=v[tmp_var].value;}
         else {anacode=1;AnalyError();}
         token_p++;
         return;}
     else if(input==NUMBER) {M1.val=t[token_p].num;token_p++;return;}
     else if(input==LEFT) {
         token_p++;
         parseE();
         M1.val=E1.val;
         input=t[token_p].attr;
         if(input!=RIGHT) {ParseError();}
         token_p++;
         return;
     } else {ParseError();}
}

void parseO() {
    input=t[token_p].attr;
    if(input==WRITE) {
         input=t[++token_p].attr;
         if(input!=LEFT) {ParseError();}
         input=t[++token_p].attr;
         if(input!=ID) {ParseError();}

         else {
             /*for(int i=0;i<var_p;i++) {
             printf("%c %s %.2f %d\n",v[i].type,v[i].name,v[i].value,v[i].assg);
            }*/
              tmp_var=findVar((char*)t[token_p].value);
              if(tmp_var==-1) {anacode=1;AnalyError();}
              else if(v[tmp_var].assg==false) {anacode=2;AnalyError();}
         }

         input=t[++token_p].attr;
         if(input!=RIGHT) {ParseError();}
         input=t[++token_p].attr;
         if(input!=SEMI) {ParseError();}
         else {
              if(v[tmp_var].type=='i') printf("%d\n",(int)v[tmp_var].value);
              else printf("%.2f\n",v[tmp_var].value);
         }
         token_p++;
         return;
    } else {ParseError();}
}

int main() {
    Lexical();
    token_p=0;
    var_p=0;
    memset(v,0,sizeof(v));
    parseP();
    /*
    for(int i=P;i<=O;i++) {
        for(int j=FLOAT;j<=END;j++) {
            if(pt[i][j][0]) {
                for(int k=1;k<=pt[i][j][0];k++)
                    printf("%d ",pt[i][j][k]);
            }
            printf("    ");
        }
        printf("\n");
    }
    */
    return 0;
}
