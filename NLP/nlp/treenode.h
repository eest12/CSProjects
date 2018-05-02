#ifndef TREENODE
#define TREENODE
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct TreeNode
{
    TreeNode(){}
    TreeNode(T i){item = i;}
    bool IsLeaf(){return children.size() == 0;}

    T item; //term or nonterm string (eg. S, NP, apple)
    //TreeNode<T>** children;
    vector<TreeNode<T>*> children;

    friend ostream& operator << (ostream& outs, const TreeNode<T>& tn){
        outs << tn.item;
        return outs;
    }
};

#endif // TREENODE

