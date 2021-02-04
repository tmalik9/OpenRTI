

#pragma once

#include "RTI/RTIAmbassador.h"
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTI/encoding/HLAvariableArray.h"

#include <list>
#include <map>
#include <functional>
#include <cassert>

using namespace rti1516e;

class IllegalStateException : public std::logic_error
{
  public:
    IllegalStateException(std::string t);
    IllegalStateException(std::wstring t);
};

class IllegalArgumentException : public std::logic_error
{
  public:
    IllegalArgumentException(std::string t);
    IllegalArgumentException(std::wstring t);
};

class MomFederation;
class MomFederate;

class Mom : public std::enable_shared_from_this<Mom>
{
  private:
    typedef RTIambassador* RTIambassadorRefType;

    struct MomTreeNode;
    struct MomHandleTree;

  public:

    //----------------------------------------------------------
    //                      CONSTRUCTORS
    //----------------------------------------------------------

    Mom(RTIambassadorRefType rtiAmb) : mRtiAmb(rtiAmb)
    {
      // Build interaction hierarchy
      initialize();
    }

    //----------------------------------------------------------
    //                    INSTANCE METHODS
    //----------------------------------------------------------
    /**
     * Builds the internal representation of metadata for standard MOM types.
     */
    void initialize();

    void OnFederationJoined(const std::wstring& federationName/*, rti1516e::FederateHandle federateHandle*/);

    // NOTE: these are *NOT* the callbacks implemented from FederateAmbassador!
    void discoverObjectInstance(ObjectInstanceHandle theObject, ObjectClassHandle theObjectClass, std::wstring const& theObjectInstanceName);
    void removeObjectInstance(ObjectInstanceHandle theObject);
    void reflectAttributeValues(
      ObjectInstanceHandle theObject,
      AttributeHandleValueMap const& theAttributeValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      SupplementalReflectInfo theReflectInfo);

    struct AttributesAccessor;
    struct ObjectClassAccessor;

