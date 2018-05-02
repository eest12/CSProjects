#include "parsetree.h"

ParseTree::ParseTree(){
    root = NULL;
}

ParseTree::ParseTree(vector<Token> sentence){
    sent = sentence;
    root = NULL;
}

void ParseTree::SetSentence(vector<Token> sentence){
    sent = sentence;
}

void ParseTree::ParseDeclarative(){
    int index = 0;
    root = Sentence(index);
}

void ParseTree::Print(){
    Print(1, root);
    cout << endl;
}

void ParseTree::Print(int level, TreeNode<string>* node){
    if (node != NULL)
    {
        if (!node->IsLeaf())
            cout<<" "<<setw(level)<<":"<<setw(3)<<level<<" ["<<node->item<<"]"<<endl;
        else
            cout<<" "<<setw(level)<<":"<<setw(3)<<level<<" \""<<node->item<<"\""<<endl;
        for (int i = 0; i < node->children.size(); i++)
            Print(level+1, node->children[i]);
    }
}

TreeNode<string>* ParseTree::Sentence(int& index){
    TreeNode<string>* sent = new TreeNode<string>("Sent");
    //<subject> <predicate>
    sent->children.push_back(Subject(index));
    sent->children.push_back(Predicate(index));
    return sent;
}

TreeNode<string>* ParseTree::Subject(int& index){
    //<simple subject> | <compound subject>
    TreeNode<string>* subj = NULL;
    TreeNode<string>* subj2 = CompundSubject(index);
    if (subj2 == NULL)
        subj2 = SimpleSubject(index);
    if (subj2 != NULL)
    {
        subj = new TreeNode<string>("Subj");
        subj->children.push_back(subj2);
    }
    return subj;
}

TreeNode<string>* ParseTree::Predicate(int& index){
    TreeNode<string>* pred = NULL;
    if (index < sent.size())
    {
        TreeNode<string>* verb = VerbPhrase(index);
        if (verb == NULL)
            verb = Verb(index);
        pred = new TreeNode<string>("Pred");
        pred->children.push_back(verb);
        if (index < sent.size())
            pred->children.push_back(Complement(index));
    }
    return pred;
}

TreeNode<string>* ParseTree::SimpleSubject(int& index){
    TreeNode<string>* ssubj = NULL;
    if (index < sent.size())
    {
        //<noun phrase> |
        ssubj = NounPhrase(index);
        if (ssubj != NULL)
            return ssubj;
        // | <nominative personal pronoun>
        if (sent[index].pos == NOMINATIVE)
        {
            ssubj = new TreeNode<string>("Simple Subj");
            ssubj->children.push_back(Pronoun(index));
        }
    }
    return ssubj;
}

TreeNode<string>* ParseTree::CompundSubject(int& index){
    TreeNode<string>* csubj = NULL;
    if (index < sent.size())
    {
        int temp = index;
        if (SimpleSubject(index) != NULL && (sent[index].token == "and" || sent[index].token == "or") && SimpleSubject(++index) != NULL)
        {
            index = temp;
            csubj = new TreeNode<string>("Compund Subj");
            csubj->children.push_back(SimpleSubject(index));
            csubj->children.push_back(Conjunction(index));
            csubj->children.push_back(SimpleSubject(index));
            return csubj;
        }
        index = temp;
        if (sent[index].token == "Either" && SimpleSubject(++index) != NULL && sent[index].token == "or" && SimpleSubject(++index) != NULL)
        {
            index = temp;
            csubj = new TreeNode<string>("Compound Subj");
            csubj->children.push_back(Conjunction(index));
            csubj->children.push_back(SimpleSubject(index));
            csubj->children.push_back(Conjunction(index));
            csubj->children.push_back(SimpleSubject(index));
            return csubj;
        }
        index = temp;
        if (sent[index].token == "Neither" && SimpleSubject(++index) != NULL && sent[index].token == "nor" && SimpleSubject(++index) != NULL)
        {
            index = temp;
            csubj = new TreeNode<string>("Compound Subj");
            csubj->children.push_back(Conjunction(index));
            csubj->children.push_back(SimpleSubject(index));
            csubj->children.push_back(Conjunction(index));
            csubj->children.push_back(SimpleSubject(index));
            return csubj;
        }
        index = temp;
    }
    return csubj;
}

