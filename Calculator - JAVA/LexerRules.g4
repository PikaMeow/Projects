lexer grammar LexerRules;
ID: [a-zA-Z][a-zA-Z0-9]* ;
NUMBER: [0-9]+('.' [0-9]+)?;
WS: [ \t\r\n]+ -> skip ;
MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';