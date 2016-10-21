/*
 * avlbst.h
 *
 * Date        Author    Notes
 * =====================================================
 * 2014-04-14  Kempe     Initial version
 * 2015-04-06  Redekopp  Updated formatting and removed
 *                         KeyExistsException
 * 2016-03-31  Cote      Modify for AVL Trees
 */
#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include "bst.h"

#define get_max(a,b) ( (a) > (b) ? (a) : (b) )

class KeyNotExistsException;
class KeyAlreadyExistsException;

template <class KeyType, class ValueType>
class AVLNode : public Node<KeyType, ValueType>
{
public:
  AVLNode (KeyType k, ValueType v, AVLNode<KeyType, ValueType> *p)
    : Node<KeyType, ValueType> (k, v, p)
    { height = 0; }
  
  virtual ~AVLNode () {}
  
  int getHeight () const
    { return height; }
  
  void setHeight (int h)
    { height = h; }
  
  virtual AVLNode<KeyType, ValueType> *getParent () const
    { return (AVLNode<KeyType,ValueType>*) this->_parent; }
  
  virtual AVLNode<KeyType, ValueType> *getLeft () const
    { return (AVLNode<KeyType,ValueType>*) this->_left; }
  
  virtual AVLNode<KeyType, ValueType> *getRight () const
    { return (AVLNode<KeyType,ValueType>*) this->_right; }
  
 protected:
  int height;
};

/* -----------------------------------------------------
 * AVL Search Tree
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
class AVLTree : public BinarySearchTree<KeyType, ValueType>
{
public:
  void insert (const std::pair<const KeyType, ValueType>& new_item) {
    insertHelper(new_item, static_cast<AVLNode<KeyType,ValueType>*>(this->root));
    Node<KeyType, ValueType> *new_node = this->internalFind(new_item.first);
    Node<KeyType, ValueType> *z, *y, *x;
    z = findFirstUnbalanced(new_node, y, x);
    if ( z ) {
      // std::cout << "insert: first unbalanced " << z->getKey() << ", " << y->getKey()
      // << ", " << x->getKey() << std::endl;
      // rotate(z, y, x); // update
      // updateHeightBottomUp();
    }
  }
  /* This one is yours to implement.
     It should insert the (key, value) pair to the tree, 
     making sure that it remains a valid AVL Tree.
     If the key is already in the tree, you should throw an exception. */
  void remove (const KeyType &toRemove) {
    Node<KeyType, ValueType> *remove_node = this->internalFind(toRemove);
    if ( remove_node == NULL ) throw KeyNotExistsException();

    Node<KeyType, ValueType> *z, *y, *x;
    z = deleteBSTNode(remove_node);
    // updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(z) );
    // static int count = 1;
    // std::cout << "\n" << count++ << " Remove: after BST deletion" << std::endl;
    // printAVL();
    // std::cout << "----------------------------------------------------------\n";

    while ( z ) {
      z = findFirstUnbalanced(z, y, x);
      if ( !z ) break;

      int diff = compareChildrenHeight( static_cast<AVLNode<KeyType,ValueType>*>(z) );
      // find y
      if ( diff > 0 ) {
        y = z->getLeft();
        x = y->getLeft();
      }
      else {
        y = z->getRight();
        x = y->getRight();
      }
      // find x
      diff = compareChildrenHeight( static_cast<AVLNode<KeyType,ValueType>*>(y) );
      if ( diff > 0 ) 
        x = y->getLeft();
      else if ( diff < 0 ) 
        x = y->getRight();
      // std::cout << "remove: first unbalanced " << z->getKey() << ", " << 
      // y->getKey() << ", " << 
      // x->getKey() << std::endl;
      rotate(z, y, x);
    }
  }
  /* Implement this as well.  It should remove the (key, value) pair from
   * the tree which corresponds to the input parameter.
   * If the key is not in the tree, you should throw an exception.
   */

   void printAVL () const { 
    printAVLRoot (this->root);
    std::cout << "\n";
  }

  void printAVLRoot (Node<KeyType, ValueType> *r) const {
    if (r != NULL) {
        AVLNode<KeyType, ValueType>* avlr = static_cast<AVLNode<KeyType, ValueType>* >(r);
        std::cout << "[";
        printAVLRoot (r->getLeft());
        std::cout << " (" << r->getKey() << ", " << avlr->getHeight() << ") ";
        printAVLRoot (r->getRight());
        std::cout << "]";
      }
  }

