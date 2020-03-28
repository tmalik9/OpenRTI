

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

    class MomTreeNode;
    class MomHandleTree;

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
    MomHandleTree* momTree;

    // Lookup tables for mom types
    std::map<rti1516e::ObjectClassHandle, MomTreeNode*> objectLookup;
    std::map<rti1516e::AttributeHandle, MomTreeNode*> attributeLookup;
    std::map<rti1516e::InteractionClassHandle, MomTreeNode*> interactionLookup;
    std::map<rti1516e::ParameterHandle, MomTreeNode*> parameterLookup;

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

    //----------------------------------------------------------
    //                     STATIC METHODS
    //----------------------------------------------------------
    /**
     * Fetch the handle for the MOM class of the given name.
     * <p/>
     * This function expects a fully qualified name be provided in the <code>className</code> parameter,
     * relative to <code>ObjectRoot</code> (e.g. "Manager.Federate")
     * <p/>
     * If the class name is unknown, {@link ObjectModel#INVALID_HANDLE} will be returned.
     *
     * @param version the HLA version of the MOM naming scheme that the <code>className</code> parameter
     *                conforms to
     * @param className the qualified name of the class to resolve the handle for, relative to ObjectRoot
     * @return the handle of the desired object class, or {@link ObjectModel#INVALID_HANDLE} if no class
     *         exists with the specified name
     */
    int getMomObjectClassHandle(std::wstring className);

    /**
     * Fetch the handle for the MOM attribute of the given name that belongs to the MOM class of the given
     * handle. If the class handle is not recognized or the name isn't a valid attribute,
     * {@link ObjectModel#INVALID_HANDLE} will be returned.
     *
     * @param version the HLA version of the MOM naming scheme that the <code>name</code> parameter
     *                conforms to
     * @param classHandle the handle of the object class to search for the attribute in
     * @param the name of the attribute
     * @return the handle of the desired attribute, or {@link ObjectModel#INVALID_HANDLE} if the specified
     *         class does not exist or does not contain an attribute with the specified name
     */
    int getMomAttributeHandle(int classHandle, std::wstring name);

    /**
     * Fetch the name for the MOM attribute that belongs to the MOM class of the given handle. If the
     * class handle is not recognized or the attribute handle isn't a valid attribute,
     * {@link ObjectModel#INVALID_HANDLE} will be returned.
     *
     * @param version the HLA version of the MOM naming scheme to use
     * @param attributeHandle the handle of the attribute
     * @return the name of the desired attribute, or <code>nullptr</code> if the specified
     *         class does not exist or does not contain the specified attribute
     */
    std::wstring getMomAttributeName(rti1516e::AttributeHandle attributeHandle);

    /**
     * Fetch the handle for the MOM interaction class of the given name. If the class name is unknown,
     * {@link ObjectModel#INVALID_HANDLE} will be returned.
     * <p/>
     * This function expects a fully qualified name be provided in the <code>className</code> parameter,
     * relative to <code>InteractionRoot</code> (e.g. "Manager.Federate.Request.RequestSubscriptions")
     * <p/>
     * If the class name is unknown, {@link ObjectModel#INVALID_HANDLE} will be returned.
     *
     * @param version the HLA version of the MOM naming scheme that the <code>className</code> parameter
     *                conforms to
     * @param className the qualified name of the interaction class relative to InteractionRoot
     * @return the handle of the desired interaction class, or {@link ObjectModel#INVALID_HANDLE} if no
     *         such interaction class exists
     */
    int getMomInteractionHandle(std::wstring className);

    /**
     * Fetch the name for the MOM interaction class of the given handle.
     * <p/>
     * If the class handle is not recognized, {@link ObjectModel#INVALID_HANDLE} will be returned.
     *
     * @param version the HLA version of the MOM naming scheme to use
     * @param classHandle the handle of the interaction class
     * @param qualified <code>true</code> to return the fully qualified name of the interaction class,
     *                  otherwise <code>false</code> to return the name of the specified interaction class
     *                  only
     * @return the name of the desired interaction class, or <code>nullptr</code> if the specified
     *         class does not exist
     */
    std::wstring getMomInteractionName(rti1516e::InteractionClassHandle, bool qualified);

    /**
     * Fetch the name of the MOM Interaction Parameter of the specified handle.
     * <p/>
     * If no parameter exists within the specified interaction with the given handle, then
     * <code>nullptr</code> is returned.
     *
     * @param version the HLA version of the MOM naming scheme to format the name for
     * @param parameterHandle the handle of the parameter to fetch the name for
     */
    std::wstring getMomParameterName(rti1516e::ParameterHandle parameterHandle);

    /////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// Object Model Update Methods //////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /**
     * Build the hierarchy of MOM object classes and initialize them with the default handles. This
     * method will also patch the MOM stuff into the model, adding all the classes and setting the
     * manager class (for example) as a child of the object root.
     */

    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// MOM Hierarchy Generation Methods ////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////


    /**
     * A helper class that allows declarative construction of the MOM tree.
     * <p/>
     * The {@link MomTreeNodeBuilder} class supports chained declarative building by returning a reference
     * to itself from each of the builder methods (e.g. {@link #object(std::wstring)},
     * {@link #attribute(std::wstring, std::wstring)}) and maintaining an internal context stack. For example an
     * object class with attributes and a sub-class could be declared as such:
     *
     * <pre>
     *  MomTreeNodeBuilder b = new MomTreeNodeBuilder();
     *  b.object("BaseClass")           // push an object node onto the context stack
     *    .attribute("baseAttribute")   // add an attribute to the current context (BaseClass)
     *    .object("ChildClass")         // push another object node onto the context stack
     *     .attribute("childAttribute") // add an attribute to the current context (BaseClass.ChildClass)
     *    .end()                        // pop the current context from the stack
     *  MomTreeNode* baseNode = b.get(); // get the current context from the stack (BaseClass)
     * </pre>
     */
    class MomTreeNodeBuilder
    {
      public:
        int objectCounter = 0;
        int attributeCounter = 0;
        int interactionCounter = 0;
        int parameterCounter = 0;
        std::map<rti1516e::ObjectClassHandle, MomTreeNode*> objectLookup;
        std::map<rti1516e::AttributeHandle, MomTreeNode*> attributeLookup;
        std::map<rti1516e::InteractionClassHandle, MomTreeNode*> interactionLookup;
        std::map<rti1516e::ParameterHandle, MomTreeNode*> parameterLookup;

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
        MomTreeNodeBuilder&  attribute(std::wstring name,  std::wstring datatype);


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
        MomTreeNodeBuilder&  parameter(std::wstring name,  std::wstring datatype);

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

        std::map<rti1516e::ObjectClassHandle, MomTreeNode*>& getObjectLookup()
        {
          return this->objectLookup;
        }

        std::map<rti1516e::AttributeHandle, MomTreeNode*>& getAttributeLookup()
        {
          return this->attributeLookup;
        }

        std::map<rti1516e::InteractionClassHandle, MomTreeNode*>& getInteractionLookup()
        {
          return this->interactionLookup;
        }

        std::map<rti1516e::ParameterHandle, MomTreeNode*>& getParameterLookup()
        {
          return this->parameterLookup;
        }
    };

    /**
     * A tree to hold the internal representation of metadata for standard MOM types.
     * <p/>
     * The tree contains nodes representing the hierarchy of objects, attributes, interactions and
     * parameters and can be used to populate an {@link ObjectModel} with {@link OCMetadata}
     * and {@link ICMetadata} for all MOM types.
     * <p/>
     * To allow a cross-version lookup, we store a {@link VersionedName} for each node, so that we can
     * resolve MOM type handles regardless of which naming version is being used.
     * <p/>
     * Nodes can be fetched from the tree by qualified name using
     * {@link #find(std::wstring, HLAVersion, MomTreeNode)}, or by handle by calling
     * {@link #find(MomType, int, MomTreeNode)}.
     */
    class MomHandleTree
    {
      public:
        MomTreeNode* objectRoot;
        MomTreeNode* interactionRoot;

        /**
         * MomHandleTree constructor with specified interaction root
         *
         * @param interactionRoot the tree node representing InteractionRoot
         */
        MomHandleTree(MomTreeNode* objectRoot, MomTreeNode* interactionRoot);

        /**
         * Finds the tree node that matches the specified qualified name.
         *
         * @param name the qualified name of the node to find e.g. "Manager.Federation.Request"
         * @param root the node to begin the search from
         * @return the node in the tree that matches the specified name, or <code>nullptr</code> if no node
         *         exists in the tree with that name
         */
        MomTreeNode* find(std::wstring name, MomTreeNode* root);

        /**
         * Finds the tree node with the specified handle
         *
         * @param type the type of node to search for
         * @param handle the handle of the node to search for
         * @param root the node to begin the search from
         * @return the node in the tree that matches the specified type/handle, or <code>nullptr</code> if no
         *         node exists in the tree that matches the specified type/handle
         */
        MomTreeNode* find(MomType type, int handle, MomTreeNode* root);

        /**
         * Splits a qualified name into individual tokens
         *
         * @param name the qualified name to split
         * @return the name tokens ordered from root to tip
         */
        std::vector<std::wstring> tokenizeName(std::wstring name);
    };

    /**
     * A node in the MomHandleTree.
     * <p/>
     * Nodes in the tree may represent Mom Object Classes, Attributes, Interaction Classes or Parameters.
     * <p/>
     * To allow a cross-version lookup, we store a {@link VersionedName} for each node, so that we can
     * resolve MOM type handles regardless of which naming version is being used.
     */
    class MomTreeNode
    {
      public:
        MomTreeNode* parent;
        std::list<MomTreeNode*> children;
        std::wstring mName;
        int handle;
        rti1516e::ObjectClassHandle objectClassHandle;
        rti1516e::AttributeHandle attributeHandle;
        rti1516e::InteractionClassHandle interactionClassHandle;
        rti1516e::ParameterHandle parameterHandle;
        MomType type;
        std::wstring datatype; // For attributes and parameters, only for 1516+ (all mom types are strings in 1.3)

        MomTreeNode(std::wstring name,
                    MomType type,
                    int handle)
        {
          this->mName = name;
          this->parent = nullptr;
          this->handle = handle;
          this->type = type;
          this->datatype = L"HLAasciiString";
        }

        MomTreeNode(std::wstring name,
                    MomType type,
                    rti1516e::ObjectClassHandle handle)
        {
          this->mName = name;
          this->parent = nullptr;
          this->objectClassHandle = handle;
          this->type = type;
          this->datatype = L"HLAasciiString";
        }

        MomTreeNode(std::wstring name,
                    MomType type,
                    rti1516e::AttributeHandle handle,
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
                    rti1516e::InteractionClassHandle handle)
        {
          this->mName = name;
          this->parent = nullptr;
          this->interactionClassHandle = handle;
          this->type = type;
          this->datatype = datatype;
        }

        MomTreeNode(std::wstring name,
                    MomType type,
                    rti1516e::ParameterHandle handle,
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

        MomType getType()
        {
          return this->type;
        }

        int getHandle()
        {
          return this->handle;
        }

        std::list<MomTreeNode*>& getChildren()
        {
          return this->children;
        }

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
        std::wstring getName()
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
        std::wstring getQualifiedName();

        std::wstring getDatatype()
        {
          return this->datatype;
        }

        void addChild(MomTreeNode* node);

        std::wstring toString()
        {
          return this->getName();
        }
    };
};

