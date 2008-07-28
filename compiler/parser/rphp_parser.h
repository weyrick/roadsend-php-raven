// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef rphp_H_INCLUDED
#define rphp_H_INCLUDED

#include "rphp_ast.h"
#include <kdev-pg-memory-pool.h>
#include <kdev-pg-allocator.h>
#include <kdev-pg-token-stream.h>

namespace rphp
  {

  class  parser
    {
    public:
      typedef kdev_pg_token_stream token_stream_type;
      typedef kdev_pg_token_stream::token_type token_type;
      kdev_pg_token_stream *token_stream;
      int yytoken;

      inline token_type LA(std::size_t k = 1) const
        {
          return token_stream->token(token_stream->index() - 1 + k - 1);
        }
      inline int yylex()
      {
        return (yytoken = token_stream->next_token());
      }
      inline void rewind(std::size_t index)
      {
        token_stream->rewind(index);
        yylex();
      }

      // token stream
      void set_token_stream(kdev_pg_token_stream *s)
      {
        token_stream = s;
      }

      // error handling
      void yy_expected_symbol(int kind, char const *name);
      void yy_expected_token(int kind, std::size_t token, char const *name);

      bool yy_block_errors;
      inline bool block_errors(bool block)
      {
        bool previous = yy_block_errors;
        yy_block_errors = block;
        return previous;
      }

      // memory pool
      typedef kdev_pg_memory_pool memory_pool_type;

      kdev_pg_memory_pool *memory_pool;
      void set_memory_pool(kdev_pg_memory_pool *p)
      {
        memory_pool = p;
      }
      template <class T>
      inline T *create()
      {
        T *node = new (memory_pool->allocate(sizeof(T))) T();
        node->kind = T::KIND;
        return node;
      }

      enum token_type_enum
      {
        Token_ABSTRACT = 1000,
        Token_AND_ASSIGN = 1001,
        Token_ARRAY = 1002,
        Token_ARRAY_CAST = 1003,
        Token_AS = 1004,
        Token_ASSIGN = 1005,
        Token_AT = 1006,
        Token_BACKTICK = 1007,
        Token_BANG = 1008,
        Token_BIT_AND = 1009,
        Token_BIT_OR = 1010,
        Token_BIT_XOR = 1011,
        Token_BOOLEAN_AND = 1012,
        Token_BOOLEAN_OR = 1013,
        Token_BOOL_CAST = 1014,
        Token_BREAK = 1015,
        Token_CASE = 1016,
        Token_CATCH = 1017,
        Token_CLASS = 1018,
        Token_CLASS_C = 1019,
        Token_CLONE = 1020,
        Token_CLOSE_TAG = 1021,
        Token_COLON = 1022,
        Token_COMMA = 1023,
        Token_COMMENT = 1024,
        Token_CONCAT = 1025,
        Token_CONCAT_ASSIGN = 1026,
        Token_CONST = 1027,
        Token_CONSTANT_ENCAPSED_STRING = 1028,
        Token_CONTINUE = 1029,
        Token_CURLY_OPEN = 1030,
        Token_DEC = 1031,
        Token_DECLARE = 1032,
        Token_DEFAULT = 1033,
        Token_DIV = 1034,
        Token_DIV_ASSIGN = 1035,
        Token_DNUMBER = 1036,
        Token_DO = 1037,
        Token_DOC_COMMENT = 1038,
        Token_DOLLAR = 1039,
        Token_DOLLAR_OPEN_CURLY_BRACES = 1040,
        Token_DOUBLE_ARROW = 1041,
        Token_DOUBLE_CAST = 1042,
        Token_DOUBLE_QUOTE = 1043,
        Token_ECHO = 1044,
        Token_ELSE = 1045,
        Token_ELSEIF = 1046,
        Token_EMPTY = 1047,
        Token_ENCAPSED_AND_WHITESPACE = 1048,
        Token_ENDDECLARE = 1049,
        Token_ENDFOR = 1050,
        Token_ENDFOREACH = 1051,
        Token_ENDIF = 1052,
        Token_ENDSWITCH = 1053,
        Token_ENDWHILE = 1054,
        Token_END_HEREDOC = 1055,
        Token_EOF = 1056,
        Token_EVAL = 1057,
        Token_EXIT = 1058,
        Token_EXTENDS = 1059,
        Token_FILE = 1060,
        Token_FINAL = 1061,
        Token_FOR = 1062,
        Token_FOREACH = 1063,
        Token_FUNCTION = 1064,
        Token_FUNC_C = 1065,
        Token_GLOBAL = 1066,
        Token_HALT_COMPILER = 1067,
        Token_IF = 1068,
        Token_IMPLEMENTS = 1069,
        Token_INC = 1070,
        Token_INCLUDE = 1071,
        Token_INCLUDE_ONCE = 1072,
        Token_INLINE_HTML = 1073,
        Token_INSTANCEOF = 1074,
        Token_INTERFACE = 1075,
        Token_INT_CAST = 1076,
        Token_INVALID = 1077,
        Token_ISSET = 1078,
        Token_IS_EQUAL = 1079,
        Token_IS_GREATER = 1080,
        Token_IS_GREATER_OR_EQUAL = 1081,
        Token_IS_IDENTICAL = 1082,
        Token_IS_NOT_EQUAL = 1083,
        Token_IS_NOT_IDENTICAL = 1084,
        Token_IS_SMALLER = 1085,
        Token_IS_SMALLER_OR_EQUAL = 1086,
        Token_LBRACE = 1087,
        Token_LBRACKET = 1088,
        Token_LINE = 1089,
        Token_LIST = 1090,
        Token_LNUMBER = 1091,
        Token_LOGICAL_AND = 1092,
        Token_LOGICAL_OR = 1093,
        Token_LOGICAL_XOR = 1094,
        Token_LPAREN = 1095,
        Token_METHOD_C = 1096,
        Token_MINUS = 1097,
        Token_MINUS_ASSIGN = 1098,
        Token_MOD = 1099,
        Token_MOD_ASSIGN = 1100,
        Token_MUL = 1101,
        Token_MUL_ASSIGN = 1102,
        Token_NEW = 1103,
        Token_NUM_STRING = 1104,
        Token_OBJECT_CAST = 1105,
        Token_OBJECT_OPERATOR = 1106,
        Token_OPEN_TAG = 1107,
        Token_OPEN_TAG_WITH_ECHO = 1108,
        Token_OR_ASSIGN = 1109,
        Token_PAAMAYIM_NEKUDOTAYIM = 1110,
        Token_PLUS = 1111,
        Token_PLUS_ASSIGN = 1112,
        Token_PRINT = 1113,
        Token_PRIVATE = 1114,
        Token_PROTECTED = 1115,
        Token_PUBLIC = 1116,
        Token_QUESTION = 1117,
        Token_RBRACE = 1118,
        Token_RBRACKET = 1119,
        Token_REQUIRE = 1120,
        Token_REQUIRE_ONCE = 1121,
        Token_RETURN = 1122,
        Token_RPAREN = 1123,
        Token_SEMICOLON = 1124,
        Token_SL = 1125,
        Token_SL_ASSIGN = 1126,
        Token_SR = 1127,
        Token_SR_ASSIGN = 1128,
        Token_START_HEREDOC = 1129,
        Token_STATIC = 1130,
        Token_STRING = 1131,
        Token_STRING_CAST = 1132,
        Token_STRING_VARNAME = 1133,
        Token_SWITCH = 1134,
        Token_THROW = 1135,
        Token_TILDE = 1136,
        Token_TRY = 1137,
        Token_UNSET = 1138,
        Token_UNSET_CAST = 1139,
        Token_USE = 1140,
        Token_VAR = 1141,
        Token_VARIABLE = 1142,
        Token_WHILE = 1143,
        Token_WHITESPACE = 1144,
        Token_XOR_ASSIGN = 1145,
        token_type_size
      }; // token_type_enum

      // user defined declarations:
    public:

      /**
       * Transform the raw input into tokens.
       * When this method returns, the parser's token stream has been filled
       * and any parse*() method can be called.
       */
      void tokenize( const UnicodeString& contents );

      enum ProblemType {
        Error,
        Warning,
        Info
      };
      void reportProblem( parser::ProblemType type, const std::string& message );
      UnicodeString tokenText(rint64 begin, rint64 end);
      void setDebug(bool debug);


    private:

      enum VarExpressionState {
        Normal,
        OnlyVariable,
        OnlyNewObject
      };
      UnicodeString m_contents;
      bool m_debug;

      struct parser_state
        {
          VarExpressionState varExpressionState;
          bool varExpressionIsVariable;
        };
      parser_state m_state;


    public:
      // The copy_current_state() and restore_state() methods are only declared
      // if you are using try blocks in your grammar, and have to be
      // implemented by yourself, and you also have to define a
      // "struct parser_state" inside a %parserclass directive.

      // This method should create a new parser_state object and return it,
      // or return 0 if no state variables need to be saved.
      parser_state *copy_current_state();

      // This method is only called for parser_state objects != 0
      // and should restore the parser state given as argument.
      void restore_state(parser_state *state);
      parser()
      {
        memory_pool = 0;
        token_stream = 0;
        yytoken = Token_EOF;
        yy_block_errors = false;

        // user defined constructor code:

        m_state.varExpressionState = Normal;
        m_state.varExpressionIsVariable = false;

      }

      virtual ~parser()
      {}

      bool parse_additiveExpression(additiveExpression_ast **yynode);
      bool parse_additiveExpressionRest(additiveExpressionRest_ast **yynode);
      bool parse_arrayPairValue(arrayPairValue_ast **yynode);
      bool parse_assignmentExpression(assignmentExpression_ast **yynode);
      bool parse_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast **yynode);
      bool parse_assignmentExpressionEqual(assignmentExpressionEqual_ast **yynode);
      bool parse_assignmentList(assignmentList_ast **yynode);
      bool parse_assignmentListElement(assignmentListElement_ast **yynode);
      bool parse_baseVariable(baseVariable_ast **yynode);
      bool parse_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast **yynode);
      bool parse_bitAndExpression(bitAndExpression_ast **yynode);
      bool parse_bitOrExpression(bitOrExpression_ast **yynode);
      bool parse_bitXorExpression(bitXorExpression_ast **yynode);
      bool parse_booleanAndExpression(booleanAndExpression_ast **yynode);
      bool parse_booleanOrExpression(booleanOrExpression_ast **yynode);
      bool parse_caseList(caseList_ast **yynode);
      bool parse_case_item(case_item_ast **yynode);
      bool parse_catch_item(catch_item_ast **yynode);
      bool parse_classConstantDeclaration(classConstantDeclaration_ast **yynode);
      bool parse_classDeclarationStatement(classDeclarationStatement_ast **yynode);
      bool parse_classNameReference(classNameReference_ast **yynode);
      bool parse_classStatement(classStatement_ast **yynode);
      bool parse_classVariable(classVariable_ast **yynode);
      bool parse_classVariableDeclaration(classVariableDeclaration_ast **yynode);
      bool parse_commonScalar(commonScalar_ast **yynode);
      bool parse_compoundVariable(compoundVariable_ast **yynode);
      bool parse_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast **yynode);
      bool parse_conditionalExpression(conditionalExpression_ast **yynode);
      bool parse_ctorArguments(ctorArguments_ast **yynode);
      bool parse_declareItem(declareItem_ast **yynode);
      bool parse_declareStatement(declareStatement_ast **yynode);
      bool parse_dimListItem(dimListItem_ast **yynode);
      bool parse_dimOffset(dimOffset_ast **yynode);
      bool parse_dynamicClassNameReference(dynamicClassNameReference_ast **yynode);
      bool parse_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast **yynode);
      bool parse_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast **yynode);
      bool parse_elseSingle(elseSingle_ast **yynode);
      bool parse_elseifList(elseifList_ast **yynode);
      bool parse_elseifListItem(elseifListItem_ast **yynode);
      bool parse_encaps(encaps_ast **yynode);
      bool parse_encapsList(encapsList_ast **yynode);
      bool parse_encapsVar(encapsVar_ast **yynode);
      bool parse_encapsVarOffset(encapsVarOffset_ast **yynode);
      bool parse_equalityExpression(equalityExpression_ast **yynode);
      bool parse_equalityExpressionRest(equalityExpressionRest_ast **yynode);
      bool parse_expr(expr_ast **yynode);
      bool parse_forExpr(forExpr_ast **yynode);
      bool parse_forStatement(forStatement_ast **yynode);
      bool parse_foreachOptionalArg(foreachOptionalArg_ast **yynode);
      bool parse_foreachStatement(foreachStatement_ast **yynode);
      bool parse_foreachVariable(foreachVariable_ast **yynode);
      bool parse_functionCall(functionCall_ast **yynode);
      bool parse_functionCallParameterList(functionCallParameterList_ast **yynode);
      bool parse_functionCallParameterListElement(functionCallParameterListElement_ast **yynode);
      bool parse_functionDeclarationStatement(functionDeclarationStatement_ast **yynode);
      bool parse_globalVar(globalVar_ast **yynode);
      bool parse_innerStatementList(innerStatementList_ast **yynode);
      bool parse_logicalAndExpression(logicalAndExpression_ast **yynode);
      bool parse_logicalOrExpression(logicalOrExpression_ast **yynode);
      bool parse_logicalXorExpression(logicalXorExpression_ast **yynode);
      bool parse_memberModifier(memberModifier_ast **yynode);
      bool parse_methodBody(methodBody_ast **yynode);
      bool parse_multiplicativeExpression(multiplicativeExpression_ast **yynode);
      bool parse_multiplicativeExpression_rest(multiplicativeExpression_rest_ast **yynode);
      bool parse_newElseSingle(newElseSingle_ast **yynode);
      bool parse_newElseifList(newElseifList_ast **yynode);
      bool parse_newelseifListItem(newelseifListItem_ast **yynode);
      bool parse_objectDimList(objectDimList_ast **yynode);
      bool parse_objectProperty(objectProperty_ast **yynode);
      bool parse_parameter(parameter_ast **yynode);
      bool parse_parameterList(parameterList_ast **yynode);
      bool parse_postprefixOperator(postprefixOperator_ast **yynode);
      bool parse_printExpression(printExpression_ast **yynode);
      bool parse_relationalExpression(relationalExpression_ast **yynode);
      bool parse_relationalExpressionRest(relationalExpressionRest_ast **yynode);
      bool parse_scalar(scalar_ast **yynode);
      bool parse_semicolonOrCloseTag(semicolonOrCloseTag_ast **yynode);
      bool parse_shiftExpression(shiftExpression_ast **yynode);
      bool parse_shiftExpressionRest(shiftExpressionRest_ast **yynode);
      bool parse_start(start_ast **yynode);
      bool parse_statement(statement_ast **yynode);
      bool parse_staticArrayPairValue(staticArrayPairValue_ast **yynode);
      bool parse_staticMember(staticMember_ast **yynode);
      bool parse_staticScalar(staticScalar_ast **yynode);
      bool parse_staticVar(staticVar_ast **yynode);
      bool parse_switchCaseList(switchCaseList_ast **yynode);
      bool parse_topStatement(topStatement_ast **yynode);
      bool parse_unaryExpression(unaryExpression_ast **yynode);
      bool parse_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast **yynode);
      bool parse_varExpression(varExpression_ast **yynode);
      bool parse_varExpressionNewObject(varExpressionNewObject_ast **yynode);
      bool parse_varExpressionNormal(varExpressionNormal_ast **yynode);
      bool parse_variable(variable_ast **yynode);
      bool parse_variableName(variableName_ast **yynode);
      bool parse_variableProperty(variableProperty_ast **yynode);
      bool parse_variableWithoutObjects(variableWithoutObjects_ast **yynode);
      bool parse_whileStatement(whileStatement_ast **yynode);
    };

} // end of namespace rphp

#endif


