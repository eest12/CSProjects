#include "stokenize.h"

STokenize::STokenize(){
    str = '\0';
    pos = 0;

    f.open("mobyposi 3.txt");
    if (f.fail())
    {
        cout << "FTokenize: File failed to open\n";
        exit(1);
    }
    //else
        //cout << "FTokenize: File opened\n";
}

STokenize::STokenize(const string s){
    str = s;
    pos = 0;

    f.open("mobyposi 3.txt");
    if (f.fail())
    {
        cout << "FTokenize: File failed to open\n";
        exit(1);
    }
    //else
        //cout << "FTokenize: File opened\n";
}

void STokenize::SetStr(const string s){
    str = s;
    pos = 0;
}

Token STokenize::NextToken(){
    //doesn't properly deal with contractions/possessive
    string token;
    while (!isalpha(str[pos])) //finds first letter of word
        pos++;
    while (isalpha(str[pos]) || str[pos] == '\'') //adds letters to token until it reaches end of word
    {
        token = token + str[pos];
        pos++;
    }

    string part_of_speech = PartOfSpeech(token);
    Token tkn(token, part_of_speech[0], part_of_speech); //tags the word as the first option from all possible POS
    return tkn;
}

bool STokenize::More(){
    for (int i = pos; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
            return true;
    }
    return false;
}

int STokenize::Pos(){
    return pos;
}

string STokenize::PartOfSpeech(string word){
    f.seekg(0, f.end);
    int last = f.tellg();
    f.seekg(0, f.beg);
    int first = f.tellg();

    string search;
    int middle = 0;
    string part_of_speech;

    //everything is kept in lowercase
    for (int i = 0; word[i] != '\0'; i++)
        word[i] = tolower(word[i]);

    while (search != word && last - first > 2)
    {
        //find the middle of the file (could be midline)
        middle = (first + last) / 2;
        f.clear();
        f.seekg(middle);
        getline(f, search, '+'); //new lines are represented by + signs

        //find the position of the beginning of next line and read it
        int count = 0;
        while (search[count] != '\0')
            count++;
        f.clear();
        f.seekg(middle + count + 1); //old position plus rest of line gives you position of new line
        getline(f, search, '+'); //search now looks like this: term@POS

        //separate the word from its POS tag
        string found_word;
        int at_pos = 0;
        for (int i = 0; search[i] != '@' && search[i] != '\0'; i++) //terms and POS are separated by @ signs
        {
            found_word = found_word + search[i];
            at_pos++;
        }
        found_word = found_word + "\0";
        for (int i = 0; found_word[i] != '\0'; i++)
            found_word[i] = tolower(found_word[i]);

        //compare it to the word we're looking for
        if (found_word == word)
        {
            //get the POS tag
            for (int i = 1; search[at_pos+i] != '\0'; i++)
                part_of_speech = part_of_speech + search[at_pos+i];
            part_of_speech = part_of_speech + "\0";
            return part_of_speech;
        }
        else if (LessThan(found_word, word))
            first = middle; // + count + 1; ?
        else if (LessThan(word, found_word))
            last = middle; // + count - 1; ?
    }

    //if word wasn't found, check for -s, -ed, or -ing suffix and search for the singular/non-conjugated version
    int last_pos = word.size() - 1;
    string str;
    if (word[last_pos] == 's')
    {
        for (int i = 0; i < last_pos; i++)
            str = str + word[i];
        return PartOfSpeech(str);
    }
    if (word[last_pos-1] == 'e' && word[last_pos] == 'd')
    {
        for (int i = 0; i < last_pos - 1; i++)
            str = str + word[i];
        return PartOfSpeech(str);
    }
    if (word[last_pos-2] == 'i' && word[last_pos-1] == 'n' && word[last_pos] == 'g')
    {
        for (int i = 0; i < last_pos - 2; i++)
            str = str + word[i];
        return PartOfSpeech(str);
    }

    cout << "word not found\n";
    return "U"; //UNKNOWN
}

bool STokenize::LessThan(string word1, string word2){
    //the dictionary used for this program lists, for example, "golden" before "gold",
    //so if word1 and word2 are the same except for some remaining letters,
    //this function marks the larger word as being less than the smaller word
    //so that PartOfSpeech() moves backward instead of forward, or vice-versa
    bool same = true;
    for (int i = 0; word1[i] != '\0' && word2[i] != '\0' && same; i++)
    {
        if (word1[i] != word2[i])
            same = false;
    }
    if (!same)
        return (word1 < word2);
    else
        return (word1.size() > word2.size());
}
