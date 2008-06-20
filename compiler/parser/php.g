-------------------------------------------------------------------------------
-- Copyright (c) 2008 Niko Sams <niko.sams@gmail.com>
-------------------------------------------------------------------------------

-----------------------------------------------------------
-- Grammar for PHP 5.2
-- Modelled after the Zend Grammar shipped with PHP5.2
-- source, the PHP Language Reference documentation,
-- and parts taken from KDevelop Java Grammar
-----------------------------------------------------------

-- 3 first/first conflicts:
--  - var_expression: variable vs. varExpressionNormal
--    no problem because of ifs that allow always just one rule
--  - classNameReference: STRING vs. staticMember (foo vs. foo::$bar)
--    resolved by LA()
--  - encapsVar: STRING_VARNAME LBRACKET vs. expr (expr allows STRING_VARNAME too - but not LBRACKET)
--    resolved by LA()
-- 1 first/follow conflicts:
--  - elseifList: dangling-else conflict - should be ok


[:

#include <QtCore/QString>
#include <kdebug.h>

namespace Php
{
    class Lexer;
    enum NumericType  {
        LongNumber,
        DoubleNumber,
    };
}
:]


------------------------------------------------------------
-- Export macro to use the parser in a shared lib
------------------------------------------------------------
%export_macro "KDEVPHPPARSER_EXPORT"
%export_macro_header "parserexport.h"

------------------------------------------------------------
-- Parser class members
------------------------------------------------------------

%parserclass (public declaration)
[:
  /**
   * Transform the raw input into tokens.
   * When this method returns, the parser's token stream has been filled
   * and any parse*() method can be called.
   */
  void tokenize( const QString& contents );

  enum ProblemType {
      Error,
      Warning,
      Info
  };
  void reportProblem( Parser::ProblemType type, const QString& message );
  QString tokenText(qint64 begin, qint64 end);
  void setDebug(bool debug);

:]

%parserclass (private declaration)
[:
    enum VarExpressionState {
        Normal,
        OnlyVariable,
        OnlyNewObject
    };
    QString m_contents;
    bool m_debug;

    struct ParserState {
        VarExpressionState varExpressionState;
        bool varExpressionIsVariable;
    };
    ParserState m_state;
:]

%parserclass (constructor)
[:
    m_state.varExpressionState = Normal;
    m_state.varExpressionIsVariable = false;
:]

-----------------------------------------------------------
-- List of defined tokens
-----------------------------------------------------------








-- keywords:
%token ABSTRACT ("abstract"), BREAK ("break"), CASE ("case"), CATCH ("catch"),
       CLASS ("class"), CONST ("const"), CONTINUE ("continue"),
       DEFAULT ("default"), DO ("do"), ELSE ("else"), EXTENDS ("extends"),
       FINAL ("final"), FOR ("for"), IF ("if"),
       IMPLEMENTS ("implements"), INSTANCEOF ("instanceof"), INTERFACE ("interface"),
       NEW ("new"), PRIVATE ("private"), PROTECTED ("protected"), PUBLIC ("public"),
       RETURN ("return"), STATIC ("static"), SWITCH ("switch"), THROW ("throw"),
       TRY ("try"), WHILE ("while"), ECHO ("echo"), PRINT ("print"),
       CLONE ("clone"), EXIT ("exit"), ELSEIF ("elseif"), ENDIF ("endif"),
       ENDWHILE ("endwhile"), ENDFOR ("endfor"), FOREACH ("foreach"), ENDFOREACH ("endforeach"),
       DECLARE ("declare"), ENDDECLARE ("enddeclare"), AS ("as"), ENDSWITCH ("endswitch"),
       FUNCTION ("function"), USE ("use"), GLOBAL ("global"), VAR ("var "), UNSET ("unset"),
       ISSET ("isset"), EMPTY ("empty"), HALT_COMPILER ("halt compiler"),
       DOUBLE_ARROW ("=>"), LIST ("list"), ARRAY ("array"), CLASS_C ("__CLASS__"),
       METHOD_C ("__METHOD__"), FUNC_C ("__FUNCTION__"), LINE ("__LINE__"), 
       FILE ("__FILE__"), COMMENT ("comment"), DOC_COMMENT ("doc comment"),  PAAMAYIM_NEKUDOTAYIM ("::"),
       INCLUDE ("include"), INCLUDE_ONCE ("include_once"), EVAL ("eval"), REQUIRE ("require"),
       REQUIRE_ONCE ("require_once") ;;

