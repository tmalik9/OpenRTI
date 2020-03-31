

#pragma once

#include "RTI/RTIAmbassador.h"
#include <list>
#include <map>

using namespace rti1516e;

class IllegalStateException : public std::exception
{
  public:
    IllegalStateException(std::string t);
    IllegalStateException(std::wstring t);
};

class IllegalArgumentException : public std::exception
{
  public:
    IllegalArgumentException(std::string t);
    IllegalArgumentException(std::wstring t);
};

/**
 * This class helps do MOM related stuff (including 1.3/1516 conversions). As a result, it is VERY
 * ugly internally. I can't understand why the IEEE 1516 standards group decided to ENTIRELY BREAK
 * ALL BACKWARDS COMPATIBILITY by prefixing everything with "HLA", but they did. As a result, we
 * have to do all sorts of ugly stuff we wouldn't normally have to do, so get used to it.
 */
class Mom
{
  public:

    struct MomTreeNode;
    struct MomHandleTree;

    //----------------------------------------------------------
    //                      ENUMERATIONS
    //----------------------------------------------------------
    /**
     * Types of things that can be stored in the {@link MomHandleTree}
     */
    enum MomType
    {
      Object,
      Attribute,
      Interaction,
      Parameter
    };

    //----------------------------------------------------------
    //                   INSTANCE VARIABLES
    //----------------------------------------------------------
    MomHandleTree* mMomHandleTree;

    ObjectInstanceHandle federationObject;
    ObjectInstanceHandle federateObject;
    //----------------------------------------------------------
    //                      CONSTRUCTORS
    //----------------------------------------------------------

    Mom(RTIambassador* rtiAmb)
    {
      // Build interaction hierarchy
      initialize(rtiAmb);
    }

    //----------------------------------------------------------
    //                    INSTANCE METHODS
    //----------------------------------------------------------
    /**
     * Builds the internal representation of metadata for standard MOM types.
     * <p/>
     * The tree contains nodes representing the hierarchy of objects, attributes, interactions and
     * parameters and can be used to populate an {@link ObjectModel} with {@link OCMetadata}
     * and {@link ICMetadata} for all MOM types.
     * <p/>
     * This method also generates the lookup tables that resolve {@link MomTreeNode} metadata by handle
     */
    void initialize(RTIambassador* rtiAmb);

    void registerFederateObject(RTIambassador* rtiAmb, const std::wstring& federationName, const std::wstring& federateName);

    struct AttributesAccessor;
    struct ObjectClassAccessor;

    struct AttributesAccessor
    {
      AttributesAccessor() : mRoot(nullptr) {}
      AttributesAccessor(MomTreeNode* root) : mRoot(root) {}
      AttributesAccessor(const AttributesAccessor& ref) {
        mRoot = ref.mRoot;
      }
      AttributesAccessor(AttributesAccessor&& ref) {
        mRoot = std::move(ref.mRoot);
      }
      AttributesAccessor& operator=(AttributesAccessor&& ref)
      {
        if (this != &ref)
        {
          mRoot = std::move(ref.mRoot);
        }
        return *this;
      }
      AttributesAccessor& operator=(const AttributesAccessor& ref)
      {
        if (this != &ref)
        {
          mRoot = ref.mRoot;
        }
        return *this;
      }
      AttributesAccessor operator [](const std::wstring& name)
      {
        if (mRoot != nullptr)
        {
          if (mRoot->type == MomType::Attribute)
          {
            if (name == mRoot->getName())
            {
              return *this;
            }
          }
          else
          {
            for (auto* childNode : mRoot->children)
            {
              if (childNode->type == MomType::Attribute && childNode->getName() == name)
              {
                return AttributesAccessor(childNode);
              }
            }
          }
        }
        return AttributesAccessor();
      }
      operator AttributeHandle() const { return mRoot != nullptr ? mRoot->attributeHandle : AttributeHandle(); }
      MomTreeNode* mRoot;
    };

