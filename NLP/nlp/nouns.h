#ifndef NOUNS
#define NOUNS
#include <iostream>

using namespace std;

struct Nouns //subject and objects that are part of a sentence; to be used with Sent class, which holds the verb
{
    Nouns(){}
    bool SemiEquivalent(Nouns n) //true if all sub/objects match but possibly one class object has more objects than the other
    {
        if (subject != n.subject && (subject.size() != 0 && n.subject.size() != 0)) //strings aren't empty and aren't the same
            return false;
        if (direct_obj != n.direct_obj && (direct_obj.size() != 0 && n.direct_obj.size() != 0))
            return false;
        if (indirect_obj != n.indirect_obj && (indirect_obj.size() != 0 && n.indirect_obj.size() != 0))
            return false;
        if (prep_obj != n.prep_obj && (prep_obj.size() != 0 && n.prep_obj.size() != 0))
            return false;
        return true; //each non-empty string is the same as n's strings
    }

    friend ostream& operator << (ostream& outs, const Nouns& n){
        outs << "[s:" << n.subject << "] [do:" << n.direct_obj << "] [io:" << n.indirect_obj << "] [po:" << n.prep_obj << "]";
        return outs;
    }

    string subject;
    string direct_obj;
    string indirect_obj;
    string prep_obj;
};

#endif // NOUNS
