#include "knowledge.h"

string Knowledge::ProcessQuestion(Sent ques){
    //look for same verb in "known"
    int index = 0;
    while (index < known.size() && ques.verb != known[index].verb)
        index++;
    if (index < known.size()) //found verb
    {
        int j = 0;
        while (j < known.size() && !ques.nouns[0].SemiEquivalent(known[index].nouns[j])) //looking for correct set of nouns
            j++;
        if (j < known.size()) //found equivalent set of nouns
        {
            //return item missing from question
            if (ques.nouns[0].subject.empty() && !known[index].nouns[j].subject.empty())
                return known[index].nouns[j].subject;
            if (ques.nouns[0].direct_obj.empty() && !known[index].nouns[j].direct_obj.empty())
                return known[index].nouns[j].direct_obj;
            if (ques.nouns[0].indirect_obj.empty() && !known[index].nouns[j].indirect_obj.empty())
                return known[index].nouns[j].indirect_obj;
            if (ques.nouns[0].prep_obj.empty() && !known[index].nouns[j].prep_obj.empty())
                return known[index].nouns[j].prep_obj;
        }
    }

    return "unknown";
}

void Knowledge::AddFact(Sent sent){
    bool exists = false;
    for (int i = 0; i < known.size() && !exists; i++)
    {
        if (sent.verb == known[i].verb)
        {
            known[i].nouns.push_back(sent.nouns[0]); //add new info to existing verb in "known"
            exists = true;
        }
    }
    if (!exists)
        known.push_back(sent); //add new verb to "known"
    Inferences(sent);
}

void Knowledge::Inferences(Sent sent){
    //inferences: give/have, kill/die

    if (sent.verb == "give" || sent.verb == "gives" || sent.verb == "gave")
    {
        Sent inference("have");
        Nouns n;
        n.subject = sent.nouns[0].indirect_obj;
        n.direct_obj = sent.nouns[0].direct_obj;
        inference.nouns.push_back(n);
        known.push_back(inference);

        Sent inference2("has");
        Nouns n2;
        n2.subject = sent.nouns[0].indirect_obj;
        n2.direct_obj = sent.nouns[0].direct_obj;
        inference2.nouns.push_back(n2);
        known.push_back(inference2);

        Sent inference3("had"); //had is different because possession changes when something is given
        Nouns n3;
        n3.subject = sent.nouns[0].subject;
        n3.direct_obj = sent.nouns[0].direct_obj;
        inference3.nouns.push_back(n3);
        known.push_back(inference3);
    }
    else if (sent.verb == "kill" || sent.verb == "kills" || sent.verb == "killed")
    {
        Sent inference("die");
        Nouns n;
        n.subject = sent.nouns[0].direct_obj;
        inference.nouns.push_back(n);
        known.push_back(inference);

        Sent inference2("dies");
        Nouns n2;
        n2.subject = sent.nouns[0].direct_obj;
        inference2.nouns.push_back(n2);
        known.push_back(inference2);

        Sent inference3("died");
        Nouns n3;
        n3.subject = sent.nouns[0].direct_obj;
        inference3.nouns.push_back(n3);
        known.push_back(inference3);
    }
}

void Knowledge::Print(){
    for (int i = 0; i < known.size(); i++)
    {
        cout << known[i].verb << " -> ";
        for (int j = 0; j < known[i].nouns.size(); j++)
            cout << known[i].nouns[j] << " || ";
        cout << endl;
    }
}
