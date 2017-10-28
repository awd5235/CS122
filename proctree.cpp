// Process Tree Implementation File
// A tree is chosen here to organize processes by priority,
// where the priority is assumed (for simplicity) to simply
// be identical to the process identifier (a small integer)
//
// The caller will simply be recording the process ID's in this tree,
// and choose the maximum value to favor processes with larger ID's.
//
#include <iostream>
#include <string>
using namespace std;

#include "proctree.h"

//  recursiveInsert
//  A recursive tree-traversal function to insert a new value
//  If the value is already present, no change is made
//  Parameters:
//  	node	(modified TreeNode ptr)	root of sub-tree to search
//  	val	(input int)		value to insert
//  NOTE: node May be a null pointer, indicating an empty tree
//  Post-condition:
//      return value points at a node with the given name,
//      even if it had to be created to do so
void ProcTree::recursiveInsert( ProcTreeNode *&node, int val )
{
    if (node == NULL)  //empty tree
    {
        node = new ProcTreeNode(val);
    }
    else if (val == node->procID){}     //inserting duplicate, do nothing
    else if (val < node->procID)        //proceed to the left
    {
        recursiveInsert( node->left, val );
    }
    else                                //proceed to the right
    {
        recursiveInsert( node->right, val );
    }
}

//  insert
//  Inserts a new value into the binary search tree
//  Parameters:
//  	val 	(input int)		value to insert
void ProcTree::insert( int val )
{
    recursiveInsert( root, val );
}

//  recursiveRemoveMax
//  A recursive tree-traversal function to identify and remove
//  a maximum value from a binary search tree
//  Parameters:
//  	node	(modified TreeNode ptr)	root of sub-tree to search
//  	val	(input int)		value to insert
//  Pre-condition:
//      The tree is assumed to be non-empty
int ProcTree::recursiveRemoveMax( ProcTreeNode *&nodel )
{
    //the max value is always the right most node in the tree
    
    int val;
    ProcTreeNode* rightChild = nodel->right;  //right child of nodel
    
    //if right child is null
    if(rightChild == NULL)
    {
        val = nodel->procID;        //assign value to return
        rightChild = nodel;              //avoid memory leak
        nodel = nodel->left;             //left child is new root of tree
        delete rightChild;               //remove the only node
        return val;
    }
    //if right most node in tree
    else if(rightChild->right == NULL)
    {
        nodel->right = rightChild->left; //connect nodel's right child to child's left child
        val = rightChild->procID;        //obtain the max value from child
        delete rightChild;               //delete child
        return val;
    }
    //if some intermediary node
    else
        return recursiveRemoveMax(nodel->right);
    
}

//  removeMax
//  identifies and removes the largest value in the tree
int ProcTree::removeMax()
{
    return recursiveRemoveMax( root );
}


ostream& operator<<( ostream& stream, ProcTreeNode &t )
{
    if (t.left != NULL)	stream << *t.left;
    stream << t.procID << " ";
    if (t.right != NULL) stream << *t.right;
    return stream;
}

ostream& operator<<( ostream& stream, ProcTree &t )
{
    return stream << *t.root;
}
