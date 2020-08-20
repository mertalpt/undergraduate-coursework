%{
  #include <string>
  #include <iostream>
  using namespace std;
  int level = 0;
  string comment;
%}

%x IN_COMMENT 

string_literal \"([^\"\\\n]|\\\"|\\n|\\t|\\\\)*\" 

%%

<INITIAL>{string_literal} ECHO;

<INITIAL>"/*"  {
  BEGIN(IN_COMMENT);
  comment = "";
  level = 1;
 }
<IN_COMMENT>"*/" { 
  level--;
  if (level==0) { 
    BEGIN(INITIAL);
    cerr << "[COMMENT_START" << comment << "COMMENT_END]"; 
  } else 
    comment += yytext;
}
<IN_COMMENT>"/*" {
  level++;
  comment += yytext;
}
<IN_COMMENT>(.|\n) {
  comment += yytext;
}
<IN_COMMENT>{string_literal} {
  comment += yytext;
}

%%

int main() { 
  yylex();
  return 0; 
}
