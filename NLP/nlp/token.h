//Token is a class that holds token strings and their type.
#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>

using namespace std;

class Token
{
public:
    Token();
    Token(string s, char part_of_speech, string all_pos);
    Token(char ch, char part_of_speech, string all_pos);

    friend ostream& operator << (ostream& outs, const Token& t);

    friend bool operator != (const Token t1, const Token t2){
        return (t1.token != t2.token);
    }
    friend bool operator <= (const Token t1, const Token t2){
        return (t1.token <= t2.token);
    }
    friend bool operator < (const Token t1, const Token t2){
        return (t1.token < t2.token);
    }

    string token;
    char pos;
    string pos_set;
};

#endif // TOKEN_H
