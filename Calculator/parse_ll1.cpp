#include "lexical.h"
#include <cstdlib>
int order=0;
string token;

void parseError(string token) {
    cout<<"Error occured at "<<token<<endl;
    exit(1);
}

void parseOut() {
    if(token=="write") {
        token=u[order++].token;
        if(token!="(") parseError(token);
        token=u[order++].token;
        if(token!="id") parseError(token);
        token=u[order++].token;
        if(token!=")") parseError(token);
        token=u[order++].token;
        if(token=="$") return;
        if(token!=";") parseError(token);
        token=u[order++].token;
        return;
    } else parseError(token);
}
void parseB();
void parseD() {
     if(token=="id") {token=u[order++].token;return;}
     else if(token=="num_i") {token=u[order++].token;return;}
     else if(token=="num_f") {token=u[order++].token;return;}
     else if(token=="(") {
         token=u[order++].token;
         parseB();
         if(token!=")") parseError(token);
         token=u[order++].token;
         return;
     } else parseError(token);
}

void parsec() {
    if(token=="*") {token=u[order++].token;parseD();parsec();return;}
    else if(token=="/") {token=u[order++].token;parseD();parsec();return;}
    else if(token=="+") {return;}
    else if(token=="-") {return;}
    else if(token==")") {return;}
    else if(token==";") {return;}
    else parseError(token);
}


void parseC() {
     if(token=="id") {parseD();parsec();return;}
     else if(token=="num_i") {parseD();parsec();return;}
     else if(token=="num_f") {parseD();parsec();return;}
     else if(token=="(") {parseD();parsec();return;}
     else parseError(token);
}

void parseb() {
    if(token=="+") {token=u[order++].token;parseC();parseb();return;}
    else if(token=="-") {token=u[order++].token;parseC();parseb();return;}
    else if(token=="*") {return;}
    else if(token=="/") {return;}
    else if(token==")") {return;}
    else if(token==";") {return;}
    else parseError(token);
}

void parseB() {
     if(token=="id") {parseC();parseb();return;}
     else if(token=="num_i") {parseC();parseb();return;}
     else if(token=="num_f") {parseC();parseb();return;}
     else if(token=="(") {parseC();parseb();return;}
     else parseError(token);
}

void parseAss() {
    if(token=="id") {
        token=u[order++].token;
        if(token!="=") parseError(token);
        token=u[order++].token;
        parseB();
        if(token!=";") parseError(token);
        token=u[order++].token;
        return;
    } else parseError(token);
}



void parseA() {
    if(token=="id") {
        token=u[order++].token;
        if(token!=";") parseError(token);
        token=u[order++].token;
        return;
    } else parseError(token);
}

void parseDec() {
    if(token=="float") {token=u[order++].token;parseA();return;}
    else if(token=="int") {token=u[order++].token;parseA();return;}
    else parseError(token);
}

void parseE() {
    if(token=="float") {parseDec();return;}
    else if(token=="int") {parseDec();return;}
    else if(token=="write") {parseOut();return;}
    else if(token=="id") {parseAss();return;}
    else parseError(token);
}

void parseProg() {
    if(token=="float") {parseE();parseProg();return;}
    else if(token=="int") {parseE();parseProg();return;}
    else if(token=="write") {parseE();parseProg();return;}
    else if(token=="id") {parseE();parseProg();return;}
    else if(token=="$") {return;}
    else {parseError(token);}
}





int main() {
    int cnt=run_lexical();

    for(int i=0;i<cnt;i++) {
        cout<<u[i].token;
        if(u[i].token=="id") cout<<" "<<u[i].value;
        if(u[i].token=="num_i") cout<<" "<<u[i].value_i;
        if(u[i].token=="num_f") cout<<" "<<u[i].value_f;
        printf("\n");
    }
    token=u[order++].token;
    parseProg();

    return 0;

}
