#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key, Value>* grandparent);
    void rotateLeft(AVLNode<Key, Value>* grandparent);

    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    const Key& searchKey = new_item.first;
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

    if (this->root_ == NULL) {
        this->root_ = newNode;
        return;
    }

    AVLNode<Key,Value>* find = static_cast<AVLNode<Key,Value>*>(this->internalFind(searchKey));
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key,Value>*>(this->root_);
    
    if (find) {
        find->setValue(new_item.second);
        return;
      } else {
          while (parent) {
            if (searchKey < parent->getKey()) {
              if (parent->getLeft() != NULL) {
                parent = parent->getLeft();
              } else {
                parent->setLeft(newNode);
                newNode->setParent(parent);
                break;
              }
            }
            if (searchKey > parent->getKey()) {
              if (parent->getRight() != NULL) {
                parent = parent->getRight();
              } else {
                parent->setRight(newNode);
                newNode->setParent(parent);
                break;
              }
            }
         }
      }

      newNode->setBalance(0);
      if (parent->getBalance() == -1 || parent->getBalance() == 1) {
          parent->setBalance(0);
          return;

      } else if (!parent->getBalance()) {
          if (newNode == parent->getLeft()) {
              parent->setBalance(-1);
          } else {
            parent->setBalance(1);
          }
          insertFix(parent, newNode);
      }
  }


