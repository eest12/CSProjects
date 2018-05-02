#ifndef SENT_H
#define SENT_H
#include <iostream>
#include <vector>
#include "nouns.h"

using namespace std;

struct Sent //stores a verb and groups of subject/objects that belong to each sentence with that verb
{
    Sent(){}
    Sent(string v){verb = v;}

    string verb;
    vector<Nouns> nouns; //list of subjects and objects

    friend ostream& operator << (ostream& outs, const Sent& sent){
        outs << sent.verb << " -> ";
        for (int i = 0; i < sent.nouns.size(); i++)
            cout << sent.nouns[i] << " ";
        return outs;
    }
};

#endif // SENT_H
