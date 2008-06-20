// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef rphp_DEFAULT_VISITOR_H_INCLUDED
#define rphp_DEFAULT_VISITOR_H_INCLUDED

#include "rphp_visitor.h"

#include <parserexport.h>

namespace rphp
  {

  class KDEVPHPPARSER_EXPORT default_visitor:  public visitor
    {

    public:
      virtual void visit_additiveExpression(additiveExpression_ast *node);
      virtual void visit_additiveExpressionRest(additiveExpressionRest_ast *node);
      virtual void visit_arrayPairValue(arrayPairValue_ast *node);
      virtual void visit_assignmentExpression(assignmentExpression_ast *node);
      virtual void visit_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast *node);
      virtual void visit_assignmentExpressionEqual(assignmentExpressionEqual_ast *node);
      virtual void visit_assignmentList(assignmentList_ast *node);
      virtual void visit_assignmentListElement(assignmentListElement_ast *node);
      virtual void visit_baseVariable(baseVariable_ast *node);
      virtual void visit_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast *node);
      virtual void visit_bitAndExpression(bitAndExpression_ast *node);
      virtual void visit_bitOrExpression(bitOrExpression_ast *node);
      virtual void visit_bitXorExpression(bitXorExpression_ast *node);
      virtual void visit_booleanAndExpression(booleanAndExpression_ast *node);
      virtual void visit_booleanOrExpression(booleanOrExpression_ast *node);
      virtual void visit_caseList(caseList_ast *node);
      virtual void visit_case_item(case_item_ast *node);
      virtual void visit_catch_item(catch_item_ast *node);
      virtual void visit_classConstantDeclaration(classConstantDeclaration_ast *node);
      virtual void visit_classDeclarationStatement(classDeclarationStatement_ast *node);
      virtual void visit_classNameReference(classNameReference_ast *node);
      virtual void visit_classStatement(classStatement_ast *node);
      virtual void visit_classVariable(classVariable_ast *node);
      virtual void visit_classVariableDeclaration(classVariableDeclaration_ast *node);
      virtual void visit_commonScalar(commonScalar_ast *node);
      virtual void visit_compoundVariable(compoundVariable_ast *node);
      virtual void visit_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast *node);
      virtual void visit_conditionalExpression(conditionalExpression_ast *node);
      virtual void visit_ctorArguments(ctorArguments_ast *node);
      virtual void visit_declareItem(declareItem_ast *node);
      virtual void visit_declareStatement(declareStatement_ast *node);
      virtual void visit_dimListItem(dimListItem_ast *node);
      virtual void visit_dimOffset(dimOffset_ast *node);
      virtual void visit_dynamicClassNameReference(dynamicClassNameReference_ast *node);
      virtual void visit_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast *node);
      virtual void visit_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast *node);
      virtual void visit_elseSingle(elseSingle_ast *node);
      virtual void visit_elseifList(elseifList_ast *node);
      virtual void visit_elseifListItem(elseifListItem_ast *node);
      virtual void visit_encaps(encaps_ast *node);
      virtual void visit_encapsList(encapsList_ast *node);
      virtual void visit_encapsVar(encapsVar_ast *node);
      virtual void visit_encapsVarOffset(encapsVarOffset_ast *node);
      virtual void visit_equalityExpression(equalityExpression_ast *node);
      virtual void visit_equalityExpressionRest(equalityExpressionRest_ast *node);
      virtual void visit_expr(expr_ast *node);
      virtual void visit_forExpr(forExpr_ast *node);
      virtual void visit_forStatement(forStatement_ast *node);
      virtual void visit_foreachOptionalArg(foreachOptionalArg_ast *node);
      virtual void visit_foreachStatement(foreachStatement_ast *node);
      virtual void visit_foreachVariable(foreachVariable_ast *node);
      virtual void visit_functionCall(functionCall_ast *node);
      virtual void visit_functionCallParameterList(functionCallParameterList_ast *node);
      virtual void visit_functionCallParameterListElement(functionCallParameterListElement_ast *node);
      virtual void visit_functionDeclarationStatement(functionDeclarationStatement_ast *node);
      virtual void visit_globalVar(globalVar_ast *node);
      virtual void visit_innerStatementList(innerStatementList_ast *node);
      virtual void visit_logicalAndExpression(logicalAndExpression_ast *node);
      virtual void visit_logicalOrExpression(logicalOrExpression_ast *node);
      virtual void visit_logicalXorExpression(logicalXorExpression_ast *node);
      virtual void visit_memberModifier(memberModifier_ast *node);
      virtual void visit_methodBody(methodBody_ast *node);
      virtual void visit_multiplicativeExpression(multiplicativeExpression_ast *node);
      virtual void visit_multiplicativeExpression_rest(multiplicativeExpression_rest_ast *node);
      virtual void visit_newElseSingle(newElseSingle_ast *node);
      virtual void visit_newElseifList(newElseifList_ast *node);
      virtual void visit_newelseifListItem(newelseifListItem_ast *node);
      virtual void visit_objectDimList(objectDimList_ast *node);
      virtual void visit_objectProperty(objectProperty_ast *node);
      virtual void visit_parameter(parameter_ast *node);
      virtual void visit_parameterList(parameterList_ast *node);
      virtual void visit_postprefixOperator(postprefixOperator_ast *node);
      virtual void visit_printExpression(printExpression_ast *node);
      virtual void visit_relationalExpression(relationalExpression_ast *node);
      virtual void visit_relationalExpressionRest(relationalExpressionRest_ast *node);
      virtual void visit_scalar(scalar_ast *node);
      virtual void visit_semicolonOrCloseTag(semicolonOrCloseTag_ast *node);
      virtual void visit_shiftExpression(shiftExpression_ast *node);
      virtual void visit_shiftExpressionRest(shiftExpressionRest_ast *node);
      virtual void visit_start(start_ast *node);
      virtual void visit_statement(statement_ast *node);
      virtual void visit_staticArrayPairValue(staticArrayPairValue_ast *node);
      virtual void visit_staticMember(staticMember_ast *node);
      virtual void visit_staticScalar(staticScalar_ast *node);
      virtual void visit_staticVar(staticVar_ast *node);
      virtual void visit_switchCaseList(switchCaseList_ast *node);
      virtual void visit_topStatement(topStatement_ast *node);
      virtual void visit_unaryExpression(unaryExpression_ast *node);
      virtual void visit_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast *node);
      virtual void visit_varExpression(varExpression_ast *node);
      virtual void visit_varExpressionNewObject(varExpressionNewObject_ast *node);
      virtual void visit_varExpressionNormal(varExpressionNormal_ast *node);
      virtual void visit_variable(variable_ast *node);
      virtual void visit_variableName(variableName_ast *node);
      virtual void visit_variableProperty(variableProperty_ast *node);
      virtual void visit_variableWithoutObjects(variableWithoutObjects_ast *node);
      virtual void visit_whileStatement(whileStatement_ast *node);
    };

} // end of namespace rphp

#endif


