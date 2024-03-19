#ifndef RECCHECK
#include <algorithm>
#include <iostream>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node* node) {
  if (node == NULL) {
    return 0;
  }
  int left_height = getHeight(node->left);
  int right_height = getHeight(node->right);

  return 1 + std::max(left_height, right_height);
}
 
bool equalPaths(Node * root)
{
    // Add your code below
    if (root == NULL) {
      return true;
    }

    int left_height = getHeight(root->left);
    int right_height = getHeight(root->right);

    if (root->left && root->right) {
      return (left_height==right_height) && equalPaths(root->left) && equalPaths(root->right);
    }

    return ((root->left == NULL && equalPaths(root->right))
           || (root->right == NULL && equalPaths(root->left)));
}

// bool arePathsEqual (Node* node, int length, int check_length) {
//   if (node == NULL) {
//     return length == check_length;
//   }


//   if (node->left != NULL) {
//     if (!arePathsEqual(node->left, length + 1, check_length)) {
//       return false;
//     }
//   }

//   if (node->right != NULL) {
//     if (!arePathsEqual(node->right, length + 1, check_length)) {
//       return false;
//     }
//   }
//   return true;
// }

// bool equalPaths(Node* root) {
//   if (root == NULL) {
//     return true;
//   }

//   int check_length = 0;
//   Node* current = root;
//   while(current != NULL) {
//     check_length++;
//     current = current->left;
//   }
//   return arePathsEqual(root, 0, check_length);
// }