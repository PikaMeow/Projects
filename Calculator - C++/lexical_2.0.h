/*------------------------------------
 * @Created Time : 2016/5/30 18:25:24
 *-----------------------------------*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

const int FLOAT=1;
const int INT=2;
const int WRITE=3;
const int ID=4;
const int NUMBER=5;
const int PLUS=6;
const int MINUS=7;
const int MUL=8;
const int DIV=9;
const int EQU=10;
const int LEFT=11;
const int RIGHT=12;
const int SEMI=13;
const int END=14;
const int MAXN=1024;


//保存token的信息
struct Token{
    int attr;//代表的类型
    void *value;//某token的值
    float num;//如果是个数，保存其值
};

Token t[MAXN];//保存已获取到的token信息
int char_p,token_p,state,id_p,num_p;
//分别为:指示某个输入字符 指示某个token 指示某个状态 指示id的获取情况 指示num的获取情况
char prog[MAXN];//保存每一个输入字符
char ch;//token分析时指示每一个输入字符
char tmp[20],num_tmp[20];//临时保存ID和NUM的字符
int l_row,l_col;//用于错误处理，指示分析到了某个位置

//根据输入初始化token,并根据不同类型分配相应的空间
void initToken(int _attr=0,void* _value=NULL) {
     t[token_p].attr=_attr;
     void *value=t[token_p].value;
     if(_attr==FLOAT) {value=malloc(5);memcpy(value,_value,5*sizeof(char));}
        else if(_attr==INT) {value=malloc(3);memcpy(value,_value,3*sizeof(char));}
        else if(_attr==WRITE) {value=malloc(5);memcpy(value,_value,5*sizeof(char));}
        else if(_attr==ID) {value=malloc(sizeof(_value));memcpy(value,_value,sizeof(_value));}
        else if(_attr==NUMBER) {t[token_p].num=*(float*)_value;}
        else {value=malloc(sizeof(char));memcpy(value,_value,sizeof(char));}
     t[token_p].value=value;
     token_p++;
}

bool isLetter(char c) {return ((c>='a'&&c<='z')||(c>='A'&&c<='Z'));}
bool isDigit(char c) {return (c>='0'&&c<='9');}
bool isDelim(char c) {return (c=='\n'||c=='\t'||c=='\r'||c==' ');}

//判断是否为关键字
int isKey() {
    int len=strlen(tmp);
    if(len==5) {
        if(tmp[0]=='f'&&tmp[1]=='l'&&tmp[2]=='o'&&tmp[3]=='a'&&tmp[4]=='t') return FLOAT;
        else if(tmp[0]=='w'&&tmp[1]=='r'&&tmp[2]=='i'&&tmp[3]=='t'&&tmp[4]=='e') return WRITE;
    } else if(len==3) {
        if(tmp[0]=='i'&&tmp[1]=='n'&&tmp[2]=='t') return INT;
    }
    return 0;
}

//根据字符数组获取整型数字
float getNumI() {
     float n=0;
     for(int i=0;i<num_p;i++) {
         n=n*10+num_tmp[i]-'0';
     }
     return n;
}

//根据字符数组获取浮点型数字
float getNumF() {
     int pos;
     float n1=0,n2=0;
     for(int i=0;i<num_p;i++) if(num_tmp[i]=='.') {pos=i;break;}
     for(int i=num_p-1;i>pos;i--) {
        n1=n1*0.1+num_tmp[i]-'0';
     }
     n1*=0.1;
     for(int i=0;i<pos;i++) {
          n2=n2*10+num_tmp[i]-'0';
     }
     return n1+n2;
}


//词法分析的错误处理
void error() {
    printf("Lexical error occured at: row %d , column %d\n",l_row,l_col);
    exit(1);
}

//根据状态转换的DFA进行token分析
void getToken() {
    int flag=1;
    int bi;
    float bf;
    char c;
    l_row=1;
    l_col=0;
    int len=strlen(prog);
    while(flag&&char_p<len) {
        switch(state) {
            case 0:
                c=prog[char_p++];
                l_col++;
                if(isLetter(c)) {memset(tmp,0,sizeof(tmp));id_p=0;tmp[id_p++]=c;state=1;}
                else if(isDigit(c)) {memset(num_tmp,0,sizeof(num_tmp));num_p=0;num_tmp[num_p++]=c;state=3;}
                else if(isDelim(c)) {if(c=='\n') {l_row++;l_col=0;}state=8;}
                else if(c=='+') state=10;
                else if(c=='-') state=11;
                else if(c=='*') state=12;
                else if(c=='/') state=13;
                else if(c=='=') state=14;
                else if(c=='(') state=15;
                else if(c==')') state=16;
                else if(c==';') state=17;
                else if(c=='.') state=18;
                else error();
                break;
            case 1:
                c=prog[char_p++];
                l_col++;
                if(isLetter(c)||isDigit(c)) {tmp[id_p++]=c;state=1;}
                else state=2;
                break;
            case 2:
                bi=isKey();
                if(bi==0) initToken(ID,tmp);
                else if(bi==FLOAT) initToken(FLOAT,tmp);
                else if(bi==WRITE) initToken(WRITE,tmp);
                else if(bi==INT) initToken(INT,tmp);
                state=0;
                char_p--;
                l_col--;
                break;
            case 3:
                c=prog[char_p++];
                l_col++;
                if(isDigit(c)) {num_tmp[num_p++]=c;state=3;}
                else if(c=='.') {num_tmp[num_p++]=c;state=5;}
                else state=4;
                break;
            case 4:
                bf=getNumI();
                initToken(NUMBER,&bf);
                state=0;
                char_p--;
                l_col--;
                break;
            case 5:
                c=prog[char_p++];
                l_col++;
                if(isDigit(c)) {num_tmp[num_p++]=c;state=6;}
                else error();
                break;
            case 6:
                c=prog[char_p++];
                l_col++;
                if(isDigit(c)) {num_tmp[num_p++]=c;state=6;}
                else state=7;
                break;
            case 7:
                bf=getNumF();
                initToken(NUMBER,&bf);
                state=0;
                char_p--;
                break;
            case 8:
                c=prog[char_p++];
                l_col++;
                if(isDelim(c)) {if(c=='\n') {l_row++;l_col=0;} state=8;}
                else state=9;
                break;
            case 9:
                state=0;
                char_p--;
                l_col--;
                break;
            case 10:
                initToken(PLUS,&c);
                state=0;
                break;
            case 11:
                initToken(MINUS,&c);
                state=0;
                break;
            case 12:
                initToken(MUL,&c);
                state=0;
                break;
            case 13:
                initToken(DIV,&c);
                state=0;
                break;
            case 14:
                initToken(EQU,&c);
                state=0;
                break;
            case 15:
                initToken(LEFT,&c);
                state=0;
                break;
            case 16:
                initToken(RIGHT,&c);
                state=0;
                break;
            case 17:
                initToken(SEMI,&c);
                state=0;
                break;
            case 18:
                initToken(END,&c);
                state=0;
                flag=0;
                break;
            default:
                error();
                break;
        }
    }
    //printf("%d\n",token_p);
    t[token_p-1].attr=SEMI;
    *(char*)(t[token_p-1].value)=';';
    initToken(END,&c);
}

void Lexical() {

    //从文件"in"中读取所有字符存到prog数组中，有效字符为 0 ~ p-1
    ifstream fin("in");
    char_p=0;
    fin.get(ch);
    while(!fin.eof()) {
        prog[char_p++]=ch;
        fin.get(ch);
    }
    token_p=0;
    memset(t,0,sizeof(t));
    char_p=0;
    state=0;
    getToken();
/*
    for(int i=0;i<token_p;i++) {
         if(t[i].attr==1||t[i].attr==2||t[i].attr==3) printf("%s\n",(char*)t[i].value);
         else if(t[i].attr==4) printf("ID %s\n",(char*)t[i].value);
         else if(t[i].attr==5) { printf("NUMBER %f\n",t[i].num);}
         else printf("%d %c\n",t[i].attr,*(char*)t[i].value);
    }
*/
}

