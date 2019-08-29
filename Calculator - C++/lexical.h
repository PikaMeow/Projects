#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
char prog[1024];
char token_tmp[20];
string Token;
int p,m,syn;
int sum_i;
float sum_f;
char ch;
string keyword[3]={"float","int","write"};

struct unit{
    string token;
    string value;
    float value_f;
    int value_i;
}u[100];


using namespace std;

void scanner() {
    memset(token_tmp,0,sizeof(token_tmp));
    ch=prog[p++];
    while(ch==' '||ch=='\t'||ch=='\n') { ch=prog[p++]; }
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) {
        m=0;
        while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) {
            token_tmp[m++]=ch;
            ch=prog[p++];
        }
        token_tmp[m++]='\0';
        Token=token_tmp;
        //cout<<token<<endl;
        p--;
        syn=4;
        for(int i=0;i<3;i++) {
            if(Token.compare(keyword[i])==0) {
                 syn=i+1;
                 break;
            }
        }
    } else if((ch>='0'&&ch<='9')) {
        sum_i=0;
        while((ch>='0'&&ch<='9')) {
            sum_i=sum_i*10+ch-'0';
            ch=prog[p++];
        }
        if(ch=='.') {
            sum_f=sum_i;
            float rate=0.1;
            ch=prog[p++];
            while((ch>='0'&&ch<='9')) {
                sum_f=sum_f+(ch-'0')*rate;
                 rate*=0.1;
                 ch=prog[p++];
            }
            syn=6;
            p--;
        } else {
            syn=5;
            p--;

        }
    } else switch(ch) {
        case '=':syn=7;Token="=";break;
        case '(':syn=8;Token="(";break;
        case ')':syn=9;Token=")";break;
        case '+':syn=10;Token="+";break;
        case '-':syn=11;Token="-";break;
        case '*':syn=12;Token="*";break;
        case '/':syn=13;Token="/";break;
        case ';':syn=14;Token=";";break;
        default:break;
    }
        //printf("%d\n",p);

}

int run_lexical() {
    ifstream fin("in");
    do {
        fin.get(ch);
        prog[p++]=ch;
    } while(!fin.eof());
    int l=strlen(prog)-1;
    //for(int i=0;i<=l-1;i++) printf("%c ",prog[i]);
    //printf("\n");
    p=0;
    int cnt=0;
    do {
        scanner();

        switch(syn) {
            case 1:
            case 2:
            case 3:
                u[cnt].token=Token;break;
            case 4:
                u[cnt].token="id";u[cnt].value=Token;break;
            case 5:
                u[cnt].token="num_i";u[cnt].value_i=sum_i;break;
            case 6:
                u[cnt].token="num_f";u[cnt].value_f=sum_f;break;
            default:
                u[cnt].token=Token;break;
        }
        cnt++;
        //printf("%d\n",p);
    } while(p<=l-1);
    u[cnt-1].token="$";
    return cnt;
}
/*
int main() {
    run_lexical();
    return 0;
}
*/
