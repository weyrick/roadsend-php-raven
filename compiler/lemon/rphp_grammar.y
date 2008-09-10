/*

rphp language grammar

*/

%include {   

#include <iostream>
#include <string>
#include "pModule.h"
#include "pLangLexerDef.h"

using namespace rphp;

}  


%token_type {std::string*}
%default_type {std::string*}
%token_destructor { delete $$; }
%extra_argument {pModule* pMod}

//%type expr {Token}
//%type NUM {Token}

   
//%left PLUS MINUS.   
//%left DIVIDE TIMES.  
   
%parse_accept {
  printf("parsing complete!\n\n\n");
}

   
%syntax_error {  
  std::cout << "Syntax error!" << std::endl;  
}   
   
start ::= statement.
statement ::= echo.
echo ::= T_ECHO T_CONSTANT_ENCAPSED_STRING(A) T_SEMI. { std::cout << "found echo: " << A << std::endl; }

   
/*  This is to terminate with a new line */
/*
main ::= in.
in ::= .
in ::= in state NEWLINE.


state ::= expr(A).   { 
                        std::cout << "Result.value=" << A.value << std::endl; 
                        std::cout << "Result.n=" << A.n << std::endl; 

                         }  



expr(A) ::= expr(B) MINUS  expr(C).   { A.value = B.value - C.value; 
                                       A.n = B.n+1  + C.n+1;
                                      }  

expr(A) ::= expr(B) PLUS  expr(C).   { A.value = B.value + C.value; 
                                       A.n = B.n+1  + C.n+1;
                                      }  

expr(A) ::= expr(B) TIMES  expr(C).   { A.value = B.value * C.value;
                                        A.n = B.n+1  + C.n+1;

                                         }  
expr(A) ::= expr(B) DIVIDE expr(C).  { 

         if(C.value != 0){
           A.value = B.value / C.value;
           A.n = B.n+1 + C.n+1;
          }else{
           std::cout << "divide by zero" << std::endl;
           }
}  // end of DIVIDE
expr(A) ::= NUM(B). { A.value = B.value; A.n = B.n+1; }
*/


