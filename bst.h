/**
 * bst.h
 *  Implements a(n unbalanced) BST storing Key,Value pairs
 */
#ifndef BST_H
#define BST_H
#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

 #include <stack>

/* -----------------------------------------------------
 * Regular Binary Tree Nodes
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
  class Node {
 public:
  Node (const KeyType & k, const ValueType & v, Node<KeyType, ValueType> *p)
    : _item(k, v)
   // the default is to create new nodes as leaves
    { _parent = p; _left = _right = NULL; }
  
  virtual ~Node()
  { }
  
  std::pair<const KeyType, ValueType> const & getItem () const
    { return _item; }
  
  std::pair<const KeyType, ValueType> & getItem ()
    { return _item; }
  
  const KeyType & getKey () const
  { return _item.first; }
  
  const ValueType & getValue () const
  { return _item.second; }
  
  /* the next three functions are virtual because for AVL-Trees,
     we'll want to use AVL nodes, and for those, the
     getParent, getLeft, and getRight functions should return 
     AVL nodes, not just ordinary nodes.
     That's an advantage of using getters/setters rather than a struct. */
  
  virtual Node<KeyType, ValueType> *getParent () const
    { return _parent; }
  
  virtual Node<KeyType, ValueType> *getLeft () const
    { return _left; }
  
  virtual Node<KeyType, ValueType> *getRight () const
    { return _right; }
  
  
  void setParent (Node<KeyType, ValueType> *p)
  { _parent = p; }
  
  void setLeft (Node<KeyType, ValueType> *l)
  { _left = l; }
  
  void setRight (Node<KeyType, ValueType> *r)
  { _right = r; }
  
  void setValue (const ValueType &v)
  { _item.second = v; }
  
 protected:
  std::pair<const KeyType, ValueType> _item;
  Node <KeyType, ValueType> *_left, *_right, *_parent;
};

