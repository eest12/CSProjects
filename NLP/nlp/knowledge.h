#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H
#include "sent.h"
#include "nouns.h"

class Knowledge //database of gathered info and inferences
{
public:
    Knowledge(){}
    string ProcessQuestion(Sent ques); //compares question to stored sentences and returns possible answer
    void AddFact(Sent sent); //adds new sentence items to existing Sent object in "known", or creates a new one if "sent" is a new verb
    void Print(); //prints everything stored in "known"

private:
    vector<Sent> known;
    void Inferences(Sent sent); //adds to "known" based on predefined inferences for certain verbs
};

#endif // KNOWLEDGE_H
