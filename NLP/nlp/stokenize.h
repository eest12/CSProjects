//will provide an interface to get the next token from a string object.
#ifndef STOKENIZE_H
#define STOKENIZE_H
#include "token.h"
#include "constants.h"
#include <fstream>

class STokenize
{
public:
    STokenize();
    STokenize(const string s);
    void SetStr(const string s);
    Token NextToken(); //returns next token (word) in the string
    bool More(); //returns false if there are no more words to be tokenized
    int Pos(); //position of current token in string
    string PartOfSpeech(string word); //looks for word in a dictionary and returns its parts of speech

private:
    string str;
    int pos;
    ifstream f;

    bool LessThan(string word1, string word2); //helps PartOfSpeech determine if a term appears before or after current spot
};

#endif // STOKENIZE_H