/* -----------------------------------------------------
 * Regular Binary Search Tree
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
class BinarySearchTree {
 protected:
  // Main data member of the class
  Node<KeyType, ValueType> *root;

 public:
  /**
   * Constructor
   */
  BinarySearchTree () { root = NULL; }

  /**
   * Destructor
   */
  ~BinarySearchTree () { deleteAll (root); }

  /**
   * Prints the entire tree structure in a nice format 
   *  
   * It will denote subtrees in [] brackets.
   *  This could be helpful if you want to debug your functions. 
   */  
  void print () const
  { 
    printRoot (root);
    std::cout << "\n";
  }

	void insert(const std::pair<KeyType, ValueType> &keyValuePair)
	{
		insertHelper(keyValuePair, root);
	}

	void insertHelper(const std::pair<KeyType, ValueType> &keyValuePair, Node<KeyType, ValueType> *root)
	{
		if(root == NULL)
		{
			// This _root variable will need to change if student's have renamed their variable!!
			this->root = new Node<KeyType, ValueType>(keyValuePair.first, keyValuePair.second, NULL);
		}
		else if(root->getKey() < keyValuePair.first)
		{
			if(root->getRight() != NULL)
			{
				insertHelper(keyValuePair, root->getRight());
			}
			else
			{
				root->setRight(new Node<KeyType, ValueType>(keyValuePair.first, keyValuePair.second, root));
			}
		}
		else if(root->getKey() > keyValuePair.first)
		{
			if(root->getLeft() != NULL)
			{
				insertHelper(keyValuePair, root->getLeft());
			}
			else
			{
				root->setLeft(new Node<KeyType, ValueType>(keyValuePair.first, keyValuePair.second, root));
			}
		}
		else
		{
			root->setValue(keyValuePair.second);
		}
	}

	
    
  /**
   * An In-Order iterator
   *  !!! You must implement this !!!
   */
  class iterator {
  public:
    /**
     * Initialize the internal members of the iterator
     */
    iterator(Node<KeyType,ValueType>* ptr) : curr(ptr) {
      // nodeStack = new std::stack<Node<KeyType, ValueType>*>();

      // bool should_pop = 0;
      // nodeStack->push(root);
      // while ( !nodeStack->empty() ) {
      // 	Node<KeyType,ValueType>* node = nodeStack->top();
      	
      // 	if ( node->getLeft() == NULL || should_pop ) {
      // 		if ( node == curr ) break;
      // 		nodeStack->pop();
      // 		should_pop = 1;
      // 		if ( node->getRight() != NULL ) { nodeStack->push(node->getRight()); should_pop = 1; }
      // 	}
      // 	else nodeStack->push(node->getLeft);
      // }
    } // edit

    ~iterator() {
      // delete nodeStack;
    }
    
    std::pair<const KeyType,ValueType>& operator*()
      { return curr->getItem();  }
    
    std::pair<const KeyType,ValueType>* operator->() 
      { return &(curr->getItem()); }
    
    /**
     * Checks if 'this' iterator's internals have the same value
     *  as 'rhs'
     */
    bool operator==(const iterator& rhs) const {
      return rhs.curr == this->curr;
    }
    
    /**
     * Checks if 'this' iterator's internals have a different value
     *  as 'rhs'
     */
    bool operator!=(const iterator& rhs) const {
      return rhs.curr != this->curr;
    }
    
    /**
     * Advances the iterator's location using an in-order sequencing
     */
    iterator& operator++() {
      // advance the current pointer
      curr = getSuccesor();
      return *this; // just derefence the object pointer, not related to * operator
    }
    
  protected:
    Node<KeyType, ValueType>* curr;
    //you are welcome to add any necessary variables and helper functions here.
    std::stack<Node<KeyType, ValueType>*>* nodeStack;

  private:
    Node<KeyType, ValueType>* getSuccesor() {
      Node<KeyType, ValueType>* node;
      if ( curr->getRight() != NULL) { // find the minimum in the right sub-tree
      	node = curr->getRight();
      	while ( node->getLeft() != NULL ) node = node->getLeft();      		
      	return node;
      }

      node = curr->getParent();
      while ( node != NULL ) { // find bottom-up until NULL or node is the left child of its parent
      	if ( node->getKey() > curr->getKey() ) return node;
      	node = node->getParent();
      }
      return NULL;
    }
  };
  /*
  ** end of iterator definition
  */
  
  /**
   * Returns an iterator to the "smallest" item in the tree
   */
  iterator begin() {
    Node<KeyType, ValueType>* ptr = root;
    while ( ptr != NULL && ptr->getLeft() != NULL) {
      ptr = ptr->getLeft();
    }
    return iterator(ptr);
  } 

  /**
   * Returns an iterator whose value means INVALID (zero?)
   */
  iterator end() {
    return iterator(0);
  }

  iterator find(const KeyType& k) const 
	{
		Node<KeyType, ValueType>* curr = internalFind(k);
		iterator it(curr);
		return it;
	} 
  
 protected:
  /**
   * Helper function to find a node with given key, k and 
   * return a pointer to it or NULL if no item with that key
   * exists
   */
  Node<KeyType, ValueType>* internalFind(const KeyType& k) const 
  {
    Node<KeyType, ValueType> *curr = root;
    while (curr) {
      if (curr->getKey() == k) {
				return curr;
      } else if (k < curr->getKey()) {
				curr = curr->getLeft();
      } else {
				curr = curr->getRight();
      }
    }
    return NULL;
  }
  
  /**
   * Helper function to print the tree's contents
   */
  void printRoot (Node<KeyType, ValueType> *r) const
  {
    if (r != NULL)
      {
				std::cout << "[";
				printRoot (r->getLeft());
				std::cout << " (" << r->getKey() << ", " << r->getValue() << ") ";
				printRoot (r->getRight());
				std::cout << "]";
      }
  }
  
  /**
   * Helper function to delete all the items
   */
  void deleteAll (Node<KeyType, ValueType> *r)
  {
    if (r != NULL)
      {
	deleteAll (r->getLeft());
	deleteAll (r->getRight());
	delete r;
      }
  }


};

/* Feel free to add member function definitions here if you need */


#endif
