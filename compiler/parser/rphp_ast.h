// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef rphp_AST_H_INCLUDED
#define rphp_AST_H_INCLUDED

#include <kdev-pg-list.h>



#include <string>
#include <iostream>

namespace rphp
  {

  class Lexer;
  enum NumericType  {
    LongNumber,
    DoubleNumber,
  };

  // TODO define win macro
#ifdef WIN
  typedef __int64 rint64;
#else
  typedef long long rint64;
#endif

}

namespace rphp
  {

  struct additiveExpression_ast;

  struct additiveExpressionRest_ast;

  struct arrayPairValue_ast;

  struct assignmentExpression_ast;

  struct assignmentExpressionCheckIfVariable_ast;

  struct assignmentExpressionEqual_ast;

  struct assignmentList_ast;

  struct assignmentListElement_ast;

  struct baseVariable_ast;

  struct baseVariableWithFunctionCalls_ast;

  struct bitAndExpression_ast;

  struct bitOrExpression_ast;

  struct bitXorExpression_ast;

  struct booleanAndExpression_ast;

  struct booleanOrExpression_ast;

  struct caseList_ast;

  struct case_item_ast;

  struct catch_item_ast;

  struct classConstantDeclaration_ast;

  struct classDeclarationStatement_ast;

  struct classNameReference_ast;

  struct classStatement_ast;

  struct classVariable_ast;

  struct classVariableDeclaration_ast;

  struct commonScalar_ast;

  struct compoundVariable_ast;

  struct compoundVariableWithSimpleIndirectReference_ast;

  struct conditionalExpression_ast;

  struct ctorArguments_ast;

  struct declareItem_ast;

  struct declareStatement_ast;

  struct dimListItem_ast;

  struct dimOffset_ast;

  struct dynamicClassNameReference_ast;

  struct dynamicClassNameVariableProperties_ast;

  struct dynamicClassNameVariableProperty_ast;

  struct elseSingle_ast;

  struct elseifList_ast;

  struct elseifListItem_ast;

  struct encaps_ast;

  struct encapsList_ast;

  struct encapsVar_ast;

  struct encapsVarOffset_ast;

  struct equalityExpression_ast;

  struct equalityExpressionRest_ast;

  struct expr_ast;

  struct forExpr_ast;

  struct forStatement_ast;

  struct foreachOptionalArg_ast;

  struct foreachStatement_ast;

  struct foreachVariable_ast;

  struct functionCall_ast;

  struct functionCallParameterList_ast;

  struct functionCallParameterListElement_ast;

  struct functionDeclarationStatement_ast;

  struct globalVar_ast;

  struct innerStatementList_ast;

  struct logicalAndExpression_ast;

  struct logicalOrExpression_ast;

  struct logicalXorExpression_ast;

  struct memberModifier_ast;

  struct methodBody_ast;

  struct multiplicativeExpression_ast;

  struct multiplicativeExpression_rest_ast;

  struct newElseSingle_ast;

  struct newElseifList_ast;

  struct newelseifListItem_ast;

  struct objectDimList_ast;

  struct objectProperty_ast;

  struct parameter_ast;

  struct parameterList_ast;

  struct postprefixOperator_ast;

  struct printExpression_ast;

  struct relationalExpression_ast;

  struct relationalExpressionRest_ast;

  struct scalar_ast;

  struct semicolonOrCloseTag_ast;

  struct shiftExpression_ast;

  struct shiftExpressionRest_ast;

  struct start_ast;

  struct statement_ast;

  struct staticArrayPairValue_ast;

  struct staticMember_ast;

  struct staticScalar_ast;

  struct staticVar_ast;

  struct switchCaseList_ast;

  struct topStatement_ast;

  struct unaryExpression_ast;

  struct unaryExpression_not_plusminus_ast;

  struct varExpression_ast;

  struct varExpressionNewObject_ast;

  struct varExpressionNormal_ast;

  struct variable_ast;

  struct variableName_ast;

  struct variableProperty_ast;

  struct variableWithoutObjects_ast;

  struct whileStatement_ast;


  struct  ast_node
    {
      enum ast_node_kind_enum {
        Kind_additiveExpression = 1000,
        Kind_additiveExpressionRest = 1001,
        Kind_arrayPairValue = 1002,
        Kind_assignmentExpression = 1003,
        Kind_assignmentExpressionCheckIfVariable = 1004,
        Kind_assignmentExpressionEqual = 1005,
        Kind_assignmentList = 1006,
        Kind_assignmentListElement = 1007,
        Kind_baseVariable = 1008,
        Kind_baseVariableWithFunctionCalls = 1009,
        Kind_bitAndExpression = 1010,
        Kind_bitOrExpression = 1011,
        Kind_bitXorExpression = 1012,
        Kind_booleanAndExpression = 1013,
        Kind_booleanOrExpression = 1014,
        Kind_caseList = 1015,
        Kind_case_item = 1016,
        Kind_catch_item = 1017,
        Kind_classConstantDeclaration = 1018,
        Kind_classDeclarationStatement = 1019,
        Kind_classNameReference = 1020,
        Kind_classStatement = 1021,
        Kind_classVariable = 1022,
        Kind_classVariableDeclaration = 1023,
        Kind_commonScalar = 1024,
        Kind_compoundVariable = 1025,
        Kind_compoundVariableWithSimpleIndirectReference = 1026,
        Kind_conditionalExpression = 1027,
        Kind_ctorArguments = 1028,
        Kind_declareItem = 1029,
        Kind_declareStatement = 1030,
        Kind_dimListItem = 1031,
        Kind_dimOffset = 1032,
        Kind_dynamicClassNameReference = 1033,
        Kind_dynamicClassNameVariableProperties = 1034,
        Kind_dynamicClassNameVariableProperty = 1035,
        Kind_elseSingle = 1036,
        Kind_elseifList = 1037,
        Kind_elseifListItem = 1038,
        Kind_encaps = 1039,
        Kind_encapsList = 1040,
        Kind_encapsVar = 1041,
        Kind_encapsVarOffset = 1042,
        Kind_equalityExpression = 1043,
        Kind_equalityExpressionRest = 1044,
        Kind_expr = 1045,
        Kind_forExpr = 1046,
        Kind_forStatement = 1047,
        Kind_foreachOptionalArg = 1048,
        Kind_foreachStatement = 1049,
        Kind_foreachVariable = 1050,
        Kind_functionCall = 1051,
        Kind_functionCallParameterList = 1052,
        Kind_functionCallParameterListElement = 1053,
        Kind_functionDeclarationStatement = 1054,
        Kind_globalVar = 1055,
        Kind_innerStatementList = 1056,
        Kind_logicalAndExpression = 1057,
        Kind_logicalOrExpression = 1058,
        Kind_logicalXorExpression = 1059,
        Kind_memberModifier = 1060,
        Kind_methodBody = 1061,
        Kind_multiplicativeExpression = 1062,
        Kind_multiplicativeExpression_rest = 1063,
        Kind_newElseSingle = 1064,
        Kind_newElseifList = 1065,
        Kind_newelseifListItem = 1066,
        Kind_objectDimList = 1067,
        Kind_objectProperty = 1068,
        Kind_parameter = 1069,
        Kind_parameterList = 1070,
        Kind_postprefixOperator = 1071,
        Kind_printExpression = 1072,
        Kind_relationalExpression = 1073,
        Kind_relationalExpressionRest = 1074,
        Kind_scalar = 1075,
        Kind_semicolonOrCloseTag = 1076,
        Kind_shiftExpression = 1077,
        Kind_shiftExpressionRest = 1078,
        Kind_start = 1079,
        Kind_statement = 1080,
        Kind_staticArrayPairValue = 1081,
        Kind_staticMember = 1082,
        Kind_staticScalar = 1083,
        Kind_staticVar = 1084,
        Kind_switchCaseList = 1085,
        Kind_topStatement = 1086,
        Kind_unaryExpression = 1087,
        Kind_unaryExpression_not_plusminus = 1088,
        Kind_varExpression = 1089,
        Kind_varExpressionNewObject = 1090,
        Kind_varExpressionNormal = 1091,
        Kind_variable = 1092,
        Kind_variableName = 1093,
        Kind_variableProperty = 1094,
        Kind_variableWithoutObjects = 1095,
        Kind_whileStatement = 1096,
        AST_NODE_KIND_COUNT
      };

      int kind;
      std::size_t start_token;
      std::size_t end_token;
    };

  struct  additiveExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_additiveExpression
      };

      multiplicativeExpression_ast *expression;
      const list_node<additiveExpressionRest_ast *> *additionalExpression_sequence;
    };

  struct  additiveExpressionRest_ast: public ast_node
    {
      enum
      {
        KIND = Kind_additiveExpressionRest
      };

      multiplicativeExpression_ast *expression;
    };

  struct  arrayPairValue_ast: public ast_node
    {
      enum
      {
        KIND = Kind_arrayPairValue
      };

      expr_ast *expr;
      expr_ast *exprValue;
      variable_ast *varValue;
      variable_ast *variable;
    };

  struct  assignmentExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignmentExpression
      };

      conditionalExpression_ast *conditionalExpression;
      assignmentExpressionEqual_ast *assignmentAxpressionEqual;
      assignmentExpression_ast *assignmentExpression;
    };

  struct  assignmentExpressionCheckIfVariable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignmentExpressionCheckIfVariable
      };

    };

  struct  assignmentExpressionEqual_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignmentExpressionEqual
      };

      assignmentExpression_ast *assignmentExpression;
    };

  struct  assignmentList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignmentList
      };

      const list_node<assignmentListElement_ast *> *element_sequence;
    };

  struct  assignmentListElement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignmentListElement
      };

      variable_ast *variable;
      assignmentList_ast *assignmentList;
    };

  struct  baseVariable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_baseVariable
      };

      compoundVariableWithSimpleIndirectReference_ast *var;
      const list_node<dimListItem_ast *> *offsetItems_sequence;
      staticMember_ast *staticMember;
    };

  struct  baseVariableWithFunctionCalls_ast: public ast_node
    {
      enum
      {
        KIND = Kind_baseVariableWithFunctionCalls
      };

      functionCall_ast *functionCall;
      baseVariable_ast *baseVariable;
    };

  struct  bitAndExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_bitAndExpression
      };

      const list_node<equalityExpression_ast *> *expression_sequence;
    };

  struct  bitOrExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_bitOrExpression
      };

      const list_node<bitXorExpression_ast *> *expression_sequence;
    };

  struct  bitXorExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_bitXorExpression
      };

      const list_node<bitAndExpression_ast *> *expression_sequence;
    };

  struct  booleanAndExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_booleanAndExpression
      };

      const list_node<bitOrExpression_ast *> *expression_sequence;
    };

  struct  booleanOrExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_booleanOrExpression
      };

      const list_node<booleanAndExpression_ast *> *expression_sequence;
    };

  struct  caseList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_caseList
      };

      const list_node<case_item_ast *> *caseItems_sequence;
    };

  struct  case_item_ast: public ast_node
    {
      enum
      {
        KIND = Kind_case_item
      };

      expr_ast *expr;
      innerStatementList_ast *statements;
      std::size_t def;
    };

  struct  catch_item_ast: public ast_node
    {
      enum
      {
        KIND = Kind_catch_item
      };

      std::size_t catchClass;
      innerStatementList_ast *statements;
    };

  struct  classConstantDeclaration_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classConstantDeclaration
      };

      staticScalar_ast *scalar;
    };

  struct  classDeclarationStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classDeclarationStatement
      };

      std::size_t classType;
      std::size_t name;
      std::size_t extends;
      const list_node<std::size_t > *implments_sequence;
      const list_node<classStatement_ast *> *statements_sequence;
    };

  struct  classNameReference_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classNameReference
      };

      dynamicClassNameReference_ast *dynamicClassNameReference;
    };

  struct  classStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classStatement
      };

      classConstantDeclaration_ast *consts;
      memberModifier_ast *modifiers;
      parameterList_ast *params;
      methodBody_ast *body;
    };

  struct  classVariable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classVariable
      };

      std::size_t var;
      staticScalar_ast *value;
    };

  struct  classVariableDeclaration_ast: public ast_node
    {
      enum
      {
        KIND = Kind_classVariableDeclaration
      };

      classVariable_ast *vars;
    };

  struct  commonScalar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_commonScalar
      };

      rphp::NumericType numType;
    };

  struct  compoundVariable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_compoundVariable
      };

      std::size_t variable;
      expr_ast *expr;
    };

  struct  compoundVariableWithSimpleIndirectReference_ast: public ast_node
    {
      enum
      {
        KIND = Kind_compoundVariableWithSimpleIndirectReference
      };

      std::size_t var;
      expr_ast *expr;
    };

  struct  conditionalExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_conditionalExpression
      };

      booleanOrExpression_ast *expression;
      expr_ast *ifExpression;
      conditionalExpression_ast *elseExpression;
    };

  struct  ctorArguments_ast: public ast_node
    {
      enum
      {
        KIND = Kind_ctorArguments
      };

      functionCallParameterList_ast *parameterList;
    };

  struct  declareItem_ast: public ast_node
    {
      enum
      {
        KIND = Kind_declareItem
      };

      staticScalar_ast *scalar;
    };

  struct  declareStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_declareStatement
      };

      statement_ast *statement;
      innerStatementList_ast *statements;
    };

  struct  dimListItem_ast: public ast_node
    {
      enum
      {
        KIND = Kind_dimListItem
      };

      dimOffset_ast *dimOffset;
      expr_ast *expr;
    };

  struct  dimOffset_ast: public ast_node
    {
      enum
      {
        KIND = Kind_dimOffset
      };

      expr_ast *expr;
    };

  struct  dynamicClassNameReference_ast: public ast_node
    {
      enum
      {
        KIND = Kind_dynamicClassNameReference
      };

      baseVariable_ast *baseVariable;
      objectProperty_ast *objectProperty;
      dynamicClassNameVariableProperties_ast *properties;
    };

  struct  dynamicClassNameVariableProperties_ast: public ast_node
    {
      enum
      {
        KIND = Kind_dynamicClassNameVariableProperties
      };

      const list_node<dynamicClassNameVariableProperty_ast *> *properties_sequence;
    };

  struct  dynamicClassNameVariableProperty_ast: public ast_node
    {
      enum
      {
        KIND = Kind_dynamicClassNameVariableProperty
      };

      objectProperty_ast *property;
    };

  struct  elseSingle_ast: public ast_node
    {
      enum
      {
        KIND = Kind_elseSingle
      };

      statement_ast *statement;
    };

  struct  elseifList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_elseifList
      };

      const list_node<elseifListItem_ast *> *elseifListItem_sequence;
    };

  struct  elseifListItem_ast: public ast_node
    {
      enum
      {
        KIND = Kind_elseifListItem
      };

      expr_ast *expr;
      statement_ast *statement;
    };

  struct  encaps_ast: public ast_node
    {
      enum
      {
        KIND = Kind_encaps
      };

      encapsVar_ast *var;
      std::size_t value;
    };

  struct  encapsList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_encapsList
      };

      const list_node<encaps_ast *> *encaps_sequence;
    };

  struct  encapsVar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_encapsVar
      };

      expr_ast *expr;
      encapsVarOffset_ast *offset;
    };

  struct  encapsVarOffset_ast: public ast_node
    {
      enum
      {
        KIND = Kind_encapsVarOffset
      };

    };

  struct  equalityExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_equalityExpression
      };

      relationalExpression_ast *expression;
      const list_node<equalityExpressionRest_ast *> *additionalExpression_sequence;
    };

  struct  equalityExpressionRest_ast: public ast_node
    {
      enum
      {
        KIND = Kind_equalityExpressionRest
      };

      relationalExpression_ast *expression;
    };

  struct  expr_ast: public ast_node
    {
      enum
      {
        KIND = Kind_expr
      };

      logicalOrExpression_ast *expression;
    };

  struct  forExpr_ast: public ast_node
    {
      enum
      {
        KIND = Kind_forExpr
      };

      const list_node<expr_ast *> *exprs_sequence;
    };

  struct  forStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_forStatement
      };

      statement_ast *statement;
      innerStatementList_ast *statements;
    };

  struct  foreachOptionalArg_ast: public ast_node
    {
      enum
      {
        KIND = Kind_foreachOptionalArg
      };

      foreachVariable_ast *foreachVariable;
    };

  struct  foreachStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_foreachStatement
      };

      statement_ast *statement;
      innerStatementList_ast *statements;
    };

  struct  foreachVariable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_foreachVariable
      };

      variable_ast *variable;
    };

  struct  functionCall_ast: public ast_node
    {
      enum
      {
        KIND = Kind_functionCall
      };

      std::size_t stringFunctionNameOrClass;
      functionCallParameterList_ast *stringParameterList;
      std::size_t stringFunctionName;
      variableWithoutObjects_ast *varFunctionName;
      functionCallParameterList_ast *varParameterList;
    };

  struct  functionCallParameterList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_functionCallParameterList
      };

      const list_node<functionCallParameterListElement_ast *> *parameters_sequence;
    };

  struct  functionCallParameterListElement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_functionCallParameterListElement
      };

      variable_ast *variable;
      expr_ast *expr;
    };

  struct  functionDeclarationStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_functionDeclarationStatement
      };

      parameterList_ast *params;
      innerStatementList_ast *statements;
    };

  struct  globalVar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_globalVar
      };

      std::size_t var;
      variable_ast *dollarVar;
      expr_ast *expr;
    };

  struct  innerStatementList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_innerStatementList
      };

      const list_node<topStatement_ast *> *statements_sequence;
    };

  struct  logicalAndExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_logicalAndExpression
      };

      const list_node<printExpression_ast *> *expression_sequence;
    };

  struct  logicalOrExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_logicalOrExpression
      };

      const list_node<logicalXorExpression_ast *> *expression_sequence;
    };

  struct  logicalXorExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_logicalXorExpression
      };

      const list_node<logicalAndExpression_ast *> *expression_sequence;
    };

  struct  memberModifier_ast: public ast_node
    {
      enum
      {
        KIND = Kind_memberModifier
      };

    };

  struct  methodBody_ast: public ast_node
    {
      enum
      {
        KIND = Kind_methodBody
      };

      innerStatementList_ast *statements;
    };

  struct  multiplicativeExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_multiplicativeExpression
      };

      unaryExpression_ast *expression;
      const list_node<multiplicativeExpression_rest_ast *> *additionalExpression_sequence;
    };

  struct  multiplicativeExpression_rest_ast: public ast_node
    {
      enum
      {
        KIND = Kind_multiplicativeExpression_rest
      };

      unaryExpression_ast *expression;
    };

  struct  newElseSingle_ast: public ast_node
    {
      enum
      {
        KIND = Kind_newElseSingle
      };

      innerStatementList_ast *statements;
    };

  struct  newElseifList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_newElseifList
      };

      const list_node<newelseifListItem_ast *> *newElseifListItem_sequence;
    };

  struct  newelseifListItem_ast: public ast_node
    {
      enum
      {
        KIND = Kind_newelseifListItem
      };

      expr_ast *expr;
      innerStatementList_ast *statements;
    };

  struct  objectDimList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_objectDimList
      };

      variableName_ast *variableName;
      const list_node<dimListItem_ast *> *offsetItems_sequence;
    };

  struct  objectProperty_ast: public ast_node
    {
      enum
      {
        KIND = Kind_objectProperty
      };

      objectDimList_ast *objectDimList;
      variableWithoutObjects_ast *variableWithoutObjects;
    };

  struct  parameter_ast: public ast_node
    {
      enum
      {
        KIND = Kind_parameter
      };

      staticScalar_ast *defaultValue;
    };

  struct  parameterList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_parameterList
      };

      const list_node<parameter_ast *> *params_sequence;
    };

  struct  postprefixOperator_ast: public ast_node
    {
      enum
      {
        KIND = Kind_postprefixOperator
      };

      std::size_t op;
    };

  struct  printExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_printExpression
      };

      std::size_t print;
      assignmentExpression_ast *expression;
    };

  struct  relationalExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_relationalExpression
      };

      shiftExpression_ast *expression;
      const list_node<relationalExpressionRest_ast *> *additionalExpression_sequence;
      classNameReference_ast *instanceofType;
    };

  struct  relationalExpressionRest_ast: public ast_node
    {
      enum
      {
        KIND = Kind_relationalExpressionRest
      };

      shiftExpression_ast *expression;
    };

  struct  scalar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_scalar
      };

      commonScalar_ast *commonScalar;
      std::size_t string;
      std::size_t constname;
      std::size_t varname;
      encapsList_ast *encapsList;
    };

  struct  semicolonOrCloseTag_ast: public ast_node
    {
      enum
      {
        KIND = Kind_semicolonOrCloseTag
      };

    };

  struct  shiftExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_shiftExpression
      };

      additiveExpression_ast *expression;
      const list_node<shiftExpressionRest_ast *> *additionalExpression_sequence;
    };

  struct  shiftExpressionRest_ast: public ast_node
    {
      enum
      {
        KIND = Kind_shiftExpressionRest
      };

      additiveExpression_ast *expression;
    };

  struct  start_ast: public ast_node
    {
      enum
      {
        KIND = Kind_start
      };

      innerStatementList_ast *statements;
    };

  struct  statement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_statement
      };

      innerStatementList_ast *statements;
      expr_ast *ifExpr;
      statement_ast *ifStatement;
      elseifList_ast *elseifList;
      elseSingle_ast *elseSingle;
      expr_ast *whileExpr;
      whileStatement_ast *whilteStatement;
      forExpr_ast *forExpr1;
      forExpr_ast *forExpr2;
      forExpr_ast *forExpr3;
      forStatement_ast *forStatement;
      expr_ast *swtichExpr;
      switchCaseList_ast *switchCaseList;
      variable_ast *foreachVar;
      foreachVariable_ast *foreachVarAsVar;
      expr_ast *foreachExpr;
      variable_ast *foreachExprAsVar;
      foreachOptionalArg_ast *foreachArg;
      foreachStatement_ast *foreachStatement;
      declareItem_ast *declareItem;
      const list_node<catch_item_ast *> *catches_sequence;
      const list_node<variable_ast *> *unsetVariables_sequence;
      expr_ast *expr;
      statement_ast *doStatement;
      expr_ast *whilteExpr;
      expr_ast *breakExpr;
      expr_ast *continueExpr;
      expr_ast *returnExpr;
      const list_node<globalVar_ast *> *globalVars_sequence;
      const list_node<staticVar_ast *> *staticVars_sequence;
      const list_node<expr_ast *> *echoExprs_sequence;
      expr_ast *throwExpr;
    };

  struct  staticArrayPairValue_ast: public ast_node
    {
      enum
      {
        KIND = Kind_staticArrayPairValue
      };

      const list_node<staticScalar_ast *> *val1_sequence;
      const list_node<staticScalar_ast *> *val2_sequence;
    };

  struct  staticMember_ast: public ast_node
    {
      enum
      {
        KIND = Kind_staticMember
      };

      std::size_t className;
      variableWithoutObjects_ast *variable;
    };

  struct  staticScalar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_staticScalar
      };

      commonScalar_ast *value;
      staticScalar_ast *plusValue;
      staticScalar_ast *minusValue;
      const list_node<staticArrayPairValue_ast *> *arrayValues_sequence;
    };

  struct  staticVar_ast: public ast_node
    {
      enum
      {
        KIND = Kind_staticVar
      };

      staticScalar_ast *staticScalar;
    };

  struct  switchCaseList_ast: public ast_node
    {
      enum
      {
        KIND = Kind_switchCaseList
      };

      caseList_ast *caseList;
    };

  struct  topStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_topStatement
      };

      statement_ast *statement;
      functionDeclarationStatement_ast *functionDeclaration;
      classDeclarationStatement_ast *classDeclaration;
    };

  struct  unaryExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_unaryExpression
      };

      unaryExpression_ast *unaryExpression;
      assignmentList_ast *assignmentList;
      expr_ast *expression;
      unaryExpression_not_plusminus_ast *unaryExpressionNotPlusminus;
    };

  struct  unaryExpression_not_plusminus_ast: public ast_node
    {
      enum
      {
        KIND = Kind_unaryExpression_not_plusminus
      };

      const list_node<postprefixOperator_ast *> *prefixOperator_sequence;
      varExpression_ast *varExpression;
      const list_node<postprefixOperator_ast *> *postfixOperator_sequence;
    };

  struct  varExpression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_varExpression
      };

      variable_ast *variable;
      varExpressionNewObject_ast *newObject;
      varExpressionNormal_ast *varExpressionNormal;
    };

  struct  varExpressionNewObject_ast: public ast_node
    {
      enum
      {
        KIND = Kind_varExpressionNewObject
      };

      classNameReference_ast *className;
      ctorArguments_ast *ctor;
    };

  struct  varExpressionNormal_ast: public ast_node
    {
      enum
      {
        KIND = Kind_varExpressionNormal
      };

      expr_ast *expression;
      encapsList_ast *encapsList;
      variable_ast *variable;
      scalar_ast *scalar;
      const list_node<arrayPairValue_ast *> *arrayValues_sequence;
      const list_node<variable_ast *> *issetVariable_sequence;
      variable_ast *emptyVarialbe;
      varExpressionNewObject_ast *newObject;
      varExpressionNormal_ast *cloneCar;
    };

  struct  variable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_variable
      };

      baseVariableWithFunctionCalls_ast *var;
      const list_node<variableProperty_ast *> *variableProperties_sequence;
    };

  struct  variableName_ast: public ast_node
    {
      enum
      {
        KIND = Kind_variableName
      };

      std::size_t name;
      expr_ast *expr;
    };

  struct  variableProperty_ast: public ast_node
    {
      enum
      {
        KIND = Kind_variableProperty
      };

      objectProperty_ast *objectProperty;
      functionCallParameterList_ast *parameterList;
    };

  struct  variableWithoutObjects_ast: public ast_node
    {
      enum
      {
        KIND = Kind_variableWithoutObjects
      };

      compoundVariableWithSimpleIndirectReference_ast *variable;
      const list_node<dimListItem_ast *> *offsetItems_sequence;
    };

  struct  whileStatement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_whileStatement
      };

      statement_ast *statement;
      innerStatementList_ast *statements;
    };



} // end of namespace rphp

#endif


