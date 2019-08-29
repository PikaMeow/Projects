grammar Calculator;
import LexerRules;
p: (d|a|o)+;

d: key=('float'|'int') ID ';';

a: ID '=' e ';';

e: e op=('*'|'/') e           # MulDiv
 | e op=('+'|'-') e           # AddSub
 | NUMBER                     # number
 | ID                         # id
 | '(' e ')'                  # parens
 ;
o: 'write' '(' ID ')' ';';