// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cpp_codemodel.h"


// ---------------------------------------------------------------------------
#define CLASS CodeModel
#define BASECLASS KDevCodeModel

CodeModel::CodeModel( QObject *parent )
    :  KDevCodeModel( parent )
{

  _M_globalNamespace =  create < ITEM(Namespace) > ();
  _M_globalNamespace->setName(QString::null);
}

CodeModel::~CodeModel()
{

  // FIXME jpetso: don't we need "delete _M_globalNamespace;" here?
}

void CodeModel::wipeout()
{}

void CodeModel::addCodeItem(CodeModelItem item)
{
  beginAppendItem(item);
  endAppendItem();
}

void CodeModel::removeCodeItem(CodeModelItem item)
{
  beginRemoveItem(item);
  endRemoveItem();
}

NamespaceModelItem CodeModel::globalNamespace() const
  {
    return  _M_globalNamespace;
  }

KDevItemCollection *CodeModel::root() const
  {
    return  _M_globalNamespace;
  }


ITEM(Code) CLASS::findItem(const QStringList &qualifiedName,  ITEM(Code) scope) const
  {
    for  (int i = 0; i < qualifiedName.size(); ++i)
      {
        // ### Extend to look for members etc too.
        const QString &name =  qualifiedName.at(i);

        if  (ITEM(Namespace) ns =  model_dynamic_cast < ITEM(Namespace) > (scope))
          {
            ITEM(Namespace) tmp_ns =  ns->findNamespace(name);

            if  (tmp_ns)
              {
                scope =  tmp_ns;
                continue;
              }
          }

        if  (ITEM(Scope) ss =  model_dynamic_cast < ITEM(Scope) > (scope))
          {
            if  (ITEM(Class) cs =  ss->findClass(name))
              {
                scope =  cs;
              }

            else if  (ITEM(Enum) es =  ss->findEnum(name))
              {
                Q_ASSERT(i ==  qualifiedName.size() -  1);
                return  es->toItem();
              }
          }
      }

    return  scope;
  }

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS TypeInfo
#define BASECLASS has_no_base_class!

QStringList TypeInfo::qualifiedName() const
  {
    return  _M_qualifiedName;
  }

void TypeInfo::setQualifiedName(QStringList qualifiedName)
{
  _M_qualifiedName =  qualifiedName;
}

bool TypeInfo::isConstant() const
  {
    return  _M_isConstant;
  }

void TypeInfo::setConstant(bool isConstant)
{
  _M_isConstant =  isConstant;
}

bool TypeInfo::isReference() const
  {
    return  _M_isReference;
  }

void TypeInfo::setReference(bool isReference)
{
  _M_isReference =  isReference;
}

int TypeInfo::indirections() const
  {
    return  _M_indirections;
  }

void TypeInfo::setIndirections(int indirections)
{
  _M_indirections =  indirections;
}


QString CLASS::toString() const
  {
    QString tmp;

    if  (isConstant())
      tmp +=  "const ";
    tmp +=  _M_qualifiedName.join("::");
    if  (indirections())
      tmp +=  QString(indirections(),  QLatin1Char('*'));
    if  (isReference())
      tmp +=  QLatin1Char('&');
    return  tmp;
  }

