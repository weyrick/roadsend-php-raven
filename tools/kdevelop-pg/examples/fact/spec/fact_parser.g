------------------------------------------------------------
-- D E C L A R A T I O N
------------------------------------------------------------
   !function_definition
-> program ;;

   FUNCTION ID LPAREN (ID @ COMMA | 0) RPAREN body
-> function_definition ;;

   LBRACE !declaration !statement RBRACE 
-> body ;;

   VAR variable @ COMMA SEMICOLON
-> declaration ;;

   ID
-> variable ;;

------------------------------------------------------------
-- S T A T E M E N T
------------------------------------------------------------
   ID EQUAL expression SEMICOLON
-> assignment_statement ;;

   IF LPAREN condition RPAREN statement
      (ELSE statement | 0)
-> if_statement ;;

   LBRACE !statement RBRACE
-> block_statement ;;

   RETURN expression SEMICOLON
-> return_statement ;;

   assignment_statement
 | if_statement
 | block_statement
 | return_statement
-> statement ;;

------------------------------------------------------------
-- E X P R E S S I O N
------------------------------------------------------------
   NUMBER 
 | ID (LPAREN expression @ COMMA RPAREN | 0)
-> primary ;;

   primary !(STAR primary)
-> mult_expression ;;

   mult_expression !(MINUS mult_expression)
-> expression ;;

   expression EQUAL_EQUAL expression
-> condition ;;

