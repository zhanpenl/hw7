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

#define max(a,b) ( (a) > (b) ? (a) : (b) )

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
      std::cout << "insert: first unbalanced " << z->getKey() << ", " << y->getKey()
      << ", " << x->getKey() << std::endl;
      rotate(z, y, x);
    }
  }
  /* This one is yours to implement.
     It should insert the (key, value) pair to the tree, 
     making sure that it remains a valid AVL Tree.
     If the key is already in the tree, you should throw an exception. */
  void remove (const KeyType &toRemove) {
    Node<KeyType, ValueType> *remove_node = this->internalFind(toRemove);
    if ( remove_node == NULL ) throw "Key does not exit";


  }
  /* Implement this as well.  It should remove the (key, value) pair from
   * the tree which corresponds to the input parameter.
   * If the key is not in the tree, you should throw an exception.
   */

private:
  void insertHelper (const std::pair<const KeyType, ValueType>& new_item, AVLNode<KeyType, ValueType>* root);
  Node<KeyType, ValueType>* findFirstUnbalanced (Node<KeyType, ValueType>* new_node, 
      Node<KeyType, ValueType>*& y, Node<KeyType, ValueType>*& x);
  void rotate (Node<KeyType, ValueType>* z, Node<KeyType, ValueType>* y, Node<KeyType, ValueType>* x);
  void rotateLeft(Node<KeyType, ValueType>* n);
  void rotateRight(Node<KeyType, ValueType>* n);
  void updateHeight(AVLNode<KeyType, ValueType>* n);
  void updateHeightBottomUp(AVLNode<KeyType, ValueType>* n);
  void replace(Node<KeyType, ValueType>* toInPlace, Node<KeyType, ValueType>* toRemove);
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
  else throw "Key Already Exists";
}

template <class KeyType, class ValueType>
Node<KeyType, ValueType>* AVLTree<KeyType, ValueType>::findFirstUnbalanced (Node<KeyType, ValueType>* new_node,
      Node<KeyType, ValueType>*& y, Node<KeyType, ValueType>*& x) {
  Node<KeyType, ValueType>* z = new_node;
  y = x = NULL;

  while ( z ) {
    updateHeight( static_cast<AVLNode<KeyType,ValueType>*>(z) ); // udate heights bottom up

    int leftHeight = -1, rightHeight = -1, diff;
    if ( z->getLeft() != NULL ) 
      leftHeight = (static_cast<AVLNode<KeyType,ValueType>*>(z->getLeft()))->getHeight();
    if ( z->getRight() != NULL ) 
      rightHeight = (static_cast<AVLNode<KeyType,ValueType>*>(z->getRight()))->getHeight();
    diff = leftHeight - rightHeight;
    std::cout << diff << "(" << z->getValue() << ") ";
    if ( diff < -1 or diff > 1 ) return z;
    x = y;
    y = z;
    z = z->getParent();
  }

  return NULL;
}

template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::rotate (Node<KeyType, ValueType>* z, Node<KeyType, ValueType>* y,
    Node<KeyType, ValueType>* x) {
  KeyType kz = z->getKey(), ky = y->getKey(), kx = x->getKey();
  // Node<KeyType, ValueType> *zp = z->getParent();

  // case 1
  if ( kz < ky && ky < kx ) {
    rotateLeft(y);
  }
  // case 2
  if ( kz > ky && ky > kx ) {
    rotateRight(y);
  }
  // case 3
  if ( kz < kx && kx < ky ) {
    rotateRight(x);
    rotateLeft(x);    
  }
  // case 4
  if ( kz > kx && kx > ky ) {
    rotateLeft(x);
    rotateRight(x);
  }
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
  n->setHeight( 1 + max(leftHeight, rightHeight) );
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

// precondition: toInPlace must be a leaf node ??
template <class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::replace(Node<KeyType, ValueType>* toInPlace, Node<KeyType, ValueType>* toRemove) {
  Node<KeyType, ValueType>* ip = toInPlace->getParent(); // parent of node to be in place
  Node<KeyType, ValueType>* rp = toRemove->getParent(); // parent of node to be removed

  if ( ip->getLeft() == toInPlace ) ip->setLeft(NULL);
  else ip->setRight(NULL);

  toInPlace->setParent(rp);
  if ( rp->getLeft() == toRemove ) rp->setLeft(toInPlace);
  else rp->setRight(toInPlace);

  toInPlace->setLeft( toRemove->getLeft() );
  if ( toRemove->getLeft() ) toRemove->getLeft()->setParent(toInPlace);

  toInPlace->setRight( toRemove->getRight() );
  if ( toRemove->getRight() ) toRemove->getRight()->setParent(toInPlace);

  delete toRemove;
}

#endif
