// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "rphp_default_visitor.h"

namespace rphp
  {

  void default_visitor::visit_additiveExpression(additiveExpression_ast *node)
  {
    visit_node(node->expression);
    if (node->additionalExpression_sequence)
      {
        const list_node<additiveExpressionRest_ast*> *__it = node->additionalExpression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_additiveExpressionRest(additiveExpressionRest_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_arrayPairValue(arrayPairValue_ast *node)
  {
    visit_node(node->expr);
    visit_node(node->exprValue);
    visit_node(node->varValue);
    visit_node(node->variable);
  }

  void default_visitor::visit_assignmentExpression(assignmentExpression_ast *node)
  {
    visit_node(node->conditionalExpression);
    visit_node(node->assignmentAxpressionEqual);
    visit_node(node->assignmentExpression);
  }

  void default_visitor::visit_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast *)
  {}

  void default_visitor::visit_assignmentExpressionEqual(assignmentExpressionEqual_ast *node)
  {
    visit_node(node->assignmentExpression);
  }

  void default_visitor::visit_assignmentList(assignmentList_ast *node)
  {
    if (node->element_sequence)
      {
        const list_node<assignmentListElement_ast*> *__it = node->element_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_assignmentListElement(assignmentListElement_ast *node)
  {
    visit_node(node->variable);
    visit_node(node->assignmentList);
  }

  void default_visitor::visit_baseVariable(baseVariable_ast *node)
  {
    visit_node(node->var);
    if (node->offsetItems_sequence)
      {
        const list_node<dimListItem_ast*> *__it = node->offsetItems_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->staticMember);
  }

  void default_visitor::visit_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast *node)
  {
    visit_node(node->functionCall);
    visit_node(node->baseVariable);
  }

  void default_visitor::visit_bitAndExpression(bitAndExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<equalityExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_bitOrExpression(bitOrExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<bitXorExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_bitXorExpression(bitXorExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<bitAndExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_booleanAndExpression(booleanAndExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<bitOrExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_booleanOrExpression(booleanOrExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<booleanAndExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_caseList(caseList_ast *node)
  {
    if (node->caseItems_sequence)
      {
        const list_node<case_item_ast*> *__it = node->caseItems_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_case_item(case_item_ast *node)
  {
    visit_node(node->expr);
    visit_node(node->statements);
  }

  void default_visitor::visit_catch_item(catch_item_ast *node)
  {
    visit_node(node->statements);
  }

  void default_visitor::visit_classConstantDeclaration(classConstantDeclaration_ast *node)
  {
    visit_node(node->scalar);
  }

  void default_visitor::visit_classDeclarationStatement(classDeclarationStatement_ast *node)
  {
    if (node->statements_sequence)
      {
        const list_node<classStatement_ast*> *__it = node->statements_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_classNameReference(classNameReference_ast *node)
  {
    visit_node(node->dynamicClassNameReference);
  }

  void default_visitor::visit_classStatement(classStatement_ast *node)
  {
    visit_node(node->consts);
    visit_node(node->modifiers);
    visit_node(node->params);
    visit_node(node->body);
  }

  void default_visitor::visit_classVariable(classVariable_ast *node)
  {
    visit_node(node->value);
  }

  void default_visitor::visit_classVariableDeclaration(classVariableDeclaration_ast *node)
  {
    visit_node(node->vars);
  }

  void default_visitor::visit_commonScalar(commonScalar_ast *)
  {}

  void default_visitor::visit_compoundVariable(compoundVariable_ast *node)
  {
    visit_node(node->expr);
  }

  void default_visitor::visit_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast *node)
  {
    visit_node(node->expr);
  }

  void default_visitor::visit_conditionalExpression(conditionalExpression_ast *node)
  {
    visit_node(node->expression);
    visit_node(node->ifExpression);
    visit_node(node->elseExpression);
  }

  void default_visitor::visit_ctorArguments(ctorArguments_ast *node)
  {
    visit_node(node->parameterList);
  }

  void default_visitor::visit_declareItem(declareItem_ast *node)
  {
    visit_node(node->scalar);
  }

  void default_visitor::visit_declareStatement(declareStatement_ast *node)
  {
    visit_node(node->statement);
    visit_node(node->statements);
  }

  void default_visitor::visit_dimListItem(dimListItem_ast *node)
  {
    visit_node(node->dimOffset);
    visit_node(node->expr);
  }

  void default_visitor::visit_dimOffset(dimOffset_ast *node)
  {
    visit_node(node->expr);
  }

  void default_visitor::visit_dynamicClassNameReference(dynamicClassNameReference_ast *node)
  {
    visit_node(node->baseVariable);
    visit_node(node->objectProperty);
    visit_node(node->properties);
  }

  void default_visitor::visit_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast *node)
  {
    if (node->properties_sequence)
      {
        const list_node<dynamicClassNameVariableProperty_ast*> *__it = node->properties_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast *node)
  {
    visit_node(node->property);
  }

  void default_visitor::visit_elseSingle(elseSingle_ast *node)
  {
    visit_node(node->statement);
  }

  void default_visitor::visit_elseifList(elseifList_ast *node)
  {
    if (node->elseifListItem_sequence)
      {
        const list_node<elseifListItem_ast*> *__it = node->elseifListItem_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_elseifListItem(elseifListItem_ast *node)
  {
    visit_node(node->expr);
    visit_node(node->statement);
  }

  void default_visitor::visit_encaps(encaps_ast *node)
  {
    visit_node(node->var);
  }

  void default_visitor::visit_encapsList(encapsList_ast *node)
  {
    if (node->encaps_sequence)
      {
        const list_node<encaps_ast*> *__it = node->encaps_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_encapsVar(encapsVar_ast *node)
  {
    visit_node(node->expr);
    visit_node(node->offset);
  }

  void default_visitor::visit_encapsVarOffset(encapsVarOffset_ast *)
  {}

  void default_visitor::visit_equalityExpression(equalityExpression_ast *node)
  {
    visit_node(node->expression);
    if (node->additionalExpression_sequence)
      {
        const list_node<equalityExpressionRest_ast*> *__it = node->additionalExpression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_equalityExpressionRest(equalityExpressionRest_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_expr(expr_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_forExpr(forExpr_ast *node)
  {
    if (node->exprs_sequence)
      {
        const list_node<expr_ast*> *__it = node->exprs_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_forStatement(forStatement_ast *node)
  {
    visit_node(node->statement);
    visit_node(node->statements);
  }

  void default_visitor::visit_foreachOptionalArg(foreachOptionalArg_ast *node)
  {
    visit_node(node->foreachVariable);
  }

  void default_visitor::visit_foreachStatement(foreachStatement_ast *node)
  {
    visit_node(node->statement);
    visit_node(node->statements);
  }

  void default_visitor::visit_foreachVariable(foreachVariable_ast *node)
  {
    visit_node(node->variable);
  }

  void default_visitor::visit_functionCall(functionCall_ast *node)
  {
    visit_node(node->stringParameterList);
    visit_node(node->varFunctionName);
    visit_node(node->varParameterList);
  }

  void default_visitor::visit_functionCallParameterList(functionCallParameterList_ast *node)
  {
    if (node->parameters_sequence)
      {
        const list_node<functionCallParameterListElement_ast*> *__it = node->parameters_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_functionCallParameterListElement(functionCallParameterListElement_ast *node)
  {
    visit_node(node->variable);
    visit_node(node->expr);
  }

  void default_visitor::visit_functionDeclarationStatement(functionDeclarationStatement_ast *node)
  {
    visit_node(node->params);
    visit_node(node->statements);
  }

  void default_visitor::visit_globalVar(globalVar_ast *node)
  {
    visit_node(node->dollarVar);
    visit_node(node->expr);
  }

  void default_visitor::visit_innerStatementList(innerStatementList_ast *node)
  {
    if (node->statements_sequence)
      {
        const list_node<topStatement_ast*> *__it = node->statements_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_logicalAndExpression(logicalAndExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<printExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_logicalOrExpression(logicalOrExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<logicalXorExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_logicalXorExpression(logicalXorExpression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<logicalAndExpression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_memberModifier(memberModifier_ast *)
{}

  void default_visitor::visit_methodBody(methodBody_ast *node)
  {
    visit_node(node->statements);
  }

  void default_visitor::visit_multiplicativeExpression(multiplicativeExpression_ast *node)
  {
    visit_node(node->expression);
    if (node->additionalExpression_sequence)
      {
        const list_node<multiplicativeExpression_rest_ast*> *__it = node->additionalExpression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_multiplicativeExpression_rest(multiplicativeExpression_rest_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_newElseSingle(newElseSingle_ast *node)
  {
    visit_node(node->statements);
  }

  void default_visitor::visit_newElseifList(newElseifList_ast *node)
  {
    if (node->newElseifListItem_sequence)
      {
        const list_node<newelseifListItem_ast*> *__it = node->newElseifListItem_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_newelseifListItem(newelseifListItem_ast *node)
  {
    visit_node(node->expr);
    visit_node(node->statements);
  }

  void default_visitor::visit_objectDimList(objectDimList_ast *node)
  {
    visit_node(node->variableName);
    if (node->offsetItems_sequence)
      {
        const list_node<dimListItem_ast*> *__it = node->offsetItems_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_objectProperty(objectProperty_ast *node)
  {
    visit_node(node->objectDimList);
    visit_node(node->variableWithoutObjects);
  }

  void default_visitor::visit_parameter(parameter_ast *node)
  {
    visit_node(node->defaultValue);
  }

  void default_visitor::visit_parameterList(parameterList_ast *node)
  {
    if (node->params_sequence)
      {
        const list_node<parameter_ast*> *__it = node->params_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_postprefixOperator(postprefixOperator_ast *)
{}

  void default_visitor::visit_printExpression(printExpression_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_relationalExpression(relationalExpression_ast *node)
  {
    visit_node(node->expression);
    if (node->additionalExpression_sequence)
      {
        const list_node<relationalExpressionRest_ast*> *__it = node->additionalExpression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->instanceofType);
  }

  void default_visitor::visit_relationalExpressionRest(relationalExpressionRest_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_scalar(scalar_ast *node)
  {
    visit_node(node->commonScalar);
    visit_node(node->encapsList);
  }

  void default_visitor::visit_semicolonOrCloseTag(semicolonOrCloseTag_ast *)
  {}

  void default_visitor::visit_shiftExpression(shiftExpression_ast *node)
  {
    visit_node(node->expression);
    if (node->additionalExpression_sequence)
      {
        const list_node<shiftExpressionRest_ast*> *__it = node->additionalExpression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_shiftExpressionRest(shiftExpressionRest_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_start(start_ast *node)
  {
    visit_node(node->statements);
  }

  void default_visitor::visit_statement(statement_ast *node)
  {
    visit_node(node->statements);
    visit_node(node->ifExpr);
    visit_node(node->ifStatement);
    visit_node(node->elseifList);
    visit_node(node->elseSingle);
    visit_node(node->whileExpr);
    visit_node(node->whilteStatement);
    visit_node(node->forExpr1);
    visit_node(node->forExpr2);
    visit_node(node->forExpr3);
    visit_node(node->forStatement);
    visit_node(node->swtichExpr);
    visit_node(node->switchCaseList);
    visit_node(node->foreachVar);
    visit_node(node->foreachVarAsVar);
    visit_node(node->foreachExpr);
    visit_node(node->foreachExprAsVar);
    visit_node(node->foreachArg);
    visit_node(node->foreachStatement);
    visit_node(node->declareItem);
    if (node->catches_sequence)
      {
        const list_node<catch_item_ast*> *__it = node->catches_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->unsetVariables_sequence)
      {
        const list_node<variable_ast*> *__it = node->unsetVariables_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->expr);
    visit_node(node->doStatement);
    visit_node(node->whilteExpr);
    visit_node(node->breakExpr);
    visit_node(node->continueExpr);
    visit_node(node->returnExpr);
    if (node->globalVars_sequence)
      {
        const list_node<globalVar_ast*> *__it = node->globalVars_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->staticVars_sequence)
      {
        const list_node<staticVar_ast*> *__it = node->staticVars_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->echoExprs_sequence)
      {
        const list_node<expr_ast*> *__it = node->echoExprs_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->throwExpr);
  }

  void default_visitor::visit_staticArrayPairValue(staticArrayPairValue_ast *node)
  {
    if (node->val1_sequence)
      {
        const list_node<staticScalar_ast*> *__it = node->val1_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->val2_sequence)
      {
        const list_node<staticScalar_ast*> *__it = node->val2_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_staticMember(staticMember_ast *node)
  {
    visit_node(node->variable);
  }

  void default_visitor::visit_staticScalar(staticScalar_ast *node)
  {
    visit_node(node->value);
    visit_node(node->plusValue);
    visit_node(node->minusValue);
    if (node->arrayValues_sequence)
      {
        const list_node<staticArrayPairValue_ast*> *__it = node->arrayValues_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_staticVar(staticVar_ast *node)
  {
    visit_node(node->staticScalar);
  }

  void default_visitor::visit_switchCaseList(switchCaseList_ast *node)
  {
    visit_node(node->caseList);
  }

  void default_visitor::visit_topStatement(topStatement_ast *node)
  {
    visit_node(node->statement);
    visit_node(node->functionDeclaration);
    visit_node(node->classDeclaration);
  }

  void default_visitor::visit_unaryExpression(unaryExpression_ast *node)
  {
    visit_node(node->unaryExpression);
    visit_node(node->assignmentList);
    visit_node(node->expression);
    visit_node(node->unaryExpressionNotPlusminus);
  }

  void default_visitor::visit_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast *node)
  {
    if (node->prefixOperator_sequence)
      {
        const list_node<postprefixOperator_ast*> *__it = node->prefixOperator_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->varExpression);
    if (node->postfixOperator_sequence)
      {
        const list_node<postprefixOperator_ast*> *__it = node->postfixOperator_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_varExpression(varExpression_ast *node)
  {
    visit_node(node->variable);
    visit_node(node->newObject);
    visit_node(node->varExpressionNormal);
  }

  void default_visitor::visit_varExpressionNewObject(varExpressionNewObject_ast *node)
  {
    visit_node(node->className);
    visit_node(node->ctor);
  }

  void default_visitor::visit_varExpressionNormal(varExpressionNormal_ast *node)
  {
    visit_node(node->expression);
    visit_node(node->encapsList);
    visit_node(node->variable);
    visit_node(node->scalar);
    if (node->arrayValues_sequence)
      {
        const list_node<arrayPairValue_ast*> *__it = node->arrayValues_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->issetVariable_sequence)
      {
        const list_node<variable_ast*> *__it = node->issetVariable_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->emptyVarialbe);
    visit_node(node->newObject);
    visit_node(node->cloneCar);
  }

  void default_visitor::visit_variable(variable_ast *node)
  {
    visit_node(node->var);
    if (node->variableProperties_sequence)
      {
        const list_node<variableProperty_ast*> *__it = node->variableProperties_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_variableName(variableName_ast *node)
  {
    visit_node(node->expr);
  }

  void default_visitor::visit_variableProperty(variableProperty_ast *node)
  {
    visit_node(node->objectProperty);
    visit_node(node->parameterList);
  }

  void default_visitor::visit_variableWithoutObjects(variableWithoutObjects_ast *node)
  {
    visit_node(node->variable);
    if (node->offsetItems_sequence)
      {
        const list_node<dimListItem_ast*> *__it = node->offsetItems_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_whileStatement(whileStatement_ast *node)
  {
    visit_node(node->statement);
    visit_node(node->statements);
  }


} // end of namespace rphp


