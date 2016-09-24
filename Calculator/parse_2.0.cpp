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

int pt[40][20][10];
int stack[MAXN];
int top;
struct Var{
    char type;
    char name[20];
    float value;
    bool assg;
};

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

Var v[MAXN];
int var_p;
void initPt() {
    memset(pt,0,sizeof(pt));
    pt[P][FLOAT][0]=pt[P][INT][0]=pt[P][ID][0]=pt[P][WRITE][0]=2;
    pt[P][FLOAT][1]=pt[P][INT][1]=pt[P][ID][1]=pt[P][WRITE][1]=S;
    pt[P][FLOAT][2]=pt[P][INT][2]=pt[P][ID][2]=pt[P][WRITE][2]=P;
    pt[P][END][0]=1;
    pt[P][END][1]=-1;

    pt[S][FLOAT][0]=pt[S][INT][0]=pt[S][ID][0]=pt[S][WRITE][0]=1;
    pt[S][FLOAT][1]=pt[S][INT][1]=D;
    pt[S][ID][1]=A;
    pt[S][WRITE][1]=O;

    pt[D][FLOAT][0]=pt[D][INT][0]=2;
    pt[D][FLOAT][1]=FLOAT;
    pt[D][INT][1]=INT;
    pt[D][FLOAT][2]=pt[D][INT][2]=I;

    pt[I][ID][0]=2;
    pt[I][ID][1]=ID;
    pt[I][ID][2]=SEMI;

    pt[A][ID][0]=4;
    pt[A][ID][1]=ID;
    pt[A][ID][2]=EQU;
    pt[A][ID][3]=E;
    pt[A][ID][4]=SEMI;

    pt[E][ID][0]=pt[E][NUMBER][0]=pt[E][LEFT][0]=2;
    pt[E][ID][1]=pt[E][NUMBER][1]=pt[E][LEFT][1]=F;
    pt[E][ID][2]=pt[E][NUMBER][2]=pt[E][LEFT][2]=G;

    pt[G][FLOAT][0]=pt[G][INT][0]=pt[G][ID][0]=pt[G][SEMI][0]
        =pt[G][RIGHT][0]=pt[G][WRITE][0]=pt[G][END][0]=1;
    pt[G][FLOAT][1]=pt[G][INT][1]=pt[G][ID][1]=pt[G][SEMI][1]
        =pt[G][RIGHT][1]=pt[G][WRITE][1]=pt[G][END][1]=-1;
    pt[G][PLUS][0]=pt[G][MINUS][0]=3;
    pt[G][PLUS][1]=PLUS;
    pt[G][MINUS][1]=MINUS;
    pt[G][PLUS][2]=pt[G][MINUS][2]=F;
    pt[G][PLUS][3]=pt[G][MINUS][3]=G;


    pt[F][ID][0]=pt[F][NUMBER][0]=pt[F][LEFT][0]=2;
    pt[F][ID][1]=pt[F][NUMBER][1]=pt[F][LEFT][1]=M;
    pt[F][ID][2]=pt[F][NUMBER][2]=pt[F][LEFT][2]=N;


    pt[N][FLOAT][0]=pt[N][INT][0]=pt[N][ID][0]=pt[N][SEMI][0]
        =pt[N][RIGHT][0]=pt[N][WRITE][0]=pt[N][END][0]
            =pt[N][PLUS][0]=pt[N][MINUS][0]=1;
    pt[N][FLOAT][1]=pt[N][INT][1]=pt[N][ID][1]=pt[N][SEMI][1]
        =pt[N][RIGHT][1]=pt[N][WRITE][1]=pt[N][END][1]
            =pt[N][PLUS][1]=pt[N][MINUS][1]=-1;
    pt[N][MUL][0]=pt[N][DIV][0]=3;
    pt[N][MUL][1]=MUL;
    pt[N][DIV][1]=DIV;
    pt[N][MUL][2]=pt[N][DIV][2]=M;
    pt[N][MUL][3]=pt[N][DIV][3]=N;

    pt[M][ID][0]=pt[M][NUMBER][0]=1;
    pt[M][LEFT][0]=3;
    pt[M][ID][1]=ID;
    pt[M][NUMBER][1]=NUMBER;
    pt[M][LEFT][1]=LEFT;
    pt[M][LEFT][2]=E;
    pt[M][LEFT][3]=RIGHT;

    pt[O][WRITE][0]=5;
    pt[O][WRITE][1]=WRITE;
    pt[O][WRITE][2]=LEFT;
    pt[O][WRITE][3]=ID;
    pt[O][WRITE][4]=RIGHT;
    pt[O][WRITE][5]=SEMI;
}
void ParseError() {printf("Error\n");}
void AnalyError() {printf("ErrorE\n");}
int findVar(char *a) {
    for(int i=0;i<var_p;i++) {
        if(strcmp(a,v[i].name)==0) {
            return i;
        }
    }
    return -1;
}

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
void Analyse(int nonter,int ter,int i) {
    prints();
    printaa();
    printf("%d %d\n",nonter,ter);
    //for(int i=1;i<=pt[nonter][ter][0];i++) {
        int res=pt[nonter][ter][i];
        int tmp_var;
        if(nonter==M) {
            if(res==NUMBER) {
                M1.val=t[token_p].num;
            } else if(res==ID) {
                tmp_var=findVar((char*)t[token_p].value);
                if(tmp_var!=-1) {
                    M1.val=v[tmp_var].value;
                } else AnalyError();
            } else if(res==E) {
                 M1.val=E1.val;
            }
        } else if(nonter==F) {
            /*if(res==M) {
                 N1.inh=M1.val;
            } else */if(res==N) {
                 F1.val=N1.syn;
            }
        } else if(nonter==N) {
            if(res==-1) {
                N1.syn=N1.inh;
            } else if(res==M&&pt[nonter][ter][i-1]==MUL) {
                N1.inh=N1.inh*M1.val;
            } else if(res==M&&pt[nonter][ter][i-1]==DIV) {
                N1.inh=N1.inh/M1.val;
            } else if(res==MUL||res==DIV) {
                N1.inh=M1.val;
            }/*else if(res==N) {
                N1.syn=N2.syn;
            }*/
        } else if(nonter==G) {
            if(res==-1) {
                G1.syn=G1.inh;
            } else if(res==F&&pt[nonter][ter][i-1]==PLUS) {
                G1.inh=G1.inh+F1.val;
            } else if(res==F&&pt[nonter][ter][i-1]==MINUS) {
                G1.inh=G1.inh-F1.val;
            } else if(res==PLUS||res==MINUS) {
                 G1.inh=F1.val;
            }
            /*else if(res==G) {
                G1.syn=G2.syn;
            }*/
        } else if(nonter==E) {
            /*if(res==F) {
                G1.inh=F1.val;
            } else */
            if(res==G) {
                E1.val=G1.syn;
            }
        } else if(nonter==D) {
            if(res==INT) {
                I1.inh='i';
            } else if(res==FLOAT) {
                I1.inh='f';
            }
        } else if(nonter==I) {
            if(res==ID) {
                memcpy(v[var_p].name,(char *)t[token_p].value,sizeof(t[token_p].value));
                v[var_p].type=I1.inh;
                var_p++;
            }
        } else if(nonter==A) {
            if(res==ID) {
                tmp_var=findVar((char*)t[token_p].value);
                if(tmp_var==-1) AnalyError();
            } else if(res==E) {
                v[tmp_var].value=E1.val;
                v[tmp_var].assg=true;
            }
        } else if(nonter==O) {
            if(res==ID) {
                tmp_var=findVar((char*)t[token_p].value);
                if(tmp_var==-1||v[tmp_var].assg==false) AnalyError();
            } else if(res==SEMI) {
                if(v[tmp_var].type=='i') printf("%d\n",(int)v[tmp_var].value);
                else printf("%f\n",v[tmp_var].value);
            }
        }
    //}
}
void printa() {
    for(int i=0;i<=top;i++) printf("%d ",stack[i]);
    printf("\n");
}
void Parse() {
     token_p=0;
     top=0;
     stack[top]=END;
     stack[++top]=P;
     int cur_s;
     int cur_t;
     memset(v,0,sizeof(v));
     var_p=0;
     while(stack[top]!=END) {
         //printa();
         cur_s=stack[top];
         cur_t=t[token_p].attr;
         if(cur_s<20) {
             if(cur_s==-1) {
                 top--;
             } else if(cur_s==cur_t) {
                 top--;
                 token_p++;
             } else ParseError();
         } else if(pt[cur_s][cur_t][0]) {
             top--;
             for(int i=pt[cur_s][cur_t][0];i>0;i--) {
                  stack[++top]=pt[cur_s][cur_t][i];
             }
             for(int i=1;i<=pt[cur_s][cur_t][0];i++) {
                 if(pt[cur_s][cur_t][i]<20) Analyse(cur_s,cur_t,i);
             }
             //Analyse(cur_s,cur_t);
         } else ParseError();
     }
}

int main() {
    Lexical();
    initPt();
    Parse();
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
