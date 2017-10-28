// Process Tree Header File
// A tree is chosen here to organize processes by priority,
// where the priority is assumed (for simplicity) to simply
// be identical to the process identifier (a small integer)
//
// The caller will simply be recording the process ID's in this tree,
// and choose the maximum value to favor processes with larger ID's.

#include <iostream>
#include <string>
using namespace std;

// A node anywhere in tree
class ProcTreeNode
{
    friend ostream& operator<<( ostream&, ProcTreeNode & );
    friend class ProcTree;
private:
    int  procID;		// process this applies to
    ProcTreeNode *left,	// sub-tree for less than
    *right;	// sub-tree for greater than
    
    // Private constructor: only for use by ProcTree
    ProcTreeNode( int id ) : procID(id), left(NULL), right(NULL) { }
};

// And a mechanism to store and retrieve entries in the tree.
class ProcTree
{
    friend ostream& operator<<( ostream&, ProcTree & );
private:
    ProcTreeNode *root;
public:
    ProcTree() : root(NULL) { }
    void insert( int );
    int removeMax();
    bool empty() { return root == NULL; }
    
private:		// these just help ProcTree do its job
    void recursiveInsert( ProcTreeNode *&, int );
    int recursiveRemoveMax( ProcTreeNode *& );
};