TreeNode<string>* ParseTree::NounPhrase(int& index){
    TreeNode<string>* nphrase = NULL;
    if (index < sent.size())
    {
        int temp = index;
        //"the" <specific proper noun>
        if (sent[index].pos == DEF_ARTICLE && Noun(++index) != NULL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Article(index));
            nphrase->children.push_back(Noun(index));
            return nphrase;
        }
        index = temp;
        if (Noun(index) != NULL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Noun(index));
            return nphrase;
        }
        if (Pronoun(index) != NULL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Pronoun(index));
            return nphrase;
        }
        if (Article(index) != NULL && Noun(index) != NULL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Article(index));
            nphrase->children.push_back(Noun(index));
            return nphrase;
        }
        index = temp;
        if (sent[index].pos == PLURAL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Noun(index));
            return nphrase;
        }
        if (sent[index].pos == POSSESSIVE && Noun(++index) != NULL)
        {
            index = temp;
            nphrase = new TreeNode<string>("Noun Phr");
            nphrase->children.push_back(Possessive(index));
            nphrase->children.push_back(Noun(index));
            return nphrase;
        }
        index = temp;
    }
    return nphrase;
}

TreeNode<string>* ParseTree::Noun(int& index){
    TreeNode<string>* noun = NULL;
    if (index < sent.size())
    {
        //<noun>
        if (sent[index].pos == NOUN || sent[index].pos == PLURAL)
        {
            noun = new TreeNode<string>("Noun");
            noun->children.push_back(new TreeNode<string>(sent[index].token));
            index++;

            //[<prep phr>*]
                TreeNode<string>* prep = PrepositionalPhrase(index);
                while (prep != NULL)
                {
                    noun->children.push_back(prep);
                    prep = PrepositionalPhrase(index);
                }
        }
    }
    return noun;
}

TreeNode<string>* ParseTree::Article(int& index){
    TreeNode<string>* art = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == DEF_ARTICLE || sent[index].pos == INDEF_ARTICLE)
        {
            art = new TreeNode<string>("Artic");
            art->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return art;
}

TreeNode<string>* ParseTree::Pronoun(int& index){
    TreeNode<string>* pnoun = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == PRONOUN || sent[index].pos == NOMINATIVE)
        {
            pnoun = new TreeNode<string>("Pron");
            pnoun->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return pnoun;
}

TreeNode<string>* ParseTree::Possessive(int& index){
    TreeNode<string>* posses = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == POSSESSIVE)
        {
            posses = new TreeNode<string>("Posses");
            posses->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return posses;
}

