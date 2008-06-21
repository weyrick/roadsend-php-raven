// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cpp_CODEMODEL_H
#define cpp_CODEMODEL_H

#include "kdevcodemodel.h"

#include "cpp_codemodel_fwd.h"

#include <QtCore/QTime>
#include <QtCore/QHash>
#include <QtCore/QList>

#include <ktexteditor/cursor.h>


#include "codedisplay.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <kurl.h>

#define DECLARE_MODEL_NODE(k) \
enum { __node_kind = Kind_##k }; \
typedef KDevSharedPtr<k##ModelItem> Pointer;

#define ITEM(item) item##ModelItem
#define LIST(item) item##List

template  <class _Target,  class _Source>
_Target model_static_cast(_Source item)
{
  typedef typename _Target::Type * _Target_pointer;

  _Target ptr =  static_cast<_Target_pointer>(item.data());
  return  ptr;
}

class CodeModel :  public KDevCodeModel
  {

  public:
    CodeModel( QObject *parent =  0 );
    virtual ~CodeModel();

    template  <class _Target>
    _Target create()
    {
      typedef typename _Target::Type _Target_type;

      _Target result =  _Target_type::create(this);
      return  result;
    }

    void addCodeItem(CodeModelItem item);
    void removeCodeItem(CodeModelItem item);

    void wipeout();

  public:
    NamespaceModelItem globalNamespace() const;
    KDevItemCollection *root() const;

  private:
    NamespaceModelItem _M_globalNamespace;

  public:
    enum AccessPolicy
    {
      Public,
      Protected,
      Private
    };
    enum FunctionType
    {
      Normal,
      Signal,
      Slot
    };
    enum ClassType
    {
      Class,
      Struct,
      Union
    };

    ITEM(Code) findItem(const QStringList &qualifiedName,  ITEM(Code) scope) const;

  private:
    CodeModel(const CodeModel &other);
    void operator=(const CodeModel &other);
  };

class TypeInfo
  {

  public:
    bool operator==(const TypeInfo &other);
    bool operator!=(const TypeInfo &other)
    {
      return  !(*this == other);
    }

    // ### arrays and templates??

    QString toString() const;

  public:
    QStringList qualifiedName() const;
    void setQualifiedName(QStringList qualifiedName);

    bool isConstant() const;
    void setConstant(bool isConstant);

    bool isReference() const;
    void setReference(bool isReference);

    int indirections() const;
    void setIndirections(int indirections);

  private:
    QStringList _M_qualifiedName;
    bool _M_isConstant;
    bool _M_isReference;
    int _M_indirections;
  };

class _CodeModelItem :  public KDevCodeItem
  {

  public:
    enum Kind
    {
      /* These are bit-flags resembling inheritance */
      Kind_Scope =  1,
      Kind_Namespace =  1 << 1 /*| Kind_Scope*/,
      Kind_Member =  1 << 2,
      Kind_Function =  1 << 3 /*| Kind_Member*/,
      KindMask =  (1 << 4) -  1,

      /* These are for classes that are not inherited from */
      FirstKind =  1 << 4,
      Kind_Class =  1 << 5 /*| Kind_Scope*/,
      Kind_File =  1 << 6 /*| Kind_Namespace*/,
      Kind_Argument =  1 << 7,
      Kind_FunctionDefinition =  1 << 8 /*| Kind_Function*/,
      Kind_Variable =  1 << 9 /*| Kind_Member*/,
      Kind_TypeAlias =  1 << 10,
      Kind_Enum =  1 << 11,
      Kind_Enumerator =  1 << 12,
      Kind_TemplateParameter =  1 << 13,
      Kind_Template =  1 << 14,
    };

  public:
    virtual ~_CodeModelItem();

    virtual _CodeModelItem *itemAt(int index) const;

    int kind() const;

    KTextEditor::Cursor startPosition() const;
    void setStartPosition(const KTextEditor::Cursor& cursor);

    KTextEditor::Cursor endPosition() const;
    void setEndPosition(const KTextEditor::Cursor& cursor);

    QTime timestamp() const
      {
        return  QTime();
      }

    inline CodeModel *model() const
      {
        return  _M_model;
      }

    CodeModelItem toItem() const;

  private:
    CodeModel *_M_model;
    int _M_kind;
    int _M_startLine;
    int _M_startColumn;
    int _M_endLine;
    int _M_endColumn;

  public:
    QStringList qualifiedName() const;

    ///TODO These functions should be cached upon initialization
    QString display() const;
    QIcon decoration() const;
    QString toolTip() const;
    QString whatsThis() const;

