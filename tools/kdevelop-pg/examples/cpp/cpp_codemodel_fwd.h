// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cpp_CODEMODEL_FWD_H
#define cpp_CODEMODEL_FWD_H

#include "kdevsharedptr.h"

#include <QtCore/QList>

// forward declarations

class CodeModel;

class TypeInfo;

class _CodeModelItem;

class _ScopeModelItem;

class _ClassModelItem;

class _NamespaceModelItem;

class _FileModelItem;

class _ArgumentModelItem;

class _MemberModelItem;

class _FunctionModelItem;

class _FunctionDefinitionModelItem;

class _VariableModelItem;

class _TypeAliasModelItem;

class _EnumModelItem;

class _EnumeratorModelItem;

class _TemplateParameterModelItem;

class _TemplateModelItem;

typedef KDevSharedPtr<_CodeModelItem> CodeModelItem;
typedef KDevSharedPtr<_ScopeModelItem> ScopeModelItem;
typedef KDevSharedPtr<_ClassModelItem> ClassModelItem;
typedef KDevSharedPtr<_NamespaceModelItem> NamespaceModelItem;
typedef KDevSharedPtr<_FileModelItem> FileModelItem;
typedef KDevSharedPtr<_ArgumentModelItem> ArgumentModelItem;
typedef KDevSharedPtr<_MemberModelItem> MemberModelItem;
typedef KDevSharedPtr<_FunctionModelItem> FunctionModelItem;
typedef KDevSharedPtr<_FunctionDefinitionModelItem> FunctionDefinitionModelItem;
typedef KDevSharedPtr<_VariableModelItem> VariableModelItem;
typedef KDevSharedPtr<_TypeAliasModelItem> TypeAliasModelItem;
typedef KDevSharedPtr<_EnumModelItem> EnumModelItem;
typedef KDevSharedPtr<_EnumeratorModelItem> EnumeratorModelItem;
typedef KDevSharedPtr<_TemplateParameterModelItem> TemplateParameterModelItem;
typedef KDevSharedPtr<_TemplateModelItem> TemplateModelItem;

typedef QList<CodeModelItem> CodeList;
typedef QList<CodeModelItem> ItemList;
typedef QList<ScopeModelItem> ScopeList;
typedef QList<ClassModelItem> ClassList;
typedef QList<NamespaceModelItem> NamespaceList;
typedef QList<FileModelItem> FileList;
typedef QList<ArgumentModelItem> ArgumentList;
typedef QList<MemberModelItem> MemberList;
typedef QList<FunctionModelItem> FunctionList;
typedef QList<FunctionDefinitionModelItem> FunctionDefinitionList;
typedef QList<VariableModelItem> VariableList;
typedef QList<TypeAliasModelItem> TypeAliasList;
typedef QList<EnumModelItem> EnumList;
typedef QList<EnumeratorModelItem> EnumeratorList;
typedef QList<TemplateParameterModelItem> TemplateParameterList;
typedef QList<TemplateModelItem> TemplateList;


#endif