TreeNode<string>* ParseTree::VerbPhrase(int& index){
    TreeNode<string>* vphrase = NULL;
    if (index < sent.size())
    {
        int temp = index;
        //("had" |"have" |"has")...
        if (sent[index].token == "had" || sent[index].token == "have" || sent[index].token == "has")
        {
            index++;
            //...["not"]...
            if (sent[index].token == "not")
                index++;
            //...<Vpastp>
            if (sent[index].token != "been" && Verb(index) != NULL)
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //had/have/has
                if (i - temp == 3)
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //Verb
                return vphrase;
            }
            //..."been" [<Vpastp> | <Ving>]
            if (sent[index].token == "been")
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //had/have/has
                if (i - temp == 2)
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //been
                vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                return vphrase;
            }
        }
        index = temp;

        //<auxV>...
        if (Auxiliary(index) != NULL)
        {
            //...["not"]...
            if (sent[index].token == "not")
                index++;
            //..."have"...
            if (sent[index].token == "have")
            {
                index++;
                //...<Vpastp>
                if (sent[index].token != "been" && Verb(index) != NULL)
                {
                    int i = index;
                    index = temp;
                    vphrase = new TreeNode<string>("Verb Phr");
                    vphrase->children.push_back(Auxiliary(index)); //aux
                    if (i - temp == 4)
                        vphrase->children.push_back(Adverb(index)); //not
                    vphrase->children.push_back(Verb(index)); //have
                    vphrase->children.push_back(Verb(index)); //Verb
                    return vphrase;
                }
                //..."been" [<Vpastp> | <Ving>]
                if (sent[index].token == "been")
                {
                    int i = index;
                    index = temp;
                    vphrase = new TreeNode<string>("Verb Phr");
                    vphrase->children.push_back(Auxiliary(index)); //aux
                    if (i - temp == 3)
                        vphrase->children.push_back(Adverb(index)); //not
                    vphrase->children.push_back(Verb(index)); //have
                    vphrase->children.push_back(Verb(index)); //been
                    vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                    return vphrase;
                }
            }
            //..."be" [<Vpastp> | <Ving>]
            if (sent[index].token == "be")
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Auxiliary(index)); //aux
                if (i - temp == 2)
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //be
                vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                return vphrase;
            }
            //...<Vinf>
            if (Verb(index) != NULL)
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Auxiliary(index)); //aux
                if (i - temp == 3)
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //Verb
                return vphrase;
            }
        }
        index = temp;

        //"ought" ("to" | "not")...
        if (sent[index].token == "ought" && (sent[index+1].token == "to" || sent[index+1].token == "not"))
        {
            index = index + 2;
            //..."be" [<Vpastp> | <Ving>]
            if (sent[index].token == "be")
            {
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //ought
                if (sent[index].token == "to")
                    vphrase->children.push_back(Preposition(index)); //to
                else
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //be
                vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                return vphrase;
            }
            //..."have"...
            if (sent[index].token == "have")
            {
                index++;
                //..."been" [<Vpastp> | <Ving>]
                if (sent[index].token == "been")
                {
                    index = temp;
                    vphrase = new TreeNode<string>("Verb Phr");
                    vphrase->children.push_back(Verb(index)); //ought
                    if (sent[index].token == "to")
                        vphrase->children.push_back(Preposition(index)); //to
                    else
                        vphrase->children.push_back(Adverb(index)); //not
                    vphrase->children.push_back(Verb(index)); //have
                    vphrase->children.push_back(Verb(index)); //been
                    vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                    return vphrase;
                }
                //...<Vpastp>
                if (Verb(index) != NULL)
                {
                    index = temp;
                    vphrase = new TreeNode<string>("Verb Phr");
                    vphrase->children.push_back(Verb(index)); //ought
                    if (sent[index].token == "to")
                        vphrase->children.push_back(Preposition(index)); //to
                    else
                        vphrase->children.push_back(Adverb(index)); //not
                    vphrase->children.push_back(Verb(index)); //have
                    vphrase->children.push_back(Verb(index)); //Verb
                    return vphrase;
                }
            }
            //...<Vinf>
            if (Verb(index) != NULL)
            {
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //ought
                if (sent[index].token == "to")
                    vphrase->children.push_back(Preposition(index)); //to
                else
                    vphrase->children.push_back(Adverb(index)); //not
                vphrase->children.push_back(Verb(index)); //Verb
                return vphrase;
            }
        }
        index = temp;

        //("do" |"does" |"did")...
        if (sent[index].token == "do" || sent[index].token == "does" || sent[index].token == "did")
        {
            //...["not"] [<Vinf>]
            vphrase = new TreeNode<string>("Verb Phr");
            vphrase->children.push_back(Verb(index)); //do/does/did
            if (sent[index].token == "not")
                vphrase->children.push_back(Adverb(index)); //not
            vphrase->children.push_back(Verb(index)); //Verb (might not have this)
            return vphrase;
        }

        //("am" |"are" |"is" |"was" |"were")...
        if (sent[index].token == "am" || sent[index].token == "are" || sent[index].token == "is" || sent[index].token == "was" ||
                sent[index].token == "were")
        {
            index++;
            //...["not"]...
            if (sent[index].token == "not")
                index++;
            //..."being"
            if (index < sent.size() && sent[index].token == "being")
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //am/are/...
                if (i - temp == 2)
                    vphrase->children.push_back(Adverb(index)); //not
                sent[index].pos = VERB_PART; //"being" is orignially recognized as a noun
                vphrase->children.push_back(Verb(index)); //being
                vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                return vphrase;
            }
            //..."going" "to"
            if (index < sent.size() && sent[index].token == "going" && sent[index+1].token == "to")
            {
                int i = index;
                index = temp;
                vphrase = new TreeNode<string>("Verb Phr");
                vphrase->children.push_back(Verb(index)); //am/are/...
                if (i - temp == 2)
                    vphrase->children.push_back(Adverb(index)); //not
                sent[index].pos = VERB_PART; //"going" is orignially recognized as a noun
                vphrase->children.push_back(Verb(index)); //going
                vphrase->children.push_back(Preposition(index)); //to
                vphrase->children.push_back(Verb(index)); //Verb (might not have this)
                return vphrase;
            }
            //...[<Vpastp> | <Ving>]
            int i = index;
            index = temp;
            vphrase = new TreeNode<string>("Verb Phr");
            vphrase->children.push_back(Verb(index)); //am/are/...
            if (i - temp == 2)
                vphrase->children.push_back(Adverb(index)); //not
            vphrase->children.push_back(Verb(index)); //Verb (might not have this)
            return vphrase;
        }
        index = temp;
    }

    return vphrase;
}

