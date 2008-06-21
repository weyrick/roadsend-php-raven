// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef rphp_VISITOR_H_INCLUDED
#define rphp_VISITOR_H_INCLUDED

#include "rphp_ast.h"

namespace rphp
  {

  class  visitor
    {
      typedef void (visitor::*parser_fun_t)(ast_node *);
      static parser_fun_t _S_parser_table[];

    public:
      virtual ~visitor()
      {}

      virtual void visit_node(ast_node *node)
      {
        if  (node)
          (this->*_S_parser_table[node->kind -  1000])(node);
      }

      virtual void visit_additiveExpression(additiveExpression_ast *)
    {}

      virtual void visit_additiveExpressionRest(additiveExpressionRest_ast *)
      {}

      virtual void visit_arrayPairValue(arrayPairValue_ast *)
      {}

      virtual void visit_assignmentExpression(assignmentExpression_ast *)
      {}

      virtual void visit_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast *)
      {}

      virtual void visit_assignmentExpressionEqual(assignmentExpressionEqual_ast *)
      {}

      virtual void visit_assignmentList(assignmentList_ast *)
      {}

      virtual void visit_assignmentListElement(assignmentListElement_ast *)
      {}

      virtual void visit_baseVariable(baseVariable_ast *)
      {}

      virtual void visit_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast *)
      {}

      virtual void visit_bitAndExpression(bitAndExpression_ast *)
      {}

      virtual void visit_bitOrExpression(bitOrExpression_ast *)
      {}

      virtual void visit_bitXorExpression(bitXorExpression_ast *)
      {}

      virtual void visit_booleanAndExpression(booleanAndExpression_ast *)
      {}

      virtual void visit_booleanOrExpression(booleanOrExpression_ast *)
      {}

      virtual void visit_caseList(caseList_ast *)
      {}

      virtual void visit_case_item(case_item_ast *)
      {}

      virtual void visit_catch_item(catch_item_ast *)
      {}

      virtual void visit_classConstantDeclaration(classConstantDeclaration_ast *)
      {}

      virtual void visit_classDeclarationStatement(classDeclarationStatement_ast *)
      {}

      virtual void visit_classNameReference(classNameReference_ast *)
      {}

      virtual void visit_classStatement(classStatement_ast *)
      {}

      virtual void visit_classVariable(classVariable_ast *)
      {}

      virtual void visit_classVariableDeclaration(classVariableDeclaration_ast *)
      {}

      virtual void visit_commonScalar(commonScalar_ast *)
      {}

      virtual void visit_compoundVariable(compoundVariable_ast *)
      {}

      virtual void visit_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast *)
      {}

      virtual void visit_conditionalExpression(conditionalExpression_ast *)
      {}

      virtual void visit_ctorArguments(ctorArguments_ast *)
      {}

      virtual void visit_declareItem(declareItem_ast *)
      {}

      virtual void visit_declareStatement(declareStatement_ast *)
      {}

      virtual void visit_dimListItem(dimListItem_ast *)
      {}

      virtual void visit_dimOffset(dimOffset_ast *)
      {}

      virtual void visit_dynamicClassNameReference(dynamicClassNameReference_ast *)
      {}

      virtual void visit_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast *)
      {}

      virtual void visit_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast *)
      {}

      virtual void visit_elseSingle(elseSingle_ast *)
      {}

      virtual void visit_elseifList(elseifList_ast *)
      {}

      virtual void visit_elseifListItem(elseifListItem_ast *)
      {}

      virtual void visit_encaps(encaps_ast *)
      {}

      virtual void visit_encapsList(encapsList_ast *)
      {}

      virtual void visit_encapsVar(encapsVar_ast *)
      {}

      virtual void visit_encapsVarOffset(encapsVarOffset_ast *)
      {}

      virtual void visit_equalityExpression(equalityExpression_ast *)
      {}

      virtual void visit_equalityExpressionRest(equalityExpressionRest_ast *)
      {}

      virtual void visit_expr(expr_ast *)
      {}

      virtual void visit_forExpr(forExpr_ast *)
      {}

      virtual void visit_forStatement(forStatement_ast *)
      {}

      virtual void visit_foreachOptionalArg(foreachOptionalArg_ast *)
      {}

      virtual void visit_foreachStatement(foreachStatement_ast *)
      {}

      virtual void visit_foreachVariable(foreachVariable_ast *)
      {}

      virtual void visit_functionCall(functionCall_ast *)
      {}

      virtual void visit_functionCallParameterList(functionCallParameterList_ast *)
      {}

      virtual void visit_functionCallParameterListElement(functionCallParameterListElement_ast *)
      {}

      virtual void visit_functionDeclarationStatement(functionDeclarationStatement_ast *)
      {}

      virtual void visit_globalVar(globalVar_ast *)
      {}

      virtual void visit_innerStatementList(innerStatementList_ast *)
      {}

      virtual void visit_logicalAndExpression(logicalAndExpression_ast *)
      {}

      virtual void visit_logicalOrExpression(logicalOrExpression_ast *)
      {}

      virtual void visit_logicalXorExpression(logicalXorExpression_ast *)
      {}

      virtual void visit_memberModifier(memberModifier_ast *)
      {}

      virtual void visit_methodBody(methodBody_ast *)
      {}

      virtual void visit_multiplicativeExpression(multiplicativeExpression_ast *)
      {}

      virtual void visit_multiplicativeExpression_rest(multiplicativeExpression_rest_ast *)
      {}

      virtual void visit_newElseSingle(newElseSingle_ast *)
      {}

      virtual void visit_newElseifList(newElseifList_ast *)
      {}

      virtual void visit_newelseifListItem(newelseifListItem_ast *)
      {}

      virtual void visit_objectDimList(objectDimList_ast *)
      {}

      virtual void visit_objectProperty(objectProperty_ast *)
      {}

      virtual void visit_parameter(parameter_ast *)
      {}

      virtual void visit_parameterList(parameterList_ast *)
      {}

      virtual void visit_postprefixOperator(postprefixOperator_ast *)
      {}

      virtual void visit_printExpression(printExpression_ast *)
      {}

      virtual void visit_relationalExpression(relationalExpression_ast *)
      {}

      virtual void visit_relationalExpressionRest(relationalExpressionRest_ast *)
      {}

      virtual void visit_scalar(scalar_ast *)
      {}

      virtual void visit_semicolonOrCloseTag(semicolonOrCloseTag_ast *)
      {}

      virtual void visit_shiftExpression(shiftExpression_ast *)
      {}

      virtual void visit_shiftExpressionRest(shiftExpressionRest_ast *)
      {}

      virtual void visit_start(start_ast *)
      {}

      virtual void visit_statement(statement_ast *)
      {}

      virtual void visit_staticArrayPairValue(staticArrayPairValue_ast *)
      {}

      virtual void visit_staticMember(staticMember_ast *)
      {}

      virtual void visit_staticScalar(staticScalar_ast *)
      {}

      virtual void visit_staticVar(staticVar_ast *)
      {}

      virtual void visit_switchCaseList(switchCaseList_ast *)
      {}

      virtual void visit_topStatement(topStatement_ast *)
      {}

      virtual void visit_unaryExpression(unaryExpression_ast *)
      {}

      virtual void visit_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast *)
      {}

      virtual void visit_varExpression(varExpression_ast *)
      {}

      virtual void visit_varExpressionNewObject(varExpressionNewObject_ast *)
      {}

      virtual void visit_varExpressionNormal(varExpressionNormal_ast *)
      {}

      virtual void visit_variable(variable_ast *)
      {}

      virtual void visit_variableName(variableName_ast *)
      {}

      virtual void visit_variableProperty(variableProperty_ast *)
      {}

      virtual void visit_variableWithoutObjects(variableWithoutObjects_ast *)
      {}

      virtual void visit_whileStatement(whileStatement_ast *)
      {}

    }

  ;

} // end of namespace rphp

#endif


