#include "token.h"

Token::Token(){
    pos = 'U'; //UNKNOWN
    pos_set = "U";
}

Token::Token(string s, char part_of_speech, string all_pos){
    token = s;
    pos = part_of_speech;
    pos_set = all_pos;
}

Token::Token(char ch, char part_of_speech, string all_pos){
    token = ch;
    pos = part_of_speech;
    pos_set = all_pos;
}

ostream& operator << (ostream& outs, const Token& t){
    //cout << "{" << t.token << " " << t.pos << ":" << t.pos_set << "} ";
    cout << "{" << t.token << " " << t.pos << "} ";
    return outs;
}