    struct AttributesAccessor
    {
      AttributesAccessor() : mRoot(nullptr) {}
      AttributesAccessor(MomTreeNode* root) : mRoot(root) {}
      AttributesAccessor(const AttributesAccessor& ref)
      {
        mRoot = ref.mRoot;
      }
      AttributesAccessor(AttributesAccessor&& ref)
      {
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
            auto* currentNode = mRoot;
            while (currentNode != nullptr && currentNode->type == MomType::Object)
            {
              for (auto* childNode : currentNode->children)
              {
                if (childNode->type == MomType::Attribute && childNode->getName() == name)
                {
                  return AttributesAccessor(childNode);
                }
              }
              currentNode = currentNode->parent;
            }
          }
        }
        return AttributesAccessor();
      }
      operator AttributeHandle() const { return (mRoot != nullptr && mRoot->type == MomType::Attribute) ? mRoot->attributeHandle : AttributeHandle(); }
      operator AttributeHandleSet() const
      {
        AttributeHandleSet result;
        MomTreeNode* currentNode = mRoot;
        while (currentNode != nullptr && currentNode->type == MomType::Object)
        {
          for (MomTreeNode* child : currentNode->children)
          {
            if (child->type == MomType::Attribute)
            {
              assert(child->attributeHandle.isValid());
              result.insert(child->attributeHandle);
            }
          }
          currentNode = currentNode->parent;
        }
        return result;
      }
      MomTreeNode* mRoot;
    };

    struct ObjectClassAccessor
    {
      ObjectClassAccessor() : mRoot(nullptr) {}
      ObjectClassAccessor(const ObjectClassAccessor& ref)
      {
        mRoot = ref.mRoot;
        attributes.mRoot = mRoot;
      }
      ObjectClassAccessor(ObjectClassAccessor&& ref)
      {
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

    struct ParametersAccessor
    {
      ParametersAccessor() : mRoot(nullptr) {}
      ParametersAccessor(MomTreeNode* root) : mRoot(root) {}
      ParametersAccessor(const ParametersAccessor& ref)
      {
        mRoot = ref.mRoot;
      }
      ParametersAccessor(ParametersAccessor&& ref)
      {
        mRoot = std::move(ref.mRoot);
      }
      ParametersAccessor& operator=(ParametersAccessor&& ref)
      {
        if (this != &ref)
        {
          mRoot = std::move(ref.mRoot);
        }
        return *this;
      }
      ParametersAccessor& operator=(const ParametersAccessor& ref)
      {
        if (this != &ref)
        {
          mRoot = ref.mRoot;
        }
        return *this;
      }
      ParametersAccessor operator [](const std::wstring& name)
      {
        if (mRoot != nullptr)
        {
          if (mRoot->type == MomType::Parameter)
          {
            if (name == mRoot->getName())
            {
              return *this;
            }
          }
          else
          {
            auto* currentNode = mRoot;
            while (currentNode != nullptr && currentNode->type == MomType::Interaction)
            {
              for (auto* childNode : currentNode->children)
              {
                if (childNode->type == MomType::Parameter && childNode->getName() == name)
                {
                  return ParametersAccessor(childNode);
                }
              }
              currentNode = currentNode->parent;
            }
          }
        }
        return ParametersAccessor();
      }
      operator ParameterHandle() const { return mRoot != nullptr ? mRoot->parameterHandle : ParameterHandle(); }
      operator ParameterHandleSet() const
      {
        ParameterHandleSet result;
        MomTreeNode* currentNode = mRoot;
        while (currentNode != nullptr && currentNode->type == MomType::Interaction)
        {
          for (MomTreeNode* child : currentNode->children)
          {
            if (child->type == MomType::Parameter)
            {
              assert(child->parameterHandle.isValid());
              result.insert(child->parameterHandle);
            }
          }
          currentNode = currentNode->parent;
        }
        return result;
      }
      MomTreeNode* mRoot;
    };

    struct InteractionClassAccessor
    {
      InteractionClassAccessor() : mRoot(nullptr) {}
      InteractionClassAccessor(MomTreeNode* root) : mRoot(root) { parameters = ParametersAccessor(mRoot); }
      InteractionClassAccessor(const InteractionClassAccessor& ref)
      {
        mRoot = ref.mRoot;
        parameters.mRoot = mRoot;
      }
      InteractionClassAccessor(InteractionClassAccessor&& ref)
      {
        mRoot = std::move(ref.mRoot);
        parameters = ParametersAccessor(mRoot);
      }
      InteractionClassAccessor& operator=(InteractionClassAccessor&& ref)
      {
        if (this != &ref)
        {
          mRoot = std::move(ref.mRoot);
          parameters = ParametersAccessor(mRoot);
        }
        return *this;
      }
      InteractionClassAccessor& operator=(const InteractionClassAccessor& ref)
      {
        if (this != &ref)
        {
          mRoot = ref.mRoot;
          parameters = ParametersAccessor(mRoot);
        }
        return *this;
      }
      InteractionClassAccessor operator [](const std::wstring& name)
      {
        if (mRoot != nullptr)
        {
          if (mRoot->type == MomType::Interaction && mRoot->parent == nullptr)
          {
            if (name == mRoot->getName())
            {
              return *this;
            }
          }
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
      ParametersAccessor parameters;
      MomTreeNode* mRoot;
    };

    ObjectClassAccessor objects;
    InteractionClassAccessor interactions;

    std::shared_ptr<MomFederation> getFederation() const { return mFederation; }

    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// MOM Hierarchy Generation Methods ////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
  private:
    std::shared_ptr<MomFederate> RegisterFederate(FederateHandle federateHandle, ObjectInstanceHandle theObject);

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
    };

    enum MomType
    {
      Object,
      Attribute,
      Interaction,
      Parameter
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

  private:

    // maybe, later: std::shared_ptr<RTIambassador> mRtiAmb;
    RTIambassadorRefType mRtiAmb;

    // the search tree for all kinds of handles
    MomHandleTree* mMomHandleTree;

    FederateHandleSet mKnownFederates;
    std::set<ObjectInstanceHandle> mKnownFederateObjects;

    ObjectClassHandle mFederationObjectClass;
    ObjectInstanceHandle mFederationObject;
    std::shared_ptr<MomFederation> mFederation;

    ObjectClassHandle mFederateObjectClass;
    // the object corresponding to our own federate
    //ObjectInstanceHandle mFederateObject;

    std::map<ObjectInstanceHandle, std::shared_ptr<MomFederate>> mFederatesByObject;
};