TreeNode<string>* ParseTree::Verb(int& index){
    TreeNode<string>* verb = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == VERB_PART || sent[index].pos == VERB_TRANS || sent[index].pos == VERB_INTRANS)
        {
            verb = new TreeNode<string>("Verb");
            verb->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return verb;
}

TreeNode<string>* ParseTree::Complement(int& index){
    TreeNode<string>* comp = NULL;
    if (index < sent.size())
    {
        int temp = index;

        //[[<indirect object>] <object>]
        if (Object(index) != NULL)
        {
            index = temp;
            comp = new TreeNode<string>("Complem");
            if (IndirectObject(index) != NULL && Object(index) != NULL)
            {
                index = temp;
                comp->children.push_back(IndirectObject(index));
                comp->children.push_back(Object(index));
            }
            else
            {
                index = temp;
                comp->children.push_back(Object(index));
            }
            return comp;
        }

        //[<adverb>* <adjective>]
        if (sent[index].pos == ADVERB)
        {
            index++;
            while (sent[index].pos == ADVERB)
                index++;
            if (sent[index].pos == ADJECTIVE)
            {
                comp = new TreeNode<string>("Complem");
                int i = temp;
                while (i < index)
                    comp->children.push_back(Adverb(i));
                comp->children.push_back(Adjective(index));
                return comp;
            }
        }
        index = temp;

        //[<prep phr>*]
        TreeNode<string>* prep = PrepositionalPhrase(index);
        if (prep != NULL)
        {
            comp = new TreeNode<string>("Complem");
            while (prep != NULL)
            {
                comp->children.push_back(prep);
                prep = PrepositionalPhrase(index);
            }
            return comp;
        }
        index = temp;

        //["to" <Vinf> [<object>]]
        if (sent[index].token == "to" && Verb(++index) != NULL)
        {
            index = temp;
            comp = new TreeNode<string>("Complem");
            comp->children.push_back(Preposition(index));
            comp->children.push_back(Verb(index));
            int temp2 = index;
            if (Noun(index) != NULL)
            {
                index = temp2;
                comp->children.push_back(Noun(index));
            }
            else if (Pronoun(index) != NULL)
                comp->children.push_back(Pronoun(--index));
            return comp;
        }
        index = temp;
    }

    return comp;
}

TreeNode<string>* ParseTree::Conjunction(int& index){
    TreeNode<string>* conj = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == CONJUNCTION)
        {
            conj = new TreeNode<string>("Conj");
            conj->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return conj;
}

TreeNode<string>* ParseTree::Adverb(int& index){
    TreeNode<string>* adv = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == ADVERB)
        {
            adv = new TreeNode<string>("Adv");
            adv->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return adv;
}