  public:
    QString name() const;
    void setName(QString name);

    QStringList scope() const;
    void setScope(QStringList scope);

    QString fileName() const;
    void setFileName(QString fileName);

  private:
    QString _M_name;
    QStringList _M_scope;
    QString _M_fileName;

  protected:
    _CodeModelItem(CodeModel *model,  int kind);
    void setKind(int kind);

  private:
    _CodeModelItem(const _CodeModelItem &other);
    void operator=(const _CodeModelItem &other);
  };

class _ScopeModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Scope)

    static ScopeModelItem create(CodeModel *model);
    virtual ~_ScopeModelItem();

  public:
    ITEM(Function) declaredFunction(ITEM(Function) item);

  public:
    ClassList classes() const;
    void addClass(ClassModelItem item);
    void removeClass(ClassModelItem item);
    ClassModelItem findClass(const QString &name) const;

    EnumList enums() const;
    void addEnum(EnumModelItem item);
    void removeEnum(EnumModelItem item);
    EnumModelItem findEnum(const QString &name) const;

    FunctionDefinitionList functionDefinitions() const;
    void addFunctionDefinition(FunctionDefinitionModelItem item);
    void removeFunctionDefinition(FunctionDefinitionModelItem item);
    FunctionDefinitionList findFunctionDefinitions(const QString &name) const;

    FunctionList functions() const;
    void addFunction(FunctionModelItem item);
    void removeFunction(FunctionModelItem item);
    FunctionList findFunctions(const QString &name) const;

    TypeAliasList typeAliases() const;
    void addTypeAlias(TypeAliasModelItem item);
    void removeTypeAlias(TypeAliasModelItem item);
    TypeAliasModelItem findTypeAlias(const QString &name) const;

    VariableList variables() const;
    void addVariable(VariableModelItem item);
    void removeVariable(VariableModelItem item);
    VariableModelItem findVariable(const QString &name) const;

    TemplateList templates() const;
    void addTemplate(TemplateModelItem item);
    void removeTemplate(TemplateModelItem item);
    TemplateList findTemplates(const QString &name) const;

    inline QHash<QString,  ClassModelItem> classMap() const
      {
        return  _M_classes;
      }

    inline QHash<QString,  EnumModelItem> enumMap() const
      {
        return  _M_enums;
      }

    inline QMultiHash<QString,  FunctionDefinitionModelItem> functionDefinitionMap() const
      {
        return  _M_functionDefinitions;
      }

    inline QMultiHash<QString,  FunctionModelItem> functionMap() const
      {
        return  _M_functions;
      }

    inline QHash<QString,  TypeAliasModelItem> typeAliasMap() const
      {
        return  _M_typeAliases;
      }

    inline QHash<QString,  VariableModelItem> variableMap() const
      {
        return  _M_variables;
      }

    inline QMultiHash<QString,  TemplateModelItem> templateMap() const
      {
        return  _M_templates;
      }

  private:
    QHash<QString,  ClassModelItem> _M_classes;
    QHash<QString,  EnumModelItem> _M_enums;
    QMultiHash<QString,  FunctionDefinitionModelItem> _M_functionDefinitions;
    QMultiHash<QString,  FunctionModelItem> _M_functions;
    QHash<QString,  TypeAliasModelItem> _M_typeAliases;
    QHash<QString,  VariableModelItem> _M_variables;
    QMultiHash<QString,  TemplateModelItem> _M_templates;

  protected:
    _ScopeModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _ScopeModelItem(const _ScopeModelItem &other);
    void operator=(const _ScopeModelItem &other);
  };

class _ClassModelItem :  public _ScopeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Class)

    static ClassModelItem create(CodeModel *model);
    virtual ~_ClassModelItem();

  public:
    void addBaseClass(const QString &baseClass);
    void removeBaseClass(const QString &baseClass);

    bool extendsClass(const QString &name) const;

  public:
    QStringList baseClasses() const;
    void setBaseClasses(QStringList baseClasses);

    CodeModel::ClassType classType() const;
    void setClassType(CodeModel::ClassType classType);

  private:
    QStringList _M_baseClasses;
    CodeModel::ClassType _M_classType;

  protected:
    _ClassModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _ClassModelItem(const _ClassModelItem &other);
    void operator=(const _ClassModelItem &other);
  };