private:
  void insertHelper (const std::pair<const KeyType, ValueType>& new_item, AVLNode<KeyType, ValueType>* root);

  Node<KeyType, ValueType>* findFirstUnbalanced (Node<KeyType, ValueType>* new_node, 
      Node<KeyType, ValueType>*& y, Node<KeyType, ValueType>*& x);
  Node<KeyType, ValueType>* rotate (Node<KeyType, ValueType>* z, Node<KeyType, ValueType>* y, Node<KeyType, ValueType>* x);
  void rotateLeft(Node<KeyType, ValueType>* n);
  void rotateRight(Node<KeyType, ValueType>* n);

  int compareChildrenHeight(AVLNode<KeyType, ValueType>* n);
  void updateHeight(AVLNode<KeyType, ValueType>* n);
  void updateHeightBottomUp(AVLNode<KeyType, ValueType>* n);

  void shiftUpwards(Node<KeyType, ValueType>* toShift);
  void replace(Node<KeyType, ValueType>* toInPlace, Node<KeyType, ValueType>* toRemove);
  Node<KeyType, ValueType>* deleteBSTNode(Node<KeyType, ValueType>* n);
};


template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::insertHelper (const std::pair<const KeyType, ValueType>& new_item, 
    AVLNode<KeyType, ValueType>* root){
  KeyType first = new_item.first;
  ValueType second = new_item.second;

  if ( root == NULL ) {
    this->root = new AVLNode<KeyType, ValueType>(first, second, NULL);
    return;
  }

  if ( first < root->getKey() ) {
    if ( root->getLeft() == NULL ) root->setLeft(new AVLNode<KeyType, ValueType>(first, second, root));
    else insertHelper(new_item, root->getLeft());
  }
  else if ( first > root->getKey() ) {
    if ( root->getRight() == NULL ) root->setRight(new AVLNode<KeyType, ValueType>(first, second, root));
    else insertHelper(new_item, root->getRight());
  }
  else throw KeyAlreadyExistsException();
}

/* return the difference between left child and right child of node n */
template <class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::compareChildrenHeight(AVLNode<KeyType, ValueType>* n) {
  int leftHeight = -1, rightHeight = -1;
    if ( n->getLeft() != NULL ) 
      leftHeight = (static_cast<AVLNode<KeyType,ValueType>*>(n->getLeft()))->getHeight();
    if ( n->getRight() != NULL ) 
      rightHeight = (static_cast<AVLNode<KeyType,ValueType>*>(n->getRight()))->getHeight();
    return leftHeight - rightHeight;
}

/* find the first unbalanced tree bottom up from node n */
template <class KeyType, class ValueType>
Node<KeyType, ValueType>* AVLTree<KeyType, ValueType>::findFirstUnbalanced (Node<KeyType, ValueType>* new_node,
      Node<KeyType, ValueType>*& y, Node<KeyType, ValueType>*& x) {
  Node<KeyType, ValueType>* z = new_node;
  y = x = NULL;

  while ( z ) {
    updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(z) ); // udate heights bottom up
    int diff;
    diff = compareChildrenHeight( static_cast<AVLNode<KeyType,ValueType>*>(z) );
    // std::cout << diff << "(" << z->getValue() << ") "; // print
    if ( diff < -1 or diff > 1 ) return z;
    x = y;
    y = z;
    z = z->getParent();
  }

  return NULL;
}

template <class KeyType, class ValueType>
Node<KeyType, ValueType>* 
AVLTree<KeyType, ValueType>::rotate (Node<KeyType, ValueType>* z, Node<KeyType, ValueType>* y,
    Node<KeyType, ValueType>* x) {
  KeyType kz = z->getKey(), ky = y->getKey(), kx = x->getKey();
  // Node<KeyType, ValueType> *zp = z->getParent();

  // case 1
  if ( kz < ky && ky < kx ) {
    rotateLeft(y);
    return y;
  }
  // case 2
  if ( kz > ky && ky > kx ) {
    rotateRight(y);
    return y;
  }
  
  // case 3
  if ( kz < kx && kx < ky ) {
    rotateRight(x);
    rotateLeft(x);
    return x;    
  }
  // case 4
  if ( kz > kx && kx > ky ) {
    rotateLeft(x);
    rotateRight(x);
    return x;
  }
  return NULL;
}

template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::rotateLeft (Node<KeyType, ValueType>* n) {
  Node<KeyType, ValueType>* lp = n->getParent(); // left parent
  Node<KeyType, ValueType>* gp = lp->getParent(); // grandparent
  Node<KeyType, ValueType>* lc = n->getLeft(); // right child
  if ( lp->getRight() != n ) throw "node to rotate left is not the right child of its parent";

  if ( lc ) lc->setParent(lp);
  lp->setRight(lc);
  lp->setParent(n);
  n->setParent(gp);
  n->setLeft(lp);
  updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(lp) );
  updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(n) );
  if ( lp == this->root ) this->root = n; // implies original parent of lp is NULL
  else if ( gp->getLeft() == lp ) gp->setLeft(n);
  else gp->setRight(n);
}