-- casts:
%token INT_CAST ("int cast"), DOUBLE_CAST ("double cast"), STRING_CAST ("string cast"),
       ARRAY_CAST ("array cast"), OBJECT_CAST ("object cast"), BOOL_CAST ("bool cast"),
       UNSET_CAST ("unset cast") ;;

-- seperators:
%token SEMICOLON (";"), DOUBLE_QUOTE ("\""), LBRACKET ("["),
       RBRACKET ("]"),
       LPAREN ("("), RPAREN (")"), LBRACE ("{"), RBRACE ("}"),
       COMMA (","), AT ("@"),
       CURLY_OPEN ("curly open"), -- { in "{$foo}"; not the same as LBRACE
       DOLLAR_OPEN_CURLY_BRACES ("${"),
       START_HEREDOC ("start heredoc"), END_HEREDOC ("end heredoc"),
       BACKTICK ("`");;

-- operators:
%token IS_EQUAL ("=="), IS_NOT_EQUAL ("!="), IS_IDENTICAL ("==="),
       IS_NOT_IDENTICAL ("!=="), IS_SMALLER ("<"), IS_GREATER (">"),
       IS_SMALLER_OR_EQUAL ("<="), IS_GREATER_OR_EQUAL (">="),
       BOOLEAN_OR ("||"), BOOLEAN_AND ("&&"), ASSIGN ("="),
       PLUS_ASSIGN ("+="), MINUS_ASSIGN ("-="), MUL_ASSIGN ("*="), DIV_ASSIGN ("/="),
       CONCAT_ASSIGN (".="), MOD_ASSIGN ("%="), AND_ASSIGN ("&="), OR_ASSIGN ("|="),
       XOR_ASSIGN ("^="), SL_ASSIGN ("<<="), SR_ASSIGN (">>="), OBJECT_OPERATOR ("->"),
       PLUS ("+"), MINUS("-"), CONCAT("."),
       INC ("++"), DEC ("--"), BANG ("!"), QUESTION ("?"), COLON (":"),
       BIT_AND ("&"), BIT_OR("|"), BIT_XOR ("^"),
       SL ("<<"), SR (">>"), MUL("*"), DIV("/"), MOD ("%"),
       TILDE ("~"), DOLLAR ("$"),
       LOGICAL_OR ("logical or"), LOGICAL_AND ("logical and"), LOGICAL_XOR ("logical xor") ;;

-- literals and identifiers:
%token INLINE_HTML ("inline html"), WHITESPACE ("whitespace"),
       CONSTANT_ENCAPSED_STRING ("constant encapsed string"),
       VARIABLE ("variable"), ENCAPSED_AND_WHITESPACE ("encapsed and whitespace"),
       DNUMBER ("double number"), LNUMBER ("long number"),
       NUM_STRING ("num string"), STRING ("string"),
       STRING_VARNAME ("string varname") ;; -- when in "${varname}"

-- open/close tags
%token OPEN_TAG ("<?"), CLOSE_TAG ("?>"), OPEN_TAG_WITH_ECHO ("<?=");;

-- token that makes the parser fail in any case:
%token INVALID ("invalid token") ;;



-- The actual grammar starts here.

statements=innerStatementList
-> start ;;

    statement=statement
  | functionDeclaration=functionDeclarationStatement
  | classDeclaration=classDeclarationStatement
  | HALT_COMPILER LPAREN RPAREN SEMICOLON -- Lexer stops allready
-> topStatement ;;

#statements=topStatement*
-> innerStatementList ;;

--Operator Precedence, from PHP Manual
--left    or
--left    xor
--left    and
--right   print
--right   = += -= *= /= .= %= &= |= ^= <<= >>=    assignment
--left    ? : ternary
--left    ||  logical
--left    &&  logical
--left    |   bitwise
--left    ^   bitwise
--left    &   bitwise and references
--non-associative == != === !==   comparison
--non-associative < <= > >=   comparison
--left    << >>   bitwise
--left    + - .   arithmetic and string
--left    * / %   arithmetic
--non-associative ! ~ - (int) (float) (string) (array) (object) @ types
--non-associative ++ --   increment/decrement
--left    [   array()
--non-associative new new

  expression=logicalOrExpression
-> expr ;;

   #expression=logicalXorExpression @ LOGICAL_OR
-> logicalOrExpression ;;

   #expression=logicalAndExpression @ LOGICAL_XOR
-> logicalXorExpression ;;

   #expression=printExpression @ LOGICAL_AND
-> logicalAndExpression ;;

  (print=PRINT*) expression=assignmentExpression
-> printExpression ;;

-- leftside must me a variable, we check afterwards if it was a variable and
-- if not we report an error
0 --needed for line below
[: m_state.varExpressionIsVariable = false; :] --reset flag
conditionalExpression=conditionalExpression
(
  assignmentAxpressionEqual=assignmentExpressionEqual | (
     (PLUS_ASSIGN | MINUS_ASSIGN | MUL_ASSIGN | DIV_ASSIGN
      | CONCAT_ASSIGN | MOD_ASSIGN | AND_ASSIGN | OR_ASSIGN
      | XOR_ASSIGN | SL_ASSIGN | SR_ASSIGN)
     assignmentExpressionCheckIfVariable
     assignmentExpression=assignmentExpression)
   | 0)
-> assignmentExpression ;;

--=& is special:
  -- $foo =& $var; is allowed but not $foo =& 'static';
  -- $foo =& new bar(); is allowed too but deprecated and reports a warning
  --we set a flag (varExpressionState) with that var_expression accepts only valid parts
  --this is done in such a strage way because we need the full expression to allow
  --things like $foo =& $bar || e();
ASSIGN
    assignmentExpressionCheckIfVariable --as in assignmentExpression
    (BIT_AND [: if (yytoken == Token_NEW) {
                reportProblem(Warning, "=& new foo() is deprecated");
                m_state.varExpressionState = OnlyNewObject;
              } else {
                m_state.varExpressionState = OnlyVariable;
              }:]
     | 0) assignmentExpression=assignmentExpression [: m_state.varExpressionState = Normal; :]
-> assignmentExpressionEqual ;;


-- check if var_expression was a variable, if not report an error
-- varExpressionIsVariable is set in var_expression
0 --to allow cpp-code
[:
    if (!m_state.varExpressionIsVariable) {
        reportProblem(Error, "Left side is not a variable");
        return false;
    }
:]
-> assignmentExpressionCheckIfVariable ;;

expression=booleanOrExpression
   (  QUESTION ifExpression=expr
      COLON    elseExpression=conditionalExpression
    | 0
   )
-> conditionalExpression ;;

  #expression=booleanAndExpression @ BOOLEAN_OR
-> booleanOrExpression ;;

   #expression=bitOrExpression @ BOOLEAN_AND
-> booleanAndExpression ;;

   #expression=bitXorExpression @ BIT_OR
-> bitOrExpression ;;

   #expression=bitAndExpression @ BIT_XOR
-> bitXorExpression ;;

   #expression=equalityExpression @ BIT_AND
-> bitAndExpression ;;

   expression=relationalExpression
   (#additionalExpression=equalityExpressionRest)*
-> equalityExpression ;;

   (  IS_EQUAL | IS_NOT_EQUAL | IS_IDENTICAL | IS_NOT_IDENTICAL )
   expression=relationalExpression
-> equalityExpressionRest ;;


   expression=shiftExpression
   (  (#additionalExpression=relationalExpressionRest)+
      --instanceof as in java.g (correct??)
    | INSTANCEOF instanceofType=classNameReference
    | 0
   )
-> relationalExpression ;;

   ( IS_SMALLER | IS_GREATER | IS_SMALLER_OR_EQUAL | IS_GREATER_OR_EQUAL )
   expression=shiftExpression
-> relationalExpressionRest ;;


   expression=additiveExpression
   (#additionalExpression=shiftExpressionRest)*
-> shiftExpression ;;

   ( SL | SR )
   expression=additiveExpression
-> shiftExpressionRest ;;


   expression=multiplicativeExpression
   (#additionalExpression=additiveExpressionRest)*
-> additiveExpression ;;

   ( PLUS | MINUS | CONCAT )
   expression=multiplicativeExpression
-> additiveExpressionRest ;;


   expression=unaryExpression
   (#additionalExpression=multiplicativeExpression_rest)*
-> multiplicativeExpression ;;

   ( MUL | DIV | MOD )
   expression=unaryExpression
-> multiplicativeExpression_rest ;;

 (
    MINUS unaryExpression=unaryExpression
  | PLUS  unaryExpression=unaryExpression
  | BANG unaryExpression=unaryExpression
  | TILDE unaryExpression=unaryExpression
  | INT_CAST unaryExpression=unaryExpression
  | DOUBLE_CAST unaryExpression=unaryExpression
  | STRING_CAST unaryExpression=unaryExpression
  | ARRAY_CAST unaryExpression=unaryExpression
  | OBJECT_CAST unaryExpression=unaryExpression
  | BOOL_CAST unaryExpression=unaryExpression
  | UNSET_CAST unaryExpression=unaryExpression
  | AT unaryExpression=unaryExpression
  | LIST LPAREN assignmentList=assignmentList RPAREN ASSIGN unaryExpression=unaryExpression
  | EXIT (LPAREN (expression=expr | 0) RPAREN | 0)
  | EVAL LPAREN expression=expr RPAREN
  | INCLUDE unaryExpression=unaryExpression
  | INCLUDE_ONCE unaryExpression=unaryExpression
  | REQUIRE unaryExpression=unaryExpression
  | REQUIRE_ONCE unaryExpression=unaryExpression

  | unaryExpressionNotPlusminus=unaryExpression_not_plusminus
 )
-> unaryExpression ;;

    (#prefixOperator=postprefixOperator)*
    varExpression=varExpression
    (#postfixOperator=postprefixOperator)*
-> unaryExpression_not_plusminus ;;

   op=INC | op=DEC
-> postprefixOperator ;;

--first/first conflict - no problem because of ifs
    ?[: m_state.varExpressionState == OnlyVariable :] 0 [: m_state.varExpressionState = Normal; :] variable=variable
  | ?[: m_state.varExpressionState == OnlyNewObject :] 0 [: m_state.varExpressionState = Normal; :] newObject=varExpressionNewObject
  | varExpressionNormal=varExpressionNormal
-> varExpression ;;

    LPAREN expression=expr RPAREN
  | BACKTICK encapsList=encapsList BACKTICK
  --try/rollback resolves conflict scalar vs. staticMember (foo::bar vs. foo::$bar)
  --varExpressionIsVariable flag is needed for assignmentExpression
  | try/rollback (variable=variable [: m_state.varExpressionIsVariable = true; :])
    catch (scalar=scalar)
  | ARRAY LPAREN
        (#arrayValues=arrayPairValue
             -- break because array(1,) is allowed (solves FIRST/FOLLOW conflict)
          @ (COMMA [: if (yytoken == Token_RPAREN) { break; } :] ) | 0)
    RPAREN
  | ISSET LPAREN (#issetVariable=variable @ COMMA) RPAREN
  | EMPTY LPAREN emptyVarialbe=variable RPAREN
  | newObject=varExpressionNewObject
  | CLONE cloneCar=varExpressionNormal
-> varExpressionNormal ;;

    NEW className=classNameReference ctor=ctorArguments
-> varExpressionNewObject ;;

    LPAREN parameterList=functionCallParameterList RPAREN
  | 0
-> ctorArguments ;;

    #parameters=functionCallParameterListElement @ COMMA | 0
-> functionCallParameterList ;;


    (BIT_AND variable=variable) | expr=expr
-> functionCallParameterListElement ;;

    #element=assignmentListElement @COMMA
-> assignmentList ;;

    variable=variable
  | LIST LPAREN assignmentList=assignmentList RPAREN
  | 0
-> assignmentListElement ;;

    expr=expr (DOUBLE_ARROW (exprValue=expr | BIT_AND varValue=variable) | 0)
  | BIT_AND variable=variable
-> arrayPairValue ;;

   var=baseVariableWithFunctionCalls (#variableProperties=variableProperty*)
-> variable ;;

    OBJECT_OPERATOR objectProperty=objectProperty
        (LPAREN parameterList=functionCallParameterList RPAREN | 0)
-> variableProperty ;;

   --Conflict
   --   foo::$bar[0] (=baseVariable-staticMember)
   --vs.foo::$bar[0](); (=static function call)
   try/rollback (functionCall=functionCall)
   catch (baseVariable=baseVariable)
-> baseVariableWithFunctionCalls ;;

    stringFunctionNameOrClass=STRING (
        LPAREN stringParameterList=functionCallParameterList RPAREN
      | PAAMAYIM_NEKUDOTAYIM
        (
            stringFunctionName=STRING LPAREN stringParameterList=functionCallParameterList RPAREN
            | varFunctionName=variableWithoutObjects LPAREN stringParameterList=functionCallParameterList RPAREN
        )
    )
  | varFunctionName=variableWithoutObjects LPAREN varParameterList=functionCallParameterList RPAREN
-> functionCall ;;

    var=compoundVariableWithSimpleIndirectReference #offsetItems=dimListItem*
  | staticMember=staticMember
-> baseVariable ;;

    variable=VARIABLE
  | DOLLAR LBRACE expr=expr RBRACE
-> compoundVariable ;;

  ( DOLLAR ( DOLLAR+ | 0 ) ( var=VARIABLE | LBRACE expr=expr RBRACE ) | var=VARIABLE )
-> compoundVariableWithSimpleIndirectReference ;;

    expr=expr | 0
-> dimOffset ;;

    className=STRING PAAMAYIM_NEKUDOTAYIM variable=variableWithoutObjects
-> staticMember ;;

    LBRACE statements=innerStatementList RBRACE
  | IF LPAREN ifExpr=expr RPAREN
      (   COLON statements=innerStatementList newElseifList newElseSingle ENDIF semicolonOrCloseTag
        | ifStatement=statement elseifList=elseifList elseSingle=elseSingle
      )
  | WHILE LPAREN whileExpr=expr RPAREN whilteStatement=whileStatement
  | FOR LPAREN forExpr1=forExpr SEMICOLON forExpr2=forExpr
    SEMICOLON forExpr3=forExpr RPAREN forStatement=forStatement
  | SWITCH LPAREN swtichExpr=expr RPAREN switchCaseList=switchCaseList

  | FOREACH LPAREN (
            -- allow $var as &$i and not expr() as &$i
        try/rollback(foreachVar=variable AS foreachVarAsVar=foreachVariable)
        catch(foreachExpr=expr AS foreachExprAsVar=variable))
        foreachArg=foreachOptionalArg RPAREN foreachStatement=foreachStatement
  | DECLARE LPAREN declareItem=declareItem @ COMMA RPAREN declareStatement
  | SEMICOLON     -- empty statement
  | TRY  LBRACE statements=innerStatementList RBRACE
    #catches=catch_item*
  | UNSET LPAREN #unsetVariables=variable @ COMMA RPAREN semicolonOrCloseTag
  | expr=expr semicolonOrCloseTag
  | DO doStatement=statement WHILE LPAREN whilteExpr=expr RPAREN semicolonOrCloseTag
  | BREAK (breakExpr=expr | 0) semicolonOrCloseTag
  | CONTINUE (continueExpr=expr | 0) semicolonOrCloseTag
  | RETURN (returnExpr=expr | 0) semicolonOrCloseTag
  | GLOBAL #globalVars=globalVar @ COMMA semicolonOrCloseTag
  | STATIC #staticVars=staticVar @ COMMA semicolonOrCloseTag
  | ECHO #echoExprs=expr @ COMMA semicolonOrCloseTag
  | THROW throwExpr=expr semicolonOrCloseTag
  -- throws error in zend parser, so ignored | USE use_filename  semicolonOrCloseTag

  | CLOSE_TAG
  | OPEN_TAG
  | OPEN_TAG_WITH_ECHO expr=expr semicolonOrCloseTag
  | INLINE_HTML

-> statement ;;

   SEMICOLON | CLOSE_TAG
-> semicolonOrCloseTag ;;

    LBRACE (SEMICOLON | 0) caseList=caseList RBRACE
  | COLON (SEMICOLON | 0) caseList=caseList ENDSWITCH semicolonOrCloseTag
-> switchCaseList ;;

    #caseItems=case_item*
-> caseList ;;

    CASE expr=expr (COLON | SEMICOLON) statements=innerStatementList
  | def=DEFAULT (COLON | SEMICOLON) statements=innerStatementList
-> case_item ;;

    CATCH LPAREN catchClass=STRING VARIABLE RPAREN
    LBRACE statements=innerStatementList RBRACE
-> catch_item ;;

    statement=statement
  | COLON statements=innerStatementList ENDDECLARE semicolonOrCloseTag
-> declareStatement ;;

    STRING ASSIGN scalar=staticScalar
-> declareItem ;;

    DOUBLE_ARROW foreachVariable=foreachVariable | 0
-> foreachOptionalArg ;;

    variable=variable | BIT_AND variable=variable
-> foreachVariable ;;

    statement=statement
  | COLON statements=innerStatementList ENDFOREACH semicolonOrCloseTag
-> foreachStatement ;;

  VARIABLE (ASSIGN staticScalar=staticScalar | 0)
-> staticVar ;;

    var=VARIABLE
  | DOLLAR (dollarVar=variable | LBRACE expr=expr RBRACE)
-> globalVar ;;

    #exprs=expr @ COMMA | 0
-> forExpr ;;

    statement=statement
  | COLON statements=innerStatementList ENDFOR semicolonOrCloseTag
-> forStatement ;;

    statement=statement
  | COLON statements=innerStatementList ENDWHILE semicolonOrCloseTag
-> whileStatement ;;

    --first/follow conflict; todo check if this is a problem
    #elseifListItem=elseifListItem*
-> elseifList ;;

    ELSEIF LPAREN expr=expr RPAREN statement=statement
-> elseifListItem ;;

    ELSE statement=statement | 0
-> elseSingle ;;

    #newElseifListItem=newelseifListItem*
-> newElseifList ;;

    ELSEIF LPAREN expr=expr RPAREN COLON statements=innerStatementList
-> newelseifListItem ;;

    ELSE COLON statements=innerStatementList | 0
-> newElseSingle ;;

    --resolve STRING vs. staticMember conflict
    ?[: LA(2).kind != Token_PAAMAYIM_NEKUDOTAYIM :]
    STRING
  | dynamicClassNameReference=dynamicClassNameReference
-> classNameReference ;;

    baseVariable=baseVariable (OBJECT_OPERATOR objectProperty=objectProperty
                          properties=dynamicClassNameVariableProperties | 0)
-> dynamicClassNameReference ;;

    #properties=dynamicClassNameVariableProperty*
-> dynamicClassNameVariableProperties ;;

    OBJECT_OPERATOR property=objectProperty
-> dynamicClassNameVariableProperty ;;

    objectDimList=objectDimList
  | variableWithoutObjects=variableWithoutObjects
-> objectProperty ;;

    variableName=variableName #offsetItems=dimListItem*
-> objectDimList ;;

  variable=compoundVariableWithSimpleIndirectReference #offsetItems=dimListItem*
-> variableWithoutObjects ;;

LBRACKET dimOffset=dimOffset RBRACKET | LBRACE expr=expr RBRACE
-> dimListItem ;;

    name=STRING
  | LBRACE expr=expr RBRACE
-> variableName ;;

    commonScalar=commonScalar
  | string=STRING (PAAMAYIM_NEKUDOTAYIM constname=STRING | 0)
  | varname=STRING_VARNAME
  | DOUBLE_QUOTE encapsList=encapsList DOUBLE_QUOTE
  | START_HEREDOC encapsList=encapsList END_HEREDOC
-> scalar ;;

    #encaps=encaps*
-> encapsList ;;

    var=encapsVar | value=ENCAPSED_AND_WHITESPACE
-> encaps ;;

     -- first/first conflict resolved by LA(2)
     --(expr allows STRING_VARNAME too - but without [expr])
    DOLLAR_OPEN_CURLY_BRACES ( ?[: LA(2).kind == Token_LBRACKET:] STRING_VARNAME LBRACKET expr=expr RBRACKET RBRACE
      | expr=expr RBRACE )
  | VARIABLE (OBJECT_OPERATOR STRING | LBRACKET offset=encapsVarOffset RBRACKET | 0)
  | CURLY_OPEN expr=expr RBRACE
-> encapsVar ;;

    STRING
  | NUM_STRING
  | VARIABLE
-> encapsVarOffset ;;


    LNUMBER [: (*yynode)->numType = Php::LongNumber;      :]
  | DNUMBER [: (*yynode)->numType = Php::DoubleNumber;      :]
  | CONSTANT_ENCAPSED_STRING
  | LINE
  | FILE
  | CLASS_C
  | METHOD_C
  | FUNC_C
-> commonScalar [
    member variable numType: Php::NumericType; ] ;;

    FUNCTION (BIT_AND | 0) STRING
    LPAREN params=parameterList RPAREN LBRACE statements=innerStatementList RBRACE
-> functionDeclarationStatement ;;

    #params=parameter @ COMMA | 0
-> parameterList ;;

(STRING | ARRAY | 0) (BIT_AND | 0) VARIABLE (ASSIGN defaultValue=staticScalar | 0)
-> parameter ;;

    value=commonScalar
  | STRING (PAAMAYIM_NEKUDOTAYIM STRING | 0)
  | PLUS plusValue=staticScalar
  | MINUS minusValue=staticScalar
  | ARRAY LPAREN
        (#arrayValues=staticArrayPairValue
             -- break because array(1,) is allowed
          @ (COMMA [: if (yytoken == Token_RPAREN) { break; } :] ) | 0)
    RPAREN
-> staticScalar ;;

    #val1=staticScalar (DOUBLE_ARROW #val2=staticScalar | 0)
-> staticArrayPairValue ;;

    (classType=ABSTRACT | classType=FINAL | 0) CLASS name=STRING
        (EXTENDS extends=STRING | 0)
        (IMPLEMENTS #implments=STRING @ COMMA | 0)
    LBRACE #statements=classStatement* RBRACE
  | INTERFACE name=STRING (EXTENDS #implments=STRING @ COMMA | 0)
    LBRACE #statements=classStatement* RBRACE
-> classDeclarationStatement ;;


    CONST consts=classConstantDeclaration @ COMMA SEMICOLON
  | VAR classVariableDeclaration SEMICOLON
  | modifiers=memberModifier*
    ( classVariableDeclaration SEMICOLON
      | FUNCTION (BIT_AND | 0) STRING LPAREN params=parameterList RPAREN
        body=methodBody
    )
-> classStatement ;;

    SEMICOLON -- abstract method
 |  LBRACE statements=innerStatementList RBRACE
-> methodBody ;;

    STRING ASSIGN scalar=staticScalar
-> classConstantDeclaration ;;

   vars=classVariable @ COMMA
-> classVariableDeclaration ;;

    var=VARIABLE (ASSIGN value=staticScalar | 0)
-> classVariable ;;

    PUBLIC
  | PROTECTED
  | PRIVATE
  | STATIC
  | ABSTRACT
  | FINAL
-> memberModifier ;;





-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:
#include "phplexer.h"
#include <QtCore/QDebug>

namespace Php
{

void Parser::tokenize( const QString& contents )
{
    m_contents = contents;
    Lexer lexer( this, contents );
    int kind = Parser::Token_EOF;

    do
    {
        kind = lexer.nextTokenKind();
        while (kind == Parser::Token_WHITESPACE || kind == Parser::Token_COMMENT || kind == Parser::Token_DOC_COMMENT) {
            kind = lexer.nextTokenKind();
        }
        if ( !kind ) // when the lexer returns 0, the end of file is reached
        {
            kind = Parser::Token_EOF;
        }
        Parser::Token &t = tokenStream->next();
        t.begin = lexer.tokenBegin();
        t.end = lexer.tokenEnd();
        t.kind = kind;
        //if ( m_debug ) qDebug() << kind << tokenText(t.begin,t.end) << t.begin << t.end;
    }
    while ( kind != Parser::Token_EOF );

    yylex(); // produce the look ahead token
}


QString Parser::tokenText(qint64 begin, qint64 end)
{
    return m_contents.mid(begin,end-begin+1);
}


void Parser::reportProblem( Parser::ProblemType type, const QString& message )
{
    if (type == Error)
        qDebug() << "** ERROR:" << message;
    else if (type == Warning)
        qDebug() << "** WARNING:" << message;
    else if (type == Info)
        qDebug() << "** Info:" << message;
}


// custom error recovery
void Parser::expectedToken(int /*expected*/, qint64 /*where*/, const QString& name)
{
    reportProblem( Parser::Error, QString("Expected token \"%1\"").arg(name));
}

void Parser::expectedSymbol(int /*expectedSymbol*/, const QString& name)
{
    qint64 line;
    qint64 col;
    qint64 index = tokenStream->index()-1;
    Token &token = tokenStream->token(index);
    kDebug() << "token starts at:" << token.begin;
    kDebug() << "index is:" << index;
    tokenStream->startPosition(index, &line, &col);
    QString tokenValue = tokenText(token.begin, token.end);
    reportProblem( Parser::Error,
                   QString("Expected symbol \"%1\" (current token: \"%2\" [%3] at line: %4 col: %5)")
                  .arg(name)
                  .arg(token.kind != 0 ? tokenValue : "EOF")
                  .arg(token.kind)
                  .arg(line)
                  .arg(col));
}

void Parser::setDebug( bool debug )
{
    m_debug = debug;
}

Parser::ParserState *Parser::copyCurrentState()
{
    ParserState *state = new ParserState();
    state->varExpressionState = m_state.varExpressionState;
    state->varExpressionIsVariable = m_state.varExpressionIsVariable;
    return state;
}

void Parser::restoreState( Parser::ParserState* state)
{
    m_state.varExpressionState = state->varExpressionState;
    m_state.varExpressionIsVariable = state->varExpressionIsVariable;
}

} // end of namespace Php

:]

-- kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