    struct ObjectClassAccessor
    {
      ObjectClassAccessor() : mRoot(nullptr) {}
      ObjectClassAccessor(const ObjectClassAccessor& ref) {
        mRoot = ref.mRoot;
        attributes.mRoot = mRoot;
      }
      ObjectClassAccessor(ObjectClassAccessor&& ref) {
        mRoot = std::move(ref.mRoot);
        attributes.mRoot = mRoot;
      }
      ObjectClassAccessor& operator=(ObjectClassAccessor&& ref)
      {
        if (this != &ref)
        {
          mRoot = std::move(ref.mRoot);
          attributes.mRoot = mRoot;
        }
        return *this;
      }
      ObjectClassAccessor& operator=(const ObjectClassAccessor& ref)
      {
        if (this != &ref)
        {
          mRoot = ref.mRoot;
          attributes.mRoot = mRoot;
        }
        return *this;
      }
      ObjectClassAccessor(MomTreeNode* root) : mRoot(root) { attributes = AttributesAccessor(mRoot); }
      ObjectClassAccessor operator [](const std::wstring& name)
      {
        if (mRoot != nullptr)
        {
          if (mRoot->type == MomType::Object && mRoot->parent == nullptr)
          {
            if (name == mRoot->getName())
            {
              return *this;
            }
          }
          for (auto* childNode : mRoot->children)
          {
            if (childNode->type == MomType::Object && childNode->getName() == name)
            {
              return ObjectClassAccessor(childNode);
            }
          }
        }
        return ObjectClassAccessor();
      }
      operator ObjectClassHandle() const { return mRoot != nullptr ? mRoot->objectClassHandle : ObjectClassHandle(); }
      AttributesAccessor attributes;
      MomTreeNode* mRoot;
    };

    struct InteractionClassAccessor
    {
      InteractionClassAccessor() : mRoot(nullptr) {}
      InteractionClassAccessor(MomTreeNode* root) : mRoot(root) {}
      InteractionClassAccessor(const InteractionClassAccessor& ref) {
        mRoot = ref.mRoot;
      }
      InteractionClassAccessor(InteractionClassAccessor&& ref) {
        mRoot = std::move(ref.mRoot);
      }
      InteractionClassAccessor& operator=(InteractionClassAccessor&& ref)
      {
        if (this != &ref)
        {
          mRoot = std::move(ref.mRoot);
        }
        return *this;
      }
      InteractionClassAccessor& operator=(const InteractionClassAccessor& ref)
      {
        if (this != &ref)
        {
          mRoot = ref.mRoot;
        }
        return *this;
      }
      InteractionClassAccessor operator [](const std::wstring& name)
      {
        if (mRoot != nullptr)
        {
          for (auto* childNode : mRoot->children)
          {
            if (childNode->getName() == name)
            {
              return InteractionClassAccessor(childNode);
            }
          }
        }
        return InteractionClassAccessor();
      }
      operator InteractionClassHandle() const { return mRoot != nullptr ? mRoot->interactionClassHandle : InteractionClassHandle(); }
      MomTreeNode* mRoot;
    };

    ObjectClassAccessor getObjects()
    {
      return ObjectClassAccessor(mMomHandleTree->objectRoot);
    }

    InteractionClassAccessor getInteractions()
    {
      return InteractionClassAccessor(mMomHandleTree->interactionRoot);
    }
    ObjectClassAccessor objects;
    InteractionClassAccessor interactions;
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// MOM Hierarchy Generation Methods ////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    struct MomTreeNodeBuilder
    {
      std::list<MomTreeNode*> mContextStack;
      RTIambassador* mRtiAmb;
      MomTreeNodeBuilder(RTIambassador* rtiAmb) : mRtiAmb(rtiAmb)
      {
      }

      /**
       * Adds a node to the context current node.
       * <p/>
       * <b>Note:</b> This method does not affect the context stack.
       *
       * @param newNode the node to add to the current context node
       * @throws IllegalStateException if the context stack is currently empty
       */
      void add(MomTreeNode* newNode);

      /**
       * Adds the specified node to the context current node and pushes it onto the context stack.
       *
       * @param newNode the node to add to the current context node
       */
      void addAndPush(MomTreeNode* newNode);

      /**
       * Creates an object {@link MomTreeNode} for the specified HLA Versions and adds it to the current
       * context node.
       * <p/>
       * <b>Note:</b> The object's name for each HLA version naming scheme will be automatically
       * generated from the <code>name</code> parameter via the simple pattern rule.
       * <p/>
       * The created node is pushed onto the context stack to become the new context node.
       *
       * @param name the name of the object
       * @param supportedVersions the HLA Versions that the object is supported in
       * @return this
       */
      MomTreeNodeBuilder&  object(std::wstring name);

      /**
       * Creates an attribute {@link MomTreeNode} for the specified HLA Versions and adds it to the
       * current context node.
       *
       * @param name the name of the attribute under each HLA version
       * @param datatype the name of this attribute's datatype
       * @param supportedVersions the HLA Versions that support this attribute
       * @return this
       * @throws IllegalStateException if the current context is not an Object node
       */
      MomTreeNodeBuilder&  attribute(std::wstring name, std::wstring datatype);


      /**
       * Creates an interaction {@link MomTreeNode} for the specified HLA Versions and adds it to the
       * current context node.
       * <p/>
       * <b>Note:</b> The interaction's name for each HLA version naming scheme will be automatically
       * generated from the <code>name</code> parameter via the simple pattern rule.
       * <p/>
       * The created node is pushed onto the context stack to become the new context node.
       *
       * @param name the name of the interaction under each HLA version
       * @param supportedVersions the HLA Versions that support this attribute
       * @return this
       */
      MomTreeNodeBuilder&  interaction(std::wstring name);

      /**
       * Creates a parameter {@link MomTreeNode} for the specified HLA Versions and adds it to the
       * current context node.
       *
       * @param name the name of the parameter under each HLA version
       * @param datatype the name of this parameter's datatype
       * @param supportedVersions the HLA Versions that support this parameter
       * @return this
       * @throws IllegalStateException if the current context is not an Parameter node
       */
      MomTreeNodeBuilder&  parameter(std::wstring name, std::wstring datatype);

      /**
       * Marks the end of the current context node, popping it from the stack.
       *
       * @return the previous node on the context stack
       */
      MomTreeNodeBuilder&  end()
      {
        this->mContextStack.pop_front();
        return *this;
      }

      /**
       * @return the current context node
       */
      MomTreeNode* getContext()
      {
        return this->mContextStack.front();
      }
    };