bool CLASS::operator==(const TypeInfo &other)
{
  return  _M_isConstant ==  other._M_isConstant
          &&  _M_isReference ==  other._M_isReference
          &&  _M_indirections ==  other._M_indirections
          &&  _M_qualifiedName ==  other._M_qualifiedName;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _CodeModelItem
#define BASECLASS has_no_base_class!

_CodeModelItem::_CodeModelItem(CodeModel *model,  int kind)
    :  KDevCodeItem( QString::null,  0 )
    ,  _M_model(model)
    ,  _M_kind(kind)
    ,  _M_startLine( -1)
    ,  _M_startColumn( -1)
    ,  _M_endLine( -1)
    ,  _M_endColumn( -1)
{}

_CodeModelItem::~_CodeModelItem()
{}

_CodeModelItem *_CodeModelItem::itemAt(int index) const
  {
    return  static_cast<_CodeModelItem*>(KDevItemCollection::itemAt(index));
  }

CodeModelItem _CodeModelItem::toItem() const
  {
    return  CodeModelItem(const_cast<_CodeModelItem*>(this));
  }

int _CodeModelItem::kind() const
  {
    return  _M_kind;
  }

void _CodeModelItem::setKind(int kind)
{
  _M_kind =  kind;
}

KTextEditor::Cursor _CodeModelItem::startPosition() const
  {
    return  KTextEditor::Cursor(_M_startLine,  _M_startColumn);
  }

void _CodeModelItem::setStartPosition(const KTextEditor::Cursor& cursor)
{
  _M_startLine =  cursor.line();
  _M_startColumn =  cursor.column();
}

KTextEditor::Cursor _CodeModelItem::endPosition() const
  {
    return  KTextEditor::Cursor(_M_endLine,  _M_endColumn);
  }

void _CodeModelItem::setEndPosition(const KTextEditor::Cursor& cursor)
{
  _M_endLine =  cursor.line();
  _M_endColumn =  cursor.column();
}

QString _CodeModelItem::name() const
  {
    return  _M_name;
  }

void _CodeModelItem::setName(QString name)
{
  _M_name =  name;
}

QStringList _CodeModelItem::scope() const
  {
    return  _M_scope;
  }

void _CodeModelItem::setScope(QStringList scope)
{
  _M_scope =  scope;
}

QString _CodeModelItem::fileName() const
  {
    return  _M_fileName;
  }

void _CodeModelItem::setFileName(QString fileName)
{
  _M_fileName =  fileName;
}


QStringList CLASS::qualifiedName() const
  {
    QStringList q =  scope();

    if  (!name().isEmpty())
      q +=  name();

    return  q;
  }

///TODO These functions should be cached upon initialization
QString CLASS::display() const
  {
    return  CodeDisplay::display( const_cast<const CLASS*>( this ) );
  }

QIcon CLASS::decoration() const
  {
    return  CodeDisplay::decoration( const_cast<const CLASS*>( this ) );
  }

QString CLASS::toolTip() const
  {
    return  CodeDisplay::toolTip( const_cast<const CLASS*>( this ) );
  }

QString CLASS::whatsThis() const
  {
    return  CodeDisplay::whatsThis( const_cast<const CLASS*>( this ) );
  }

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _ScopeModelItem
#define BASECLASS _CodeModelItem

_ScopeModelItem::_ScopeModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
{}

_ScopeModelItem::~_ScopeModelItem()
{}

ScopeModelItem _ScopeModelItem::create(CodeModel *model)
{
  ScopeModelItem item(new _ScopeModelItem(model));
  return  item;
}

ClassList _ScopeModelItem::classes() const
  {
    return  _M_classes.values();
  }

void _ScopeModelItem::addClass(ClassModelItem item)
{
  if  ( _M_classes.contains(item->name()) )
    removeClass(_M_classes[item->name()]);

  model()->beginAppendItem(item,  this);
  _M_classes.insertMulti(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeClass(ClassModelItem item)
{
  model()->beginRemoveItem(item);
  _M_classes.remove(item->name());
  model()->endRemoveItem();
}

ClassModelItem _ScopeModelItem::findClass(const QString &name) const
  {
    return  _M_classes.value(name);
  }

EnumList _ScopeModelItem::enums() const
  {
    return  _M_enums.values();
  }

void _ScopeModelItem::addEnum(EnumModelItem item)
{
  if  ( _M_enums.contains(item->name()) )
    removeEnum(_M_enums[item->name()]);

  model()->beginAppendItem(item,  this);
  _M_enums.insertMulti(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeEnum(EnumModelItem item)
{
  model()->beginRemoveItem(item);
  _M_enums.remove(item->name());
  model()->endRemoveItem();
}

EnumModelItem _ScopeModelItem::findEnum(const QString &name) const
  {
    return  _M_enums.value(name);
  }

FunctionDefinitionList _ScopeModelItem::functionDefinitions() const
  {
    return  _M_functionDefinitions.values();
  }

void _ScopeModelItem::addFunctionDefinition(FunctionDefinitionModelItem item)
{
  foreach( _FunctionDefinitionModelItem *existingItem,
           _M_functionDefinitions.values(item->name()) )

  if  (!(  !existingItem->isSimilar( model_static_cast < ITEM(Code) > (item) )  ))
    removeFunctionDefinition(existingItem);
  model()->beginAppendItem(item,  this);
  _M_functionDefinitions.insert(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeFunctionDefinition(FunctionDefinitionModelItem item)
{
  QMultiHash<QString,  FunctionDefinitionModelItem>::Iterator it =  _M_functionDefinitions.find(item->name());

  while  (it !=  _M_functionDefinitions.end() &&  it.key() ==  item->name()
          &&  it.value() !=  item)
    {
      ++it;
    }

  if  (it !=  _M_functionDefinitions.end() &&  it.value() ==  item)
    {
      model()->beginRemoveItem(item);
      _M_functionDefinitions.erase(it);
      model()->endRemoveItem();
    }
}

FunctionDefinitionList _ScopeModelItem::findFunctionDefinitions(const QString &name) const
  {
    return  _M_functionDefinitions.values(name);
  }

FunctionList _ScopeModelItem::functions() const
  {
    return  _M_functions.values();
  }

void _ScopeModelItem::addFunction(FunctionModelItem item)
{
  foreach( _FunctionModelItem *existingItem,
           _M_functions.values(item->name()) )

  if  (!(  !existingItem->isSimilar( model_static_cast < ITEM(Code) > (item) )  ))
    removeFunction(existingItem);
  model()->beginAppendItem(item,  this);
  _M_functions.insert(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeFunction(FunctionModelItem item)
{
  QMultiHash<QString,  FunctionModelItem>::Iterator it =  _M_functions.find(item->name());

  while  (it !=  _M_functions.end() &&  it.key() ==  item->name()
          &&  it.value() !=  item)
    {
      ++it;
    }

  if  (it !=  _M_functions.end() &&  it.value() ==  item)
    {
      model()->beginRemoveItem(item);
      _M_functions.erase(it);
      model()->endRemoveItem();
    }
}

FunctionList _ScopeModelItem::findFunctions(const QString &name) const
  {
    return  _M_functions.values(name);
  }

TypeAliasList _ScopeModelItem::typeAliases() const
  {
    return  _M_typeAliases.values();
  }

void _ScopeModelItem::addTypeAlias(TypeAliasModelItem item)
{
  if  ( _M_typeAliases.contains(item->name()) )
    removeTypeAlias(_M_typeAliases[item->name()]);

  model()->beginAppendItem(item,  this);
  _M_typeAliases.insertMulti(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeTypeAlias(TypeAliasModelItem item)
{
  model()->beginRemoveItem(item);
  _M_typeAliases.remove(item->name());
  model()->endRemoveItem();
}

TypeAliasModelItem _ScopeModelItem::findTypeAlias(const QString &name) const
  {
    return  _M_typeAliases.value(name);
  }

VariableList _ScopeModelItem::variables() const
  {
    return  _M_variables.values();
  }

void _ScopeModelItem::addVariable(VariableModelItem item)
{
  if  ( _M_variables.contains(item->name()) )
    removeVariable(_M_variables[item->name()]);

  model()->beginAppendItem(item,  this);
  _M_variables.insertMulti(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeVariable(VariableModelItem item)
{
  model()->beginRemoveItem(item);
  _M_variables.remove(item->name());
  model()->endRemoveItem();
}

VariableModelItem _ScopeModelItem::findVariable(const QString &name) const
  {
    return  _M_variables.value(name);
  }

TemplateList _ScopeModelItem::templates() const
  {
    return  _M_templates.values();
  }

void _ScopeModelItem::addTemplate(TemplateModelItem item)
{
  foreach( _TemplateModelItem *existingItem,
           _M_templates.values(item->name()) )
  removeTemplate(existingItem);
  model()->beginAppendItem(item,  this);
  _M_templates.insert(item->name(),  item);
  model()->endAppendItem();
}

void _ScopeModelItem::removeTemplate(TemplateModelItem item)
{
  QMultiHash<QString,  TemplateModelItem>::Iterator it =  _M_templates.find(item->name());

  while  (it !=  _M_templates.end() &&  it.key() ==  item->name()
          &&  it.value() !=  item)
    {
      ++it;
    }

  if  (it !=  _M_templates.end() &&  it.value() ==  item)
    {
      model()->beginRemoveItem(item);
      _M_templates.erase(it);
      model()->endRemoveItem();
    }
}

TemplateList _ScopeModelItem::findTemplates(const QString &name) const
  {
    return  _M_templates.values(name);
  }


ITEM(Function) CLASS::declaredFunction(ITEM(Function) item)
{
  LIST(Function) function_list =  findFunctions(item->name());

  foreach (ITEM(Function) fun,  function_list)
  {
    if  (fun->isSimilar(model_static_cast < ITEM(Code) > (item), false))
      return  fun;
  }

  return  ITEM(Function)();
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _ClassModelItem
#define BASECLASS _ScopeModelItem

_ClassModelItem::_ClassModelItem(CodeModel *model,  int kind)
    :  _ScopeModelItem(model,  kind)
    ,  _M_classType( CodeModel::Class )
{}

_ClassModelItem::~_ClassModelItem()
{}

ClassModelItem _ClassModelItem::create(CodeModel *model)
{
  ClassModelItem item(new _ClassModelItem(model));
  return  item;
}

QStringList _ClassModelItem::baseClasses() const
  {
    return  _M_baseClasses;
  }

void _ClassModelItem::setBaseClasses(QStringList baseClasses)
{
  _M_baseClasses =  baseClasses;
}

CodeModel::ClassType _ClassModelItem::classType() const
  {
    return  _M_classType;
  }

void _ClassModelItem::setClassType(CodeModel::ClassType classType)
{
  _M_classType =  classType;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _NamespaceModelItem
#define BASECLASS _ScopeModelItem

_NamespaceModelItem::_NamespaceModelItem(CodeModel *model,  int kind)
    :  _ScopeModelItem(model,  kind)
{}

_NamespaceModelItem::~_NamespaceModelItem()
{}

NamespaceModelItem _NamespaceModelItem::create(CodeModel *model)
{
  NamespaceModelItem item(new _NamespaceModelItem(model));
  return  item;
}

NamespaceList _NamespaceModelItem::namespaces() const
  {
    return  _M_namespaces.values();
  }

void _NamespaceModelItem::addNamespace(NamespaceModelItem item)
{
  if  ( _M_namespaces.contains(item->name()) )
    removeNamespace(_M_namespaces[item->name()]);

  model()->beginAppendItem(item,  this);
  _M_namespaces.insertMulti(item->name(),  item);
  model()->endAppendItem();
}

void _NamespaceModelItem::removeNamespace(NamespaceModelItem item)
{
  model()->beginRemoveItem(item);
  _M_namespaces.remove(item->name());
  model()->endRemoveItem();
}

NamespaceModelItem _NamespaceModelItem::findNamespace(const QString &name) const
  {
    return  _M_namespaces.value(name);
  }

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _FileModelItem
#define BASECLASS _NamespaceModelItem

_FileModelItem::_FileModelItem(CodeModel *model,  int kind)
    :  _NamespaceModelItem(model,  kind)
{}

_FileModelItem::~_FileModelItem()
{}

FileModelItem _FileModelItem::create(CodeModel *model)
{
  FileModelItem item(new _FileModelItem(model));
  return  item;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _ArgumentModelItem
#define BASECLASS _CodeModelItem

_ArgumentModelItem::_ArgumentModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
    ,  _M_defaultValue( false )
{}

_ArgumentModelItem::~_ArgumentModelItem()
{}

ArgumentModelItem _ArgumentModelItem::create(CodeModel *model)
{
  ArgumentModelItem item(new _ArgumentModelItem(model));
  return  item;
}

TypeInfo _ArgumentModelItem::type() const
  {
    return  _M_type;
  }

void _ArgumentModelItem::setType(TypeInfo type)
{
  _M_type =  type;
}

bool _ArgumentModelItem::defaultValue() const
  {
    return  _M_defaultValue;
  }

void _ArgumentModelItem::setDefaultValue(bool defaultValue)
{
  _M_defaultValue =  defaultValue;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _MemberModelItem
#define BASECLASS _CodeModelItem

_MemberModelItem::_MemberModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
    ,  _M_isConstant( false )
    ,  _M_isVolatile( false )
    ,  _M_isStatic( false )
    ,  _M_isAuto( false )
    ,  _M_isFriend( false )
    ,  _M_isRegister( false )
    ,  _M_isExtern( false )
    ,  _M_isMutable( false )
    ,  _M_accessPolicy( CodeModel::Public )
{}

_MemberModelItem::~_MemberModelItem()
{}

MemberModelItem _MemberModelItem::create(CodeModel *model)
{
  MemberModelItem item(new _MemberModelItem(model));
  return  item;
}

bool _MemberModelItem::isConstant() const
  {
    return  _M_isConstant;
  }

void _MemberModelItem::setConstant(bool isConstant)
{
  _M_isConstant =  isConstant;
}

bool _MemberModelItem::isVolatile() const
  {
    return  _M_isVolatile;
  }

void _MemberModelItem::setVolatile(bool isVolatile)
{
  _M_isVolatile =  isVolatile;
}

bool _MemberModelItem::isStatic() const
  {
    return  _M_isStatic;
  }

void _MemberModelItem::setStatic(bool isStatic)
{
  _M_isStatic =  isStatic;
}

bool _MemberModelItem::isAuto() const
  {
    return  _M_isAuto;
  }

void _MemberModelItem::setAuto(bool isAuto)
{
  _M_isAuto =  isAuto;
}

bool _MemberModelItem::isFriend() const
  {
    return  _M_isFriend;
  }

void _MemberModelItem::setFriend(bool isFriend)
{
  _M_isFriend =  isFriend;
}

bool _MemberModelItem::isRegister() const
  {
    return  _M_isRegister;
  }

void _MemberModelItem::setRegister(bool isRegister)
{
  _M_isRegister =  isRegister;
}

bool _MemberModelItem::isExtern() const
  {
    return  _M_isExtern;
  }

void _MemberModelItem::setExtern(bool isExtern)
{
  _M_isExtern =  isExtern;
}

bool _MemberModelItem::isMutable() const
  {
    return  _M_isMutable;
  }

void _MemberModelItem::setMutable(bool isMutable)
{
  _M_isMutable =  isMutable;
}

CodeModel::AccessPolicy _MemberModelItem::accessPolicy() const
  {
    return  _M_accessPolicy;
  }

void _MemberModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy)
{
  _M_accessPolicy =  accessPolicy;
}

TypeInfo _MemberModelItem::type() const
  {
    return  _M_type;
  }

void _MemberModelItem::setType(TypeInfo type)
{
  _M_type =  type;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _FunctionModelItem
#define BASECLASS _MemberModelItem

_FunctionModelItem::_FunctionModelItem(CodeModel *model,  int kind)
    :  _MemberModelItem(model,  kind)
    ,  _M_functionType( CodeModel::Normal )
    ,  _M_isVirtual( false )
    ,  _M_isInline( false )
    ,  _M_isExplicit( false )
    ,  _M_isAbstract( false )
{}

_FunctionModelItem::~_FunctionModelItem()
{}

FunctionModelItem _FunctionModelItem::create(CodeModel *model)
{
  FunctionModelItem item(new _FunctionModelItem(model));
  return  item;
}

ArgumentList _FunctionModelItem::arguments() const
  {
    return  _M_arguments;
  }

void _FunctionModelItem::addArgument(ArgumentModelItem item)
{
  model()->beginAppendItem(item,  this);
  _M_arguments.append(item);
  model()->endAppendItem();
}

void _FunctionModelItem::removeArgument(ArgumentModelItem item)
{
  model()->beginRemoveItem(item);
  _M_arguments.removeAt(_M_arguments.indexOf(item));
  model()->endRemoveItem();
}

CodeModel::FunctionType _FunctionModelItem::functionType() const
  {
    return  _M_functionType;
  }

void _FunctionModelItem::setFunctionType(CodeModel::FunctionType functionType)
{
  _M_functionType =  functionType;
}

bool _FunctionModelItem::isVirtual() const
  {
    return  _M_isVirtual;
  }

void _FunctionModelItem::setVirtual(bool isVirtual)
{
  _M_isVirtual =  isVirtual;
}

bool _FunctionModelItem::isInline() const
  {
    return  _M_isInline;
  }

void _FunctionModelItem::setInline(bool isInline)
{
  _M_isInline =  isInline;
}

bool _FunctionModelItem::isExplicit() const
  {
    return  _M_isExplicit;
  }

void _FunctionModelItem::setExplicit(bool isExplicit)
{
  _M_isExplicit =  isExplicit;
}

bool _FunctionModelItem::isAbstract() const
  {
    return  _M_isAbstract;
  }

void _FunctionModelItem::setAbstract(bool isAbstract)
{
  _M_isAbstract =  isAbstract;
}


bool CLASS::isConstructor() const
  {
    ///TODO cache this information upon initialization
    return  scope().isEmpty() ?  false :  scope().last() ==  name();
  }

bool CLASS::isDestructor() const
  {
    ///TODO cache this information upon initialization
    return  name().startsWith('~');
  }

bool CLASS::isSimilar(KDevCodeItem *other,  bool strict ) const
  {
    if  (!BASECLASS::isSimilar(other, strict))
      return  false;

    ITEM(Function) func =  dynamic_cast<CLASS*>(other);

    if  (isConstant() !=  func->isConstant())
      return  false;

    if  (arguments().count() !=  func->arguments().count())
      return  false;

    for  (int i = 0; i < arguments().count(); ++i)
      {
        ITEM(Argument) arg1 =  arguments().at(i);
        ITEM(Argument) arg2 =  arguments().at(i);

        if  (arg1->type() !=  arg2->type())
          return  false;
      }

    return  true;
  }

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _FunctionDefinitionModelItem
#define BASECLASS _FunctionModelItem

_FunctionDefinitionModelItem::_FunctionDefinitionModelItem(CodeModel *model,  int kind)
    :  _FunctionModelItem(model,  kind)
{}

_FunctionDefinitionModelItem::~_FunctionDefinitionModelItem()
{}

FunctionDefinitionModelItem _FunctionDefinitionModelItem::create(CodeModel *model)
{
  FunctionDefinitionModelItem item(new _FunctionDefinitionModelItem(model));
  return  item;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _VariableModelItem
#define BASECLASS _MemberModelItem

_VariableModelItem::_VariableModelItem(CodeModel *model,  int kind)
    :  _MemberModelItem(model,  kind)
{}

_VariableModelItem::~_VariableModelItem()
{}

VariableModelItem _VariableModelItem::create(CodeModel *model)
{
  VariableModelItem item(new _VariableModelItem(model));
  return  item;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _TypeAliasModelItem
#define BASECLASS _CodeModelItem

_TypeAliasModelItem::_TypeAliasModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
{}

_TypeAliasModelItem::~_TypeAliasModelItem()
{}

TypeAliasModelItem _TypeAliasModelItem::create(CodeModel *model)
{
  TypeAliasModelItem item(new _TypeAliasModelItem(model));
  return  item;
}

TypeInfo _TypeAliasModelItem::type() const
  {
    return  _M_type;
  }

void _TypeAliasModelItem::setType(TypeInfo type)
{
  _M_type =  type;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _EnumModelItem
#define BASECLASS _CodeModelItem

_EnumModelItem::_EnumModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
    ,  _M_accessPolicy( CodeModel::Public )
{}

_EnumModelItem::~_EnumModelItem()
{}

EnumModelItem _EnumModelItem::create(CodeModel *model)
{
  EnumModelItem item(new _EnumModelItem(model));
  return  item;
}

CodeModel::AccessPolicy _EnumModelItem::accessPolicy() const
  {
    return  _M_accessPolicy;
  }

void _EnumModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy)
{
  _M_accessPolicy =  accessPolicy;
}

EnumeratorList _EnumModelItem::enumerators() const
  {
    return  _M_enumerators;
  }

void _EnumModelItem::addEnumerator(EnumeratorModelItem item)
{
  model()->beginAppendItem(item,  this);
  _M_enumerators.append(item);
  model()->endAppendItem();
}

void _EnumModelItem::removeEnumerator(EnumeratorModelItem item)
{
  model()->beginRemoveItem(item);
  _M_enumerators.removeAt(_M_enumerators.indexOf(item));
  model()->endRemoveItem();
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _EnumeratorModelItem
#define BASECLASS _CodeModelItem

_EnumeratorModelItem::_EnumeratorModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
{}

_EnumeratorModelItem::~_EnumeratorModelItem()
{}

EnumeratorModelItem _EnumeratorModelItem::create(CodeModel *model)
{
  EnumeratorModelItem item(new _EnumeratorModelItem(model));
  return  item;
}

QString _EnumeratorModelItem::value() const
  {
    return  _M_value;
  }

void _EnumeratorModelItem::setValue(QString value)
{
  _M_value =  value;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _TemplateParameterModelItem
#define BASECLASS _CodeModelItem

_TemplateParameterModelItem::_TemplateParameterModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
    ,  _M_defaultValue( false )
{}

_TemplateParameterModelItem::~_TemplateParameterModelItem()
{}

TemplateParameterModelItem _TemplateParameterModelItem::create(CodeModel *model)
{
  TemplateParameterModelItem item(new _TemplateParameterModelItem(model));
  return  item;
}

TypeInfo _TemplateParameterModelItem::type() const
  {
    return  _M_type;
  }

void _TemplateParameterModelItem::setType(TypeInfo type)
{
  _M_type =  type;
}

bool _TemplateParameterModelItem::defaultValue() const
  {
    return  _M_defaultValue;
  }

void _TemplateParameterModelItem::setDefaultValue(bool defaultValue)
{
  _M_defaultValue =  defaultValue;
}

#undef CLASS
#undef BASECLASS

// ---------------------------------------------------------------------------
#define CLASS _TemplateModelItem
#define BASECLASS _CodeModelItem

_TemplateModelItem::_TemplateModelItem(CodeModel *model,  int kind)
    :  _CodeModelItem(model,  kind)
{}

_TemplateModelItem::~_TemplateModelItem()
{}

TemplateModelItem _TemplateModelItem::create(CodeModel *model)
{
  TemplateModelItem item(new _TemplateModelItem(model));
  return  item;
}

TemplateParameterList _TemplateModelItem::parameters() const
  {
    return  _M_parameters;
  }

void _TemplateModelItem::addParameter(TemplateParameterModelItem item)
{
  model()->beginAppendItem(item,  this);
  _M_parameters.append(item);
  model()->endAppendItem();
}

void _TemplateModelItem::removeParameter(TemplateParameterModelItem item)
{
  model()->beginRemoveItem(item);
  _M_parameters.removeAt(_M_parameters.indexOf(item));
  model()->endRemoveItem();
}

CodeModelItem _TemplateModelItem::declaration() const
  {
    return  _M_declaration;
  }

void _TemplateModelItem::setDeclaration(CodeModelItem declaration)
{
  _M_declaration =  declaration;
}

#undef CLASS
#undef BASECLASS


