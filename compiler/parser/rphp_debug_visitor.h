// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef rphp_DEBUG_VISITOR_H_INCLUDED
#define rphp_DEBUG_VISITOR_H_INCLUDED

#include "rphp_default_visitor.h"

#include <iostream>
#include <fstream>

namespace rphp
  {

  class  debug_visitor: public default_visitor
    {
    public:
      virtual void visit_additiveExpression(additiveExpression_ast *node)
      {
        std::cout << "additiveExpression" << std::endl;
        default_visitor::visit_additiveExpression(node);
      }

      virtual void visit_additiveExpressionRest(additiveExpressionRest_ast *node)
      {
        std::cout << "additiveExpressionRest" << std::endl;
        default_visitor::visit_additiveExpressionRest(node);
      }

      virtual void visit_arrayPairValue(arrayPairValue_ast *node)
      {
        std::cout << "arrayPairValue" << std::endl;
        default_visitor::visit_arrayPairValue(node);
      }

      virtual void visit_assignmentExpression(assignmentExpression_ast *node)
      {
        std::cout << "assignmentExpression" << std::endl;
        default_visitor::visit_assignmentExpression(node);
      }

      virtual void visit_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast *node)
      {
        std::cout << "assignmentExpressionCheckIfVariable" << std::endl;
        default_visitor::visit_assignmentExpressionCheckIfVariable(node);
      }

      virtual void visit_assignmentExpressionEqual(assignmentExpressionEqual_ast *node)
      {
        std::cout << "assignmentExpressionEqual" << std::endl;
        default_visitor::visit_assignmentExpressionEqual(node);
      }

      virtual void visit_assignmentList(assignmentList_ast *node)
      {
        std::cout << "assignmentList" << std::endl;
        default_visitor::visit_assignmentList(node);
      }

      virtual void visit_assignmentListElement(assignmentListElement_ast *node)
      {
        std::cout << "assignmentListElement" << std::endl;
        default_visitor::visit_assignmentListElement(node);
      }

      virtual void visit_baseVariable(baseVariable_ast *node)
      {
        std::cout << "baseVariable" << std::endl;
        default_visitor::visit_baseVariable(node);
      }

      virtual void visit_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast *node)
      {
        std::cout << "baseVariableWithFunctionCalls" << std::endl;
        default_visitor::visit_baseVariableWithFunctionCalls(node);
      }

      virtual void visit_bitAndExpression(bitAndExpression_ast *node)
      {
        std::cout << "bitAndExpression" << std::endl;
        default_visitor::visit_bitAndExpression(node);
      }

      virtual void visit_bitOrExpression(bitOrExpression_ast *node)
      {
        std::cout << "bitOrExpression" << std::endl;
        default_visitor::visit_bitOrExpression(node);
      }

      virtual void visit_bitXorExpression(bitXorExpression_ast *node)
      {
        std::cout << "bitXorExpression" << std::endl;
        default_visitor::visit_bitXorExpression(node);
      }

      virtual void visit_booleanAndExpression(booleanAndExpression_ast *node)
      {
        std::cout << "booleanAndExpression" << std::endl;
        default_visitor::visit_booleanAndExpression(node);
      }

      virtual void visit_booleanOrExpression(booleanOrExpression_ast *node)
      {
        std::cout << "booleanOrExpression" << std::endl;
        default_visitor::visit_booleanOrExpression(node);
      }

      virtual void visit_caseList(caseList_ast *node)
      {
        std::cout << "caseList" << std::endl;
        default_visitor::visit_caseList(node);
      }

      virtual void visit_case_item(case_item_ast *node)
      {
        std::cout << "case_item" << std::endl;
        default_visitor::visit_case_item(node);
      }

      virtual void visit_catch_item(catch_item_ast *node)
      {
        std::cout << "catch_item" << std::endl;
        default_visitor::visit_catch_item(node);
      }

      virtual void visit_classConstantDeclaration(classConstantDeclaration_ast *node)
      {
        std::cout << "classConstantDeclaration" << std::endl;
        default_visitor::visit_classConstantDeclaration(node);
      }

      virtual void visit_classDeclarationStatement(classDeclarationStatement_ast *node)
      {
        std::cout << "classDeclarationStatement" << std::endl;
        default_visitor::visit_classDeclarationStatement(node);
      }

      virtual void visit_classNameReference(classNameReference_ast *node)
      {
        std::cout << "classNameReference" << std::endl;
        default_visitor::visit_classNameReference(node);
      }

      virtual void visit_classStatement(classStatement_ast *node)
      {
        std::cout << "classStatement" << std::endl;
        default_visitor::visit_classStatement(node);
      }

      virtual void visit_classVariable(classVariable_ast *node)
      {
        std::cout << "classVariable" << std::endl;
        default_visitor::visit_classVariable(node);
      }

      virtual void visit_classVariableDeclaration(classVariableDeclaration_ast *node)
      {
        std::cout << "classVariableDeclaration" << std::endl;
        default_visitor::visit_classVariableDeclaration(node);
      }

      virtual void visit_commonScalar(commonScalar_ast *node)
      {
        std::cout << "commonScalar" << std::endl;
        default_visitor::visit_commonScalar(node);
      }

      virtual void visit_compoundVariable(compoundVariable_ast *node)
      {
        std::cout << "compoundVariable" << std::endl;
        default_visitor::visit_compoundVariable(node);
      }

      virtual void visit_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast *node)
      {
        std::cout << "compoundVariableWithSimpleIndirectReference" << std::endl;
        default_visitor::visit_compoundVariableWithSimpleIndirectReference(node);
      }

      virtual void visit_conditionalExpression(conditionalExpression_ast *node)
      {
        std::cout << "conditionalExpression" << std::endl;
        default_visitor::visit_conditionalExpression(node);
      }

      virtual void visit_ctorArguments(ctorArguments_ast *node)
      {
        std::cout << "ctorArguments" << std::endl;
        default_visitor::visit_ctorArguments(node);
      }

      virtual void visit_declareItem(declareItem_ast *node)
      {
        std::cout << "declareItem" << std::endl;
        default_visitor::visit_declareItem(node);
      }

      virtual void visit_declareStatement(declareStatement_ast *node)
      {
        std::cout << "declareStatement" << std::endl;
        default_visitor::visit_declareStatement(node);
      }

      virtual void visit_dimListItem(dimListItem_ast *node)
      {
        std::cout << "dimListItem" << std::endl;
        default_visitor::visit_dimListItem(node);
      }

      virtual void visit_dimOffset(dimOffset_ast *node)
      {
        std::cout << "dimOffset" << std::endl;
        default_visitor::visit_dimOffset(node);
      }

      virtual void visit_dynamicClassNameReference(dynamicClassNameReference_ast *node)
      {
        std::cout << "dynamicClassNameReference" << std::endl;
        default_visitor::visit_dynamicClassNameReference(node);
      }

      virtual void visit_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast *node)
      {
        std::cout << "dynamicClassNameVariableProperties" << std::endl;
        default_visitor::visit_dynamicClassNameVariableProperties(node);
      }

      virtual void visit_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast *node)
      {
        std::cout << "dynamicClassNameVariableProperty" << std::endl;
        default_visitor::visit_dynamicClassNameVariableProperty(node);
      }

      virtual void visit_elseSingle(elseSingle_ast *node)
      {
        std::cout << "elseSingle" << std::endl;
        default_visitor::visit_elseSingle(node);
      }

      virtual void visit_elseifList(elseifList_ast *node)
      {
        std::cout << "elseifList" << std::endl;
        default_visitor::visit_elseifList(node);
      }

      virtual void visit_elseifListItem(elseifListItem_ast *node)
      {
        std::cout << "elseifListItem" << std::endl;
        default_visitor::visit_elseifListItem(node);
      }

      virtual void visit_encaps(encaps_ast *node)
      {
        std::cout << "encaps" << std::endl;
        default_visitor::visit_encaps(node);
      }

      virtual void visit_encapsList(encapsList_ast *node)
      {
        std::cout << "encapsList" << std::endl;
        default_visitor::visit_encapsList(node);
      }

      virtual void visit_encapsVar(encapsVar_ast *node)
      {
        std::cout << "encapsVar" << std::endl;
        default_visitor::visit_encapsVar(node);
      }

      virtual void visit_encapsVarOffset(encapsVarOffset_ast *node)
      {
        std::cout << "encapsVarOffset" << std::endl;
        default_visitor::visit_encapsVarOffset(node);
      }

      virtual void visit_equalityExpression(equalityExpression_ast *node)
      {
        std::cout << "equalityExpression" << std::endl;
        default_visitor::visit_equalityExpression(node);
      }

      virtual void visit_equalityExpressionRest(equalityExpressionRest_ast *node)
      {
        std::cout << "equalityExpressionRest" << std::endl;
        default_visitor::visit_equalityExpressionRest(node);
      }

      virtual void visit_expr(expr_ast *node)
      {
        std::cout << "expr" << std::endl;
        default_visitor::visit_expr(node);
      }

      virtual void visit_forExpr(forExpr_ast *node)
      {
        std::cout << "forExpr" << std::endl;
        default_visitor::visit_forExpr(node);
      }

      virtual void visit_forStatement(forStatement_ast *node)
      {
        std::cout << "forStatement" << std::endl;
        default_visitor::visit_forStatement(node);
      }

      virtual void visit_foreachOptionalArg(foreachOptionalArg_ast *node)
      {
        std::cout << "foreachOptionalArg" << std::endl;
        default_visitor::visit_foreachOptionalArg(node);
      }

      virtual void visit_foreachStatement(foreachStatement_ast *node)
      {
        std::cout << "foreachStatement" << std::endl;
        default_visitor::visit_foreachStatement(node);
      }

      virtual void visit_foreachVariable(foreachVariable_ast *node)
      {
        std::cout << "foreachVariable" << std::endl;
        default_visitor::visit_foreachVariable(node);
      }

      virtual void visit_functionCall(functionCall_ast *node)
      {
        std::cout << "functionCall" << std::endl;
        default_visitor::visit_functionCall(node);
      }

      virtual void visit_functionCallParameterList(functionCallParameterList_ast *node)
      {
        std::cout << "functionCallParameterList" << std::endl;
        default_visitor::visit_functionCallParameterList(node);
      }

      virtual void visit_functionCallParameterListElement(functionCallParameterListElement_ast *node)
      {
        std::cout << "functionCallParameterListElement" << std::endl;
        default_visitor::visit_functionCallParameterListElement(node);
      }

      virtual void visit_functionDeclarationStatement(functionDeclarationStatement_ast *node)
      {
        std::cout << "functionDeclarationStatement" << std::endl;
        default_visitor::visit_functionDeclarationStatement(node);
      }

      virtual void visit_globalVar(globalVar_ast *node)
      {
        std::cout << "globalVar" << std::endl;
        default_visitor::visit_globalVar(node);
      }

      virtual void visit_innerStatementList(innerStatementList_ast *node)
      {
        std::cout << "innerStatementList" << std::endl;
        default_visitor::visit_innerStatementList(node);
      }

      virtual void visit_logicalAndExpression(logicalAndExpression_ast *node)
      {
        std::cout << "logicalAndExpression" << std::endl;
        default_visitor::visit_logicalAndExpression(node);
      }

      virtual void visit_logicalOrExpression(logicalOrExpression_ast *node)
      {
        std::cout << "logicalOrExpression" << std::endl;
        default_visitor::visit_logicalOrExpression(node);
      }

      virtual void visit_logicalXorExpression(logicalXorExpression_ast *node)
      {
        std::cout << "logicalXorExpression" << std::endl;
        default_visitor::visit_logicalXorExpression(node);
      }

      virtual void visit_memberModifier(memberModifier_ast *node)
      {
        std::cout << "memberModifier" << std::endl;
        default_visitor::visit_memberModifier(node);
      }

      virtual void visit_methodBody(methodBody_ast *node)
      {
        std::cout << "methodBody" << std::endl;
        default_visitor::visit_methodBody(node);
      }

      virtual void visit_multiplicativeExpression(multiplicativeExpression_ast *node)
      {
        std::cout << "multiplicativeExpression" << std::endl;
        default_visitor::visit_multiplicativeExpression(node);
      }

      virtual void visit_multiplicativeExpression_rest(multiplicativeExpression_rest_ast *node)
      {
        std::cout << "multiplicativeExpression_rest" << std::endl;
        default_visitor::visit_multiplicativeExpression_rest(node);
      }

      virtual void visit_newElseSingle(newElseSingle_ast *node)
      {
        std::cout << "newElseSingle" << std::endl;
        default_visitor::visit_newElseSingle(node);
      }

      virtual void visit_newElseifList(newElseifList_ast *node)
      {
        std::cout << "newElseifList" << std::endl;
        default_visitor::visit_newElseifList(node);
      }

      virtual void visit_newelseifListItem(newelseifListItem_ast *node)
      {
        std::cout << "newelseifListItem" << std::endl;
        default_visitor::visit_newelseifListItem(node);
      }

      virtual void visit_objectDimList(objectDimList_ast *node)
      {
        std::cout << "objectDimList" << std::endl;
        default_visitor::visit_objectDimList(node);
      }

      virtual void visit_objectProperty(objectProperty_ast *node)
      {
        std::cout << "objectProperty" << std::endl;
        default_visitor::visit_objectProperty(node);
      }

      virtual void visit_parameter(parameter_ast *node)
      {
        std::cout << "parameter" << std::endl;
        default_visitor::visit_parameter(node);
      }

      virtual void visit_parameterList(parameterList_ast *node)
      {
        std::cout << "parameterList" << std::endl;
        default_visitor::visit_parameterList(node);
      }

      virtual void visit_postprefixOperator(postprefixOperator_ast *node)
      {
        std::cout << "postprefixOperator" << std::endl;
        default_visitor::visit_postprefixOperator(node);
      }

      virtual void visit_printExpression(printExpression_ast *node)
      {
        std::cout << "printExpression" << std::endl;
        default_visitor::visit_printExpression(node);
      }

      virtual void visit_relationalExpression(relationalExpression_ast *node)
      {
        std::cout << "relationalExpression" << std::endl;
        default_visitor::visit_relationalExpression(node);
      }

      virtual void visit_relationalExpressionRest(relationalExpressionRest_ast *node)
      {
        std::cout << "relationalExpressionRest" << std::endl;
        default_visitor::visit_relationalExpressionRest(node);
      }

      virtual void visit_scalar(scalar_ast *node)
      {
        std::cout << "scalar" << std::endl;
        default_visitor::visit_scalar(node);
      }

      virtual void visit_semicolonOrCloseTag(semicolonOrCloseTag_ast *node)
      {
        std::cout << "semicolonOrCloseTag" << std::endl;
        default_visitor::visit_semicolonOrCloseTag(node);
      }

      virtual void visit_shiftExpression(shiftExpression_ast *node)
      {
        std::cout << "shiftExpression" << std::endl;
        default_visitor::visit_shiftExpression(node);
      }

      virtual void visit_shiftExpressionRest(shiftExpressionRest_ast *node)
      {
        std::cout << "shiftExpressionRest" << std::endl;
        default_visitor::visit_shiftExpressionRest(node);
      }

      virtual void visit_start(start_ast *node)
      {
        std::cout << "start" << std::endl;
        default_visitor::visit_start(node);
      }

      virtual void visit_statement(statement_ast *node)
      {
        std::cout << "statement" << std::endl;
        default_visitor::visit_statement(node);
      }

      virtual void visit_staticArrayPairValue(staticArrayPairValue_ast *node)
      {
        std::cout << "staticArrayPairValue" << std::endl;
        default_visitor::visit_staticArrayPairValue(node);
      }

      virtual void visit_staticMember(staticMember_ast *node)
      {
        std::cout << "staticMember" << std::endl;
        default_visitor::visit_staticMember(node);
      }

      virtual void visit_staticScalar(staticScalar_ast *node)
      {
        std::cout << "staticScalar" << std::endl;
        default_visitor::visit_staticScalar(node);
      }

      virtual void visit_staticVar(staticVar_ast *node)
      {
        std::cout << "staticVar" << std::endl;
        default_visitor::visit_staticVar(node);
      }

      virtual void visit_switchCaseList(switchCaseList_ast *node)
      {
        std::cout << "switchCaseList" << std::endl;
        default_visitor::visit_switchCaseList(node);
      }

      virtual void visit_topStatement(topStatement_ast *node)
      {
        std::cout << "topStatement" << std::endl;
        default_visitor::visit_topStatement(node);
      }

      virtual void visit_unaryExpression(unaryExpression_ast *node)
      {
        std::cout << "unaryExpression" << std::endl;
        default_visitor::visit_unaryExpression(node);
      }

      virtual void visit_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast *node)
      {
        std::cout << "unaryExpression_not_plusminus" << std::endl;
        default_visitor::visit_unaryExpression_not_plusminus(node);
      }

      virtual void visit_varExpression(varExpression_ast *node)
      {
        std::cout << "varExpression" << std::endl;
        default_visitor::visit_varExpression(node);
      }

      virtual void visit_varExpressionNewObject(varExpressionNewObject_ast *node)
      {
        std::cout << "varExpressionNewObject" << std::endl;
        default_visitor::visit_varExpressionNewObject(node);
      }

      virtual void visit_varExpressionNormal(varExpressionNormal_ast *node)
      {
        std::cout << "varExpressionNormal" << std::endl;
        default_visitor::visit_varExpressionNormal(node);
      }

      virtual void visit_variable(variable_ast *node)
      {
        std::cout << "variable" << std::endl;
        default_visitor::visit_variable(node);
      }

      virtual void visit_variableName(variableName_ast *node)
      {
        std::cout << "variableName" << std::endl;
        default_visitor::visit_variableName(node);
      }

      virtual void visit_variableProperty(variableProperty_ast *node)
      {
        std::cout << "variableProperty" << std::endl;
        default_visitor::visit_variableProperty(node);
      }

      virtual void visit_variableWithoutObjects(variableWithoutObjects_ast *node)
      {
        std::cout << "variableWithoutObjects" << std::endl;
        default_visitor::visit_variableWithoutObjects(node);
      }

      virtual void visit_whileStatement(whileStatement_ast *node)
      {
        std::cout << "whileStatement" << std::endl;
        default_visitor::visit_whileStatement(node);
      }

    };

} // end of namespace rphp

#endif