class _NamespaceModelItem :  public _ScopeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Namespace)

    static NamespaceModelItem create(CodeModel *model);
    virtual ~_NamespaceModelItem();

  public:
    NamespaceList namespaces() const;
    void addNamespace(NamespaceModelItem item);
    void removeNamespace(NamespaceModelItem item);
    NamespaceModelItem findNamespace(const QString &name) const;

    inline QHash<QString,  NamespaceModelItem> namespaceMap() const
      {
        return  _M_namespaces;
      }

  private:
    QHash<QString,  NamespaceModelItem> _M_namespaces;

  protected:
    _NamespaceModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _NamespaceModelItem(const _NamespaceModelItem &other);
    void operator=(const _NamespaceModelItem &other);
  };

class _FileModelItem :  public _NamespaceModelItem
  {

  public:
    DECLARE_MODEL_NODE(File)

    static FileModelItem create(CodeModel *model);
    virtual ~_FileModelItem();

  public:

  private:

  protected:
    _FileModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _FileModelItem(const _FileModelItem &other);
    void operator=(const _FileModelItem &other);
  };

class _ArgumentModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Argument)

    static ArgumentModelItem create(CodeModel *model);
    virtual ~_ArgumentModelItem();

  public:
    TypeInfo type() const;
    void setType(TypeInfo type);

    bool defaultValue() const;
    void setDefaultValue(bool defaultValue);

  private:
    TypeInfo _M_type;
    bool _M_defaultValue;

  protected:
    _ArgumentModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _ArgumentModelItem(const _ArgumentModelItem &other);
    void operator=(const _ArgumentModelItem &other);
  };

class _MemberModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Member)

    static MemberModelItem create(CodeModel *model);
    virtual ~_MemberModelItem();

  public:
    bool isConstant() const;
    void setConstant(bool isConstant);

    bool isVolatile() const;
    void setVolatile(bool isVolatile);

    bool isStatic() const;
    void setStatic(bool isStatic);

    bool isAuto() const;
    void setAuto(bool isAuto);

    bool isFriend() const;
    void setFriend(bool isFriend);

    bool isRegister() const;
    void setRegister(bool isRegister);

    bool isExtern() const;
    void setExtern(bool isExtern);

    bool isMutable() const;
    void setMutable(bool isMutable);

    CodeModel::AccessPolicy accessPolicy() const;
    void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);

    TypeInfo type() const;
    void setType(TypeInfo type);

  private:
    bool _M_isConstant;
    bool _M_isVolatile;
    bool _M_isStatic;
    bool _M_isAuto;
    bool _M_isFriend;
    bool _M_isRegister;
    bool _M_isExtern;
    bool _M_isMutable;
    CodeModel::AccessPolicy _M_accessPolicy;
    TypeInfo _M_type;

  protected:
    _MemberModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _MemberModelItem(const _MemberModelItem &other);
    void operator=(const _MemberModelItem &other);
  };

class _FunctionModelItem :  public _MemberModelItem
  {

  public:
    DECLARE_MODEL_NODE(Function)

    static FunctionModelItem create(CodeModel *model);
    virtual ~_FunctionModelItem();

  public:
    bool isConstructor() const;
    bool isDestructor() const;

    bool isSimilar(KDevCodeItem *other,  bool strict =  true) const;

  private:
    mutable bool _M_isConstructor;
    mutable bool _M_isDestructor;

  public:
    ArgumentList arguments() const;
    void addArgument(ArgumentModelItem item);
    void removeArgument(ArgumentModelItem item);

    CodeModel::FunctionType functionType() const;
    void setFunctionType(CodeModel::FunctionType functionType);

    bool isVirtual() const;
    void setVirtual(bool isVirtual);

    bool isInline() const;
    void setInline(bool isInline);

    bool isExplicit() const;
    void setExplicit(bool isExplicit);

    bool isAbstract() const;
    void setAbstract(bool isAbstract);

  private:
    ArgumentList _M_arguments;
    CodeModel::FunctionType _M_functionType;
    bool _M_isVirtual;
    bool _M_isInline;
    bool _M_isExplicit;
    bool _M_isAbstract;

  protected:
    _FunctionModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _FunctionModelItem(const _FunctionModelItem &other);
    void operator=(const _FunctionModelItem &other);
  };

class _FunctionDefinitionModelItem :  public _FunctionModelItem
  {

  public:
    DECLARE_MODEL_NODE(FunctionDefinition)

    static FunctionDefinitionModelItem create(CodeModel *model);
    virtual ~_FunctionDefinitionModelItem();

  public:

  private:

  protected:
    _FunctionDefinitionModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _FunctionDefinitionModelItem(const _FunctionDefinitionModelItem &other);
    void operator=(const _FunctionDefinitionModelItem &other);
  };