    struct MomHandleTree
    {
      MomTreeNode* objectRoot;
      MomTreeNode* interactionRoot;

      /**
       * MomHandleTree constructor with specified interaction root
       *
       * @param interactionRoot the tree node representing InteractionRoot
       */
      MomHandleTree(MomTreeNode* objectRoot, MomTreeNode* interactionRoot);

      /**
       * Splits a qualified name into individual tokens
       *
       * @param name the qualified name to split
       * @return the name tokens ordered from root to tip
       */
      std::vector<std::wstring> tokenizeName(std::wstring name);
      void getClassAttributes(AttributeHandleSet& attributes, MomTreeNode* objectClassNode);
      void getClassAttributesValueVector(AttributeHandleValueMap& attributes, MomTreeNode* objectClassNode);
      bool publishLeafObjectClasses(RTIambassador* rtiAmb, MomTreeNode* root);
      bool subscribeLeafObjectClasses(RTIambassador* rtiAmb, MomTreeNode* root);

      bool publishLeafInteractionClasses(RTIambassador* rtiAmb, MomTreeNode* root);
      bool subscribeLeafInteractionClasses(RTIambassador* rtiAmb, MomTreeNode* root);
    };

    struct MomTreeNode
    {
      MomTreeNode* parent;
      std::list<MomTreeNode*> children;
      std::wstring mName;
      ObjectClassHandle objectClassHandle;
      AttributeHandle attributeHandle;
      InteractionClassHandle interactionClassHandle;
      ParameterHandle parameterHandle;
      MomType type;
      std::wstring datatype; // For attributes and parameters, only for 1516+ (all mom types are strings in 1.3)


      MomTreeNode(std::wstring name,
        MomType type,
        ObjectClassHandle handle)
      {
        this->mName = name;
        this->parent = nullptr;
        this->objectClassHandle = handle;
        this->type = type;
        this->datatype = L"HLAasciiString";
      }

      MomTreeNode(std::wstring name,
        MomType type,
        AttributeHandle handle,
        std::wstring datatype)
      {
        this->mName = name;
        this->parent = nullptr;
        this->attributeHandle = handle;
        this->type = type;
        this->datatype = L"HLAasciiString";
      }

      MomTreeNode(std::wstring name,
        MomType type,
        InteractionClassHandle handle)
      {
        this->mName = name;
        this->parent = nullptr;
        this->interactionClassHandle = handle;
        this->type = type;
        this->datatype = datatype;
      }

      MomTreeNode(std::wstring name,
        MomType type,
        ParameterHandle handle,
        std::wstring datatype)
      {
        this->mName = name;
        this->parent = nullptr;
        this->parameterHandle = handle;
        this->type = type;
        this->datatype = datatype;
      }
      MomTreeNode* getParent()
      {
        return this->parent;
      }

      MomType getType() const
      {
        return this->type;
      }

      //std::list<MomTreeNode*>& getChildren()
      //{
      //  return this->children;
      //}

      const std::list<MomTreeNode*>& getChildren() const
      {
        return this->children;
      }

      /**
       * Returns the version-specific name of this node.
       *
       * @param version a HLA specification version
       * @return the name of the node for the specified HLA version
       *
       * @see #getQualifiedName()
       */
      const std::wstring getName() const
      {
        return this->mName;
      }

      /**
       * Returns the fully qualified version-specific name of this node.
       *
       * @param version a HLA specification version
       * @return the name of the node for the specified HLA version
       *
       * @see #getName()
       */
      std::wstring getQualifiedName() const;

      const std::wstring& getDatatype() const
      {
        return this->datatype;
      }

      void addChild(MomTreeNode* node);
    };
};