template <class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key, Value>* parent, AVLNode<Key,Value>* node) 
{
    // if tree is empty or there is only one node ?
    if (parent == NULL || parent->getParent() == NULL) {
        return;
    } else {
        AVLNode<Key,Value>* grandparent = parent->getParent();

        // if parent is left child of grandparent
        if (parent == grandparent->getLeft()) {
            grandparent->setBalance(grandparent->getBalance() - 1);
            if (grandparent->getBalance() == 0) {
                return;
            } else if (grandparent->getBalance() == -1) {
                insertFix(grandparent, parent);
            } else if (grandparent->getBalance() == -2) {
                if (node == node->getParent()->getParent()->getLeft()->getLeft()) {
                    rotateRight(grandparent);
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else {
                    rotateLeft(parent);
                    rotateRight(grandparent);
                    if (node->getBalance() == -1) {
                        parent->setBalance(0);
                        grandparent->setBalance(1);
                        node->setBalance(0);
                    } else if (node->getBalance() == 0) {
                        parent->setBalance(0);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    } else if (node->getBalance() == 1) {
                        parent->setBalance(-1);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                }
            }
        } else {
          // if parent is right child of grandparent
            grandparent->setBalance(grandparent->getBalance() + 1);
            if (grandparent->getBalance() == 0) {
                return;
            } else if (grandparent->getBalance() == 1) {
                insertFix(grandparent, parent);
            } else if (grandparent->getBalance() == 2) {
                if (node == node->getParent()->getParent()->getRight()->getRight()) {
                    rotateLeft(grandparent);
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else {
                    rotateRight(parent);
                    rotateLeft(grandparent);
                    if (node->getBalance() == 1) {
                        parent->setBalance(0);
                        grandparent->setBalance(-1);
                        node->setBalance(0);
                    } else if (node->getBalance() == 0) {
                        parent->setBalance(0);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    } else if (node->getBalance() == -1) {
                        parent->setBalance(1);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                }
            }
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* grandparent) {
    AVLNode<Key, Value>* greatgrandparent = grandparent->getParent();
    AVLNode<Key, Value>* parent = grandparent->getLeft();
    AVLNode<Key, Value>* child = parent->getRight();

    parent->setParent(greatgrandparent);
    if (greatgrandparent != NULL) {

        if (grandparent == greatgrandparent->getRight()) {
          greatgrandparent->setRight(parent);
          parent->setRight(grandparent);
          grandparent->setLeft(child);
          grandparent->setParent(parent);
        } else if (grandparent == greatgrandparent->getLeft()) {
          greatgrandparent->setLeft(parent);
          parent->setRight(grandparent);
          grandparent->setParent(parent);
          grandparent->setLeft(child);
        }

    } else {
        this->root_ = parent;
        parent->setRight(grandparent);
        grandparent->setParent(parent);
        grandparent->setLeft(child);
    }
  if (child != NULL) {
    child->setParent(grandparent);
  }
}




template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* grandparent) {
    AVLNode<Key, Value>* greatgrandparent = grandparent->getParent();
    AVLNode<Key, Value>* parent = grandparent->getRight();
    AVLNode<Key, Value>* child = parent->getLeft();

    parent->setParent(greatgrandparent);
    if (greatgrandparent != NULL) {
        if (grandparent == greatgrandparent->getRight()) {
          greatgrandparent->setRight(parent);
          parent->setLeft(grandparent);
          grandparent->setParent(parent);
          grandparent->setRight(child);
        } else if (grandparent == greatgrandparent->getLeft()) {
          greatgrandparent->setLeft(parent);
          parent->setLeft(grandparent);
          grandparent->setParent(parent);
          grandparent->setRight(child);
        }
    } else {
      this->root_ = parent;
      parent->setLeft(grandparent);
      grandparent->setParent(parent);
      grandparent->setRight(child);
    }

    if (child != NULL) {
      child->setParent(grandparent);
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    int8_t diff(0);
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if (n == NULL) {
        return;
    }



    // if node has 2 children
    if (n->getLeft() != NULL && n->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(n));
        nodeSwap(n, pred);
    }

    AVLNode<Key, Value>* parent = n->getParent();
    AVLNode<Key, Value>* child = NULL;

    if (parent != NULL) {
         if (n == parent->getLeft()) {
          diff = 1;
         } else {
          diff = -1;
         }
    } 

    // check if there is 1 or 0 children
    if (n->getRight() != NULL) {
        child = n->getRight();
    } else if (n->getLeft() != NULL) {
        child = n->getLeft();
    }

    // if node is leaf node
    if (child == NULL) {
        if (parent == NULL) {
            this->root_ = NULL;
        } else if (n == parent->getRight()) {
          parent->setRight(child);
        } else if (n == parent->getLeft()) {
          parent->setLeft(child);
        }
    } else {
      // if node has one child
      if (parent == NULL) {
        this->root_ = child;
        child->setParent(NULL);
      } else if (n == parent->getRight()) {
        child->setParent(parent);
        parent->setRight(child);
      } else if (n == parent->getLeft()) {
        child->setParent(parent);
        parent->setLeft(child);
      }
    }

    delete n;
    removeFix(parent, diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff) {
  if (node == NULL) {
    return;
  } 
  AVLNode<Key, Value>* parent = node->getParent();
  int8_t ndiff = 0;

  if (parent != NULL) {
    if (node == parent->getLeft()) {
        ndiff = 1;
    } else {
      ndiff = -1;
    }
  }

  if (diff == -1) {
    if (node->getBalance() + diff == -2) {
        AVLNode<Key, Value>* child = node->getLeft();
        
        if (child->getBalance() == -1) {
            rotateRight(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, ndiff);
        } else if (child->getBalance() == 0) {
            rotateRight(node);
            node->setBalance(-1);
            child->setBalance(1);
            return;
        } else if (child->getBalance() == 1) {
            AVLNode<Key,Value>* grandparent = child->getRight();
            rotateLeft(child);
            rotateRight(node);

            if (grandparent->getBalance() == 1) {
              node->setBalance(0);
              child->setBalance(-1);
              grandparent->setBalance(0);
            } else if (grandparent->getBalance() == 0) {
              node->setBalance(0);
              child->setBalance(0);
              grandparent->setBalance(0);
            } else if (grandparent->getBalance() == -1) {
              node->setBalance(1);
              child->setBalance(0);
              grandparent->setBalance(0);
            }

            removeFix(parent, ndiff);
       }
    } else if (node->getBalance() + diff == -1) {
        node->setBalance(-1);
        return;
    } else if (node->getBalance() + diff == 0) {
        node->setBalance(0);
        removeFix(parent, ndiff);
      }
  }

  else if (diff == 1) {
      if (node->getBalance() + diff == 2) {
        AVLNode<Key, Value>* child = node->getRight();
        
        if (child->getBalance() == 1) {
            rotateLeft(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, ndiff);
        } else if (child->getBalance() == 0) {
            rotateLeft(node);
            node->setBalance(1);
            child->setBalance(-1);
            return;
        } else if (child->getBalance() == -1) {
            AVLNode<Key,Value>* grandparent = child->getLeft();
            rotateRight(child);
            rotateLeft(node);

            if (grandparent->getBalance() == -1) {
              node->setBalance(0);
              child->setBalance(1);
              grandparent->setBalance(0);
            } else if (grandparent->getBalance() == 0) {
              node->setBalance(0);
              child->setBalance(0);
              grandparent->setBalance(0);
            } else if (grandparent->getBalance() == 1) {
              node->setBalance(-1);
              child->setBalance(0);
              grandparent->setBalance(0);
            }

            removeFix(parent, ndiff);
       }
    } else if (node->getBalance() + diff == 1) {
        node->setBalance(1);
        return;
    } else if (node->getBalance() + diff == 0) {
        node->setBalance(0);
        removeFix(parent, ndiff);
      }
  }
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