TreeNode<string>* ParseTree::Auxiliary(int& index){
    TreeNode<string>* aux = NULL;
    if (index < sent.size())
    {
        if (sent[index].token == "must" || sent[index].token == "may" || sent[index].token == "might" ||
            sent[index].token == "will" || sent[index].token == "would" || sent[index].token == "shall" ||
            sent[index].token == "should" || sent[index].token == "can" || sent[index].token == "could")
        {
            aux = new TreeNode<string>("Aux");
            aux->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return aux;
}

TreeNode<string>* ParseTree::Preposition(int& index){
    TreeNode<string>* prep = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == PREPOSITION)
        {
            prep = new TreeNode<string>("Prep");
            prep->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return prep;
}

TreeNode<string>* ParseTree::Adjective(int& index){
    TreeNode<string>* adj = NULL;
    if (index < sent.size())
    {
        if (sent[index].pos == ADJECTIVE)
        {
            adj = new TreeNode<string>("Adj");
            adj->children.push_back(new TreeNode<string>(sent[index].token));
            index++;
        }
    }
    return adj;
}

TreeNode<string>* ParseTree::PrepositionalPhrase(int& index){
    TreeNode<string>* pphrase = NULL;
    if (index < sent.size())
    {
        int temp = index;
        //<preposition> <prep object>
        if (Preposition(index) != NULL && PrepositionalObject(index) != NULL)
        {
            index = temp;
            pphrase = new TreeNode<string>("Prep Phr");
            pphrase->children.push_back(Preposition(index));
            pphrase->children.push_back(PrepositionalObject(index));
        }
    }
    return pphrase;
}

TreeNode<string>* ParseTree::Object(int& index){
    TreeNode<string>* obj = NULL;
    if (index < sent.size())
    {
        int temp = index;
        if (SimpleObject(index) != NULL)
        {
            index = temp;
            obj = new TreeNode<string>("Obj");
            obj->children.push_back(SimpleObject(index));
            return obj;
        }
        if (CompoundObject(index) != NULL) //switch comp obj so it's tested first?
        {
            index = temp;
            obj = new TreeNode<string>("Obj");
            obj->children.push_back(CompoundObject(index));
            return obj;
        }
    }
    return obj;
}

TreeNode<string>* ParseTree::SimpleObject(int& index){
    TreeNode<string>* sobj = NULL;
    if (index < sent.size())
    {
        int temp = index;
        if (NounPhrase(index) != NULL)
        {
            index = temp;
            sobj = new TreeNode<string>("Simple Obj");
            sobj->children.push_back(NounPhrase(index));
            return sobj;
        }
        if (Pronoun(index) != NULL)
        {
            index = temp;
            sobj = new TreeNode<string>("Simple Obj");
            sobj->children.push_back(Pronoun(index));
            return sobj;
        }
    }
    return sobj;
}

TreeNode<string>* ParseTree::CompoundObject(int& index){
    TreeNode<string>* cobj = NULL;
    if (index < sent.size())
    {
        int temp = index;
        if (SimpleObject(index) != NULL && (sent[index].token == "and" || sent[index].token == "or") && SimpleObject(++index) != NULL)
        {
            index = temp;
            cobj = new TreeNode<string>("Compound Obj");
            cobj->children.push_back(NounPhrase(index));
            //cobj->children.push_back(SimpleObject(index));
            return cobj;
        }
        index = temp;
    }
    return cobj;
}

TreeNode<string>* ParseTree::IndirectObject(int &index){
    TreeNode<string>* iobj = Object(index);
    if (iobj != NULL)
        iobj->item = "Indirect Obj";
    return iobj;
}

TreeNode<string>* ParseTree::PrepositionalObject(int& index){
    TreeNode<string>* pobj = Object(index);
    if (pobj != NULL)
        pobj->item = "Prep Obj";
    return pobj;
}

void ParseTree::ParseInterrogative(){
    int index = 0;

    //("What" |"Which") <noun> <predicate>"?"
    if ((sent[index].token == "What" || sent[++index].token == "Which") && Noun(++index) != NULL && Predicate(index) != NULL)
    {
        index = 0;
        root = new TreeNode<string>("Sentence");
        root->children.push_back(Article(index)); //"what" and "which" are tagged as articles
        root->children.push_back(Noun(index));
        root->children.push_back(Predicate(index));
        return;
    }
    index = 0;

    //["What" |"When" |"Where" |"Who" |"To whom" |"Why"]
    if (sent[index].token == "What" || sent[index].token == "When" || sent[index].token == "Where" ||
        sent[index].token == "Who" || sent[index].token == "Why")
        index++;
    else if (sent[index].token == "To" && sent[index+1].token == "whom")
        index = 2;
    //("do" |"does" |"don't" |"doesn't" |"did" |"didn't")
    if (sent[index].token == "do" || sent[index].token == "does" || sent[index].token == "did")
    {
        index++;
        if (sent[index].token == "not")
            index++;
        //<subject> <predicate>"?"
        if (Subject(index) != NULL && Predicate(index) != NULL)
        {
            root = new TreeNode<string>("Sentence");
            index = 0;
            if (sent[index].token == "What")
                root->children.push_back(Article(index));
            else if (sent[index].token == "When" || sent[index].token == "Where" || sent[index].token == "Why")
                root->children.push_back(Adverb(index));
            else if (sent[index].token == "Who")
                root->children.push_back(Pronoun(index));
            else if (sent[index].token == "To" && sent[index+1].token == "whom")
            {
                root->children.push_back(Preposition(index));
                root->children.push_back(Pronoun(index));
            }
            root->children.push_back(Verb(index)); //do/does/did
            if (sent[index].token == "not")
                root->children.push_back(Adverb(index));
            root->children.push_back(Subject(index));
            root->children.push_back(Predicate(index));
            return;
        }
    }
    index = 0;

    //"Which"
    if (sent[index].token == "Which")
    {
        index++;
        //[<noun phrase>]
        NounPhrase(index);
        //("do" |"does" |"don't" |"doesn't" |"did" |"didn't")
        if (sent[index].token == "do" || sent[index].token == "does" || sent[index].token == "did")
        {
            index++;
            if (sent[index].token == "not")
                index++;
            //<subject> <predicate>"?"
            if (Subject(index) != NULL && Predicate(index) != NULL)
            {
                root = new TreeNode<string>("Sentence");
                index = 0;
                root->children.push_back(Article(index)); //which
                int temp = index;
                if (NounPhrase(index) != NULL)
                {
                    index = temp;
                    root->children.push_back(NounPhrase(index));
                }
                root->children.push_back(Verb(index)); //do/does/did
                if (sent[index].token == "not")
                    root->children.push_back(Adverb(index));
                root->children.push_back(Subject(index));
                root->children.push_back(Predicate(index));
                return;
            }
        }
    }
    index = 0;

    //["What" |"Which" |"When" |"Where" |"Who" |"To whom" |"Why"]
    if (sent[index].token == "What" || sent[index].token == "Which" || sent[index].token == "When" ||
        sent[index].token == "Where" || sent[index].token == "Who" || sent[index].token == "Why")
        index++;
    else if (sent[index].token == "To" && sent[index+1].token == "whom")
        index = 2;
    //("will" |"won't")
    if (sent[index].token == "will")
    {
        index++;
        if (sent[index].token == "not")
            index++;
        //<subject> <predicate>"?"
        if (Subject(index) != NULL && Predicate(index) != NULL)
        {
            root = new TreeNode<string>("Sentence");
            index = 0;
            if (sent[index].token == "What" || sent[index].token == "Which" || sent[index].token == "Who")
                root->children.push_back(Article(index));
            else if (sent[index].token == "When" || sent[index].token == "Where" || sent[index].token == "Why")
                root->children.push_back(Adverb(index));
            else if (sent[index].token == "To" && sent[index+1].token == "whom")
            {
                root->children.push_back(Preposition(index));
                root->children.push_back(Pronoun(index));
            }
            root->children.push_back(Verb(index)); //will
            if (sent[index].token == "not")
                root->children.push_back(Adverb(index));
            root->children.push_back(Subject(index));
            root->children.push_back(Predicate(index));
            return;
        }
    }
    index = 0;

    //"Who" <predicate>"?"
    if (sent[index].token == "Who" && Predicate(++index) != NULL)
    {
        index = 0;
        root = new TreeNode<string>("Sentence");
        root->children.push_back(Pronoun(index));
        root->children.push_back(Predicate(index));
        return;
    }
    index = 0;

    //...
}

Sent ParseTree::ReducedSentence(){
    Sent sent;
    vector<TreeNode<string>*> v;
    Nouns n;
    TreeNode<string>* find;

    //find the verb and add it to sent
    find = Find("Pred", root);
    find = Find("Verb", find);
    sent.verb = find->children[0]->item;

    //find subject
    find = Find("Subj", root);
    find = Find("Noun", find);
    if (find == NULL)
    {
        find = Find("Subj", root);
        find = Find("Pron", find);
    }
    if (find != NULL)
        n.subject = find->children[0]->item;

    //find direct object
    find = Find("Obj", root);
    find = Find("Noun", find);
    if (find == NULL)
    {
        find = Find("Obj", root);
        find = Find("Pron", find);
    }
    if (find != NULL)
        n.direct_obj = find->children[0]->item;

    //find indirect object
    find = Find("Indirect Obj", root);
    find = Find("Noun", find);
    if (find == NULL)
    {
        find = Find("Indirect Obj", root);
        find = Find("Pron", find);
    }
    if (find != NULL)
        n.indirect_obj = find->children[0]->item;

    //find prepositional object
    find = Find("Prep Obj", root);
    find = Find("Noun", find);
    if (find == NULL)
    {
        find = Find("Prep Obj", root);
        find = Find("Pron", find);
    }
    if (find != NULL)
        n.prep_obj = find->children[0]->item;

    //add n to sent's list of nouns
    sent.nouns.push_back(n);

    return sent;
}

TreeNode<string>* ParseTree::Find(string label, TreeNode<string>* node){
    if (node != NULL)
    {
        if (node->item == label)
            return node;
        else
        {
            for (int i = 0; i < node->children.size(); i++)
            {
                TreeNode<string>* find = Find(label, node->children[i]);
                if (find != NULL)
                    return find;
            }
        }
    }
    return NULL;
}