class _VariableModelItem :  public _MemberModelItem
  {

  public:
    DECLARE_MODEL_NODE(Variable)

    static VariableModelItem create(CodeModel *model);
    virtual ~_VariableModelItem();

  public:

  private:

  protected:
    _VariableModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _VariableModelItem(const _VariableModelItem &other);
    void operator=(const _VariableModelItem &other);
  };

class _TypeAliasModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(TypeAlias)

    static TypeAliasModelItem create(CodeModel *model);
    virtual ~_TypeAliasModelItem();

  public:
    TypeInfo type() const;
    void setType(TypeInfo type);

  private:
    TypeInfo _M_type;

  protected:
    _TypeAliasModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _TypeAliasModelItem(const _TypeAliasModelItem &other);
    void operator=(const _TypeAliasModelItem &other);
  };

class _EnumModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Enum)

    static EnumModelItem create(CodeModel *model);
    virtual ~_EnumModelItem();

  public:
    CodeModel::AccessPolicy accessPolicy() const;
    void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);

    EnumeratorList enumerators() const;
    void addEnumerator(EnumeratorModelItem item);
    void removeEnumerator(EnumeratorModelItem item);

  private:
    CodeModel::AccessPolicy _M_accessPolicy;
    EnumeratorList _M_enumerators;

  protected:
    _EnumModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _EnumModelItem(const _EnumModelItem &other);
    void operator=(const _EnumModelItem &other);
  };

class _EnumeratorModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Enumerator)

    static EnumeratorModelItem create(CodeModel *model);
    virtual ~_EnumeratorModelItem();

  public:
    QString value() const;
    void setValue(QString value);

  private:
    QString _M_value;

  protected:
    _EnumeratorModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _EnumeratorModelItem(const _EnumeratorModelItem &other);
    void operator=(const _EnumeratorModelItem &other);
  };

class _TemplateParameterModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(TemplateParameter)

    static TemplateParameterModelItem create(CodeModel *model);
    virtual ~_TemplateParameterModelItem();

  public:
    TypeInfo type() const;
    void setType(TypeInfo type);

    bool defaultValue() const;
    void setDefaultValue(bool defaultValue);

  private:
    TypeInfo _M_type;
    bool _M_defaultValue;

  protected:
    _TemplateParameterModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _TemplateParameterModelItem(const _TemplateParameterModelItem &other);
    void operator=(const _TemplateParameterModelItem &other);
  };

class _TemplateModelItem :  public _CodeModelItem
  {

  public:
    DECLARE_MODEL_NODE(Template)

    static TemplateModelItem create(CodeModel *model);
    virtual ~_TemplateModelItem();

  public:
    TemplateParameterList parameters() const;
    void addParameter(TemplateParameterModelItem item);
    void removeParameter(TemplateParameterModelItem item);

    CodeModelItem declaration() const;
    void setDeclaration(CodeModelItem declaration);

  private:
    TemplateParameterList _M_parameters;
    CodeModelItem _M_declaration;

  protected:
    _TemplateModelItem(CodeModel *model,  int kind =  __node_kind);

  private:
    _TemplateModelItem(const _TemplateModelItem &other);
    void operator=(const _TemplateModelItem &other);
  };

template  <class _Target,  class _Source>
_Target model_safe_cast(_Source item)
{
  typedef typename _Target::Type * _Target_pointer;
  typedef typename _Source::Type * _Source_pointer;

  _Source_pointer source =  item.data();

  if  (source &&  source->kind() ==  _Target_pointer(0)->__node_kind)
    {
      _Target ptr(static_cast<_Target_pointer>(source));
      return  ptr;
    }

  return  _Target();
}

template  <typename _Target,  typename _Source>
_Target model_dynamic_cast(_Source item)
{
  typedef typename _Target::Type * _Target_pointer;
  typedef typename _Source::Type * _Source_pointer;

  _Source_pointer source =  item.data();

  if  (source &&  (source->kind() ==  _Target_pointer(0)->__node_kind
                   ||  (_Target_pointer(0)->__node_kind <=  int(_CodeModelItem::KindMask)
                        &&  ((source->kind() & _Target_pointer(0)->__node_kind)
                             ==  _Target_pointer(0)->__node_kind))))
    {
      _Target ptr(static_cast<_Target_pointer>(source));
      return  ptr;
    }

  return  _Target();
}

#endif


