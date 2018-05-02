#ifndef PARSETREE_H
#define PARSETREE_H
#include <iostream>
#include "treenode.h"
#include "token.h"
#include <vector>
#include <iomanip>
#include "constants.h"
#include "sent.h"

using namespace std;

class ParseTree
{
public:
    ParseTree();
    ParseTree(vector<Token> sentence);
    void SetSentence(vector<Token> sentence);
    void ParseDeclarative();
    void ParseInterrogative();
    void Print();
    Sent ReducedSentence(); //stores only the verb and the subject & objects into a Sent class object

private:
    vector<Token> sent;
    TreeNode<string>* root;

    TreeNode<string>* Find(string label, TreeNode<string> *node); //finds a specified non-terminal in the tree
    void Print(int level, TreeNode<string>* node);

    //BNF functions
    TreeNode<string>* Sentence(int& index);
    TreeNode<string>* Subject(int& index);
    TreeNode<string>* Predicate(int& index);
    TreeNode<string>* SimpleSubject(int& index);
    TreeNode<string>* CompundSubject(int& index);
    TreeNode<string>* NounPhrase(int& index);
    TreeNode<string>* Noun(int& index);
    TreeNode<string>* Article(int& index);
    TreeNode<string>* Pronoun(int& index);
    TreeNode<string>* Possessive(int& index);
    TreeNode<string>* VerbPhrase(int& index);
    TreeNode<string>* Verb(int& index);
    TreeNode<string>* Complement(int& index);
    TreeNode<string>* Conjunction(int& index);
    TreeNode<string>* Adverb(int& index);
    TreeNode<string>* Auxiliary(int& index);
    TreeNode<string>* Preposition(int& index);
    TreeNode<string>* Adjective(int& index);
    TreeNode<string>* PrepositionalPhrase(int& index);
    TreeNode<string>* Object(int& index);
    TreeNode<string>* SimpleObject(int& index);
    TreeNode<string>* CompoundObject(int& index);
    TreeNode<string>* IndirectObject(int& index);
    TreeNode<string>* PrepositionalObject(int& index);
};

#endif // PARSETREE_H