template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::rotateRight (Node<KeyType, ValueType>* n) {
  Node<KeyType, ValueType>* rp = n->getParent(); // right parent
  Node<KeyType, ValueType>* gp = rp->getParent(); // grandparent
  Node<KeyType, ValueType>* rc = n->getRight(); // right child
  if ( rp->getLeft() != n ) throw "node to rotate left is not the left child of its parent";

  // !!! each setParent should be paired up with setLeft or setRight, and vice versa
  if ( rc ) rc->setParent(rp);
  rp->setLeft(rc);
  rp->setParent(n);
  n->setParent(gp);
  n->setRight(rp);
  updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(rp) );
  updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(n) );
  if ( rp == this->root ) this->root = n; // implies original parent of lp is NULL
  else if ( gp->getLeft() == rp ) gp->setLeft(n);
  else gp->setRight(n);
}

template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::updateHeight(AVLNode<KeyType, ValueType>* n) {
  int leftHeight = -1, rightHeight = -1;

  if ( n->getLeft() != NULL ) leftHeight = n->getLeft()->getHeight();
  if ( n->getRight() != NULL ) rightHeight = n->getRight()->getHeight();
  n->setHeight( 1 + get_max(leftHeight, rightHeight) );
}

template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::updateHeightBottomUp(AVLNode<KeyType, ValueType>* n) { 
  while ( n ) {
    int oldHeight = n->getHeight(), newHeight;
    updateHeight(n);
    newHeight = n->getHeight();
    if ( oldHeight == newHeight ) break;
    n = n->getParent();
  }
}

// precondition: toInPlace must be a leaf node ?? No
template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::replace(Node<KeyType, ValueType>* toInPlace, Node<KeyType, ValueType>* toRemove) {
  if ( toInPlace->getLeft() && toInPlace->getRight() ) throw "node to be in place should at most have a child";

  Node<KeyType, ValueType>* ip = toInPlace->getParent(); // parent of node to be in place
  Node<KeyType, ValueType>* rp = toRemove->getParent(); // parent of node to be removed

  if ( ip == toRemove ) throw "node to be in place should not be a child of the node to be removed";

  // update the info of right child of node to be in place
  if ( toInPlace->getLeft() ) shiftUpwards( toInPlace->getLeft() );
  else ip->setRight(NULL); // simply remove the node to be in place

  // update parent of the replacer
  toInPlace->setParent(rp);
  if ( rp ) {
    if ( rp->getLeft() == toRemove ) rp->setLeft(toInPlace);
    else rp->setRight(toInPlace);
  }

  // take the children of the toRemove node
  toInPlace->setLeft( toRemove->getLeft() );
  if ( toRemove->getLeft() ) toRemove->getLeft()->setParent(toInPlace);
  toInPlace->setRight( toRemove->getRight() );
  if ( toRemove->getRight() ) toRemove->getRight()->setParent(toInPlace);

  if ( this->root == toRemove ) 
    this->root = toInPlace;
  // delete toRemove;
}

// shift upwards toShift, ? delete its origin parent ?
template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>:: shiftUpwards(Node<KeyType, ValueType>* toShift) {
  Node<KeyType, ValueType>* sp = toShift->getParent(); // parent node to shift, which also is node to be deleted
  Node<KeyType, ValueType>* spp = sp->getParent();

  toShift->setParent(spp);
  if ( spp ) { // sp is not the root
      if ( spp->getLeft() == sp ) spp->setLeft(toShift);
      else spp->setRight(toShift);
  }
  else this->root = toShift;
  // delete sp;
}

/* delete a BST node, return the first possible unbalanced node after deletion */
template <class KeyType, class ValueType>
Node<KeyType, ValueType>* AVLTree<KeyType, ValueType>::deleteBSTNode(Node<KeyType, ValueType>* toDelete) {
  Node<KeyType, ValueType>* ret;

  if ( toDelete->getLeft() == NULL && toDelete->getRight() == NULL ) { // leaf node
    ret = toDelete->getParent();
    if ( ret ) {
      if ( ret->getLeft() == toDelete ) ret->setLeft(NULL);
      else ret->setRight(NULL);
    }
    else this->root = NULL; // case where toDelete is the only node in the tree
  }
  else if ( toDelete->getLeft() == NULL ) { // only has right child
    ret = toDelete->getRight();
    shiftUpwards(ret);
  }
  else if ( toDelete->getRight() == NULL ) { // only has left child
    ret = toDelete->getLeft();
    shiftUpwards(ret);
  }
  else {
    // find the previous largest (maximum in left sub-tree)
    Node<KeyType, ValueType>* curr = toDelete->getLeft();
    while ( curr->getRight() ) curr = curr->getRight();
    // (left side) case where the child of delete node shift upwards to replace it
    if ( curr->getParent() == toDelete ) {
      ret = curr;
      shiftUpwards(ret);
      ret->setRight( toDelete->getRight() );
      toDelete->getRight()->setParent(ret);
    }
    else {
      ret = curr->getParent();
      replace(curr, toDelete);      
    }
  }
  delete toDelete;

  return ret;
}

class KeyNotExistsException : public std::exception {
  public:
    std::string what() {
      std::string message = "Key does not exist\n";
      return message;
    }
};

class KeyAlreadyExistsException : public std::exception {
  public:
    std::string what() {
      std::string message = "Key already exist\n";
      return message;
    }
};
#endif
