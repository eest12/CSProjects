//Erika Estrada - CS 8
//Natural Language Processing
#include <iostream>
#include "stokenize.h"
#include <vector>
#include "token.h"
#include "parsetree.h"
#include "knowledge.h"

using namespace std;

int main()
{
    cout << endl;

    STokenize stk;
    string input;
    Knowledge k;

    cout << "Enter a complete sentence on each line.\n";
    cout << "Ask questions about those sentences.\n";
    cout << "Enter 'x' to exit.\n\n";

    cout << ": ";
    getline(cin, input);
    stk.SetStr(input);

    while (input != "x" && input != "X")
    {
        vector<Token> sentence;
//        cout << endl;
        while (stk.More())
        {
            Token next = stk.NextToken();
//            cout << next;
            sentence.push_back(next);
        }
//        cout << endl << endl;

        ParseTree pt(sentence);
        if (input[input.size()-1] == '?')
        {
            pt.ParseInterrogative();
            cout << ":: " << k.ProcessQuestion(pt.ReducedSentence()) << endl;
        }
        else
        {
            pt.ParseDeclarative();
            k.AddFact(pt.ReducedSentence());
        }

        pt.Print();
//        cout << "main elements:\n";
//        cout << pt.ReducedSentence() << endl << endl;
//        cout << "stored:\n";
//        k.Print();
//        cout << endl;

        cout << ": ";
        getline(cin, input);
        stk.SetStr(input);
    }

    cout << endl;

    return 0;
}

/*
 * Testing without print functions to demonstrate question-answering:
 *
Enter a complete sentence on each line.
Ask questions about those sentences.
Enter 'x' to exit.

: Mary gave John the ball.
: Who gave John the ball?
:: Mary
: Who did Mary gave the ball?
:: John
: What did Mary gave John?
:: unknown
: Tom killed Bob.
: Who did Tom killed?
:: Bob
: Who killed Bob?
:: Tom
: Who died?
:: Bob
: I read a book at home.
: Who read a book at home?
:: I
: What did I read at home?
:: book
: Where did I read a book?
:: home
: x

Press <RETURN> to close this window...
*/



/*
 * Testing with print functions to show data structures:
 *
Enter a complete sentence on each line.
Ask questions about those sentences.
Enter 'x' to exit.

: Mary gave John the ball.

{Mary N} {gave V} {John N} {the D} {ball N}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Noun Phr]
    :  4 [Noun]
     :  5 "Mary"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "gave"
   :  3 [Complem]
    :  4 [Indirect Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Noun]
        :  8 "John"
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Artic]
        :  8 "the"
       :  7 [Noun]
        :  8 "ball"

main elements:
gave -> [s:Mary] [do:ball] [io:John] [po:]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] ||
has -> [s:John] [do:ball] [io:] [po:] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||

: He and John play music.

{He o} {and C} {John N} {play V} {music N}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Compund Subj]
    :  4 [Noun Phr]
     :  5 [Pron]
      :  6 "He"
    :  4 [Conj]
     :  5 "and"
    :  4 [Noun Phr]
     :  5 [Noun]
      :  6 "John"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "play"
   :  3 [Complem]
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Noun]
        :  8 "music"

main elements:
play -> [s:John] [do:music] [io:] [po:]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] ||
has -> [s:John] [do:ball] [io:] [po:] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||

: They have not spoken to me.

{They o} {have V} {not v} {spoken V} {to P} {me r}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Noun Phr]
    :  4 [Pron]
     :  5 "They"
  :  2 [Pred]
   :  3 [Verb Phr]
    :  4 [Verb]
     :  5 "have"
    :  4 [Adv]
     :  5 "not"
    :  4 [Verb]
     :  5 "spoken"
   :  3 [Complem]
    :  4 [Prep Phr]
     :  5 [Prep]
      :  6 "to"
     :  5 [Prep Obj]
      :  6 [Simple Obj]
       :  7 [Noun Phr]
        :  8 [Pron]
         :  9 "me"

main elements:
have -> [s:They] [do:] [io:] [po:me]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||

: Mary and Samantha took the bus.

{Mary N} {and C} {Samantha N} {took V} {the D} {bus N}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Compund Subj]
    :  4 [Noun Phr]
     :  5 [Noun]
      :  6 "Mary"
    :  4 [Conj]
     :  5 "and"
    :  4 [Noun Phr]
     :  5 [Noun]
      :  6 "Samantha"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "took"
   :  3 [Complem]
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Artic]
        :  8 "the"
       :  7 [Noun]
        :  8 "bus"

main elements:
took -> [s:Mary] [do:bus] [io:] [po:]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||
took -> [s:Mary] [do:bus] [io:] [po:] ||

: I eat breakfast at eight in the morning.

{I o} {eat V} {breakfast N} {at P} {eight N} {in P} {the D} {morning N}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Noun Phr]
    :  4 [Pron]
     :  5 "I"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "eat"
   :  3 [Complem]
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Noun]
        :  8 "breakfast"
        :  8 [Prep Phr]
         :  9 [Prep]
          : 10 "at"
         :  9 [Prep Obj]
          : 10 [Simple Obj]
           : 11 [Noun Phr]
            : 12 [Noun]
             : 13 "eight"
             : 13 [Prep Phr]
              : 14 [Prep]
               : 15 "in"
              : 14 [Prep Obj]
               : 15 [Simple Obj]
                : 16 [Noun Phr]
                 : 17 [Artic]
                  : 18 "the"
                 : 17 [Noun]
                  : 18 "morning"

main elements:
eat -> [s:I] [do:breakfast] [io:] [po:eight]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||
took -> [s:Mary] [do:bus] [io:] [po:] ||
eat -> [s:I] [do:breakfast] [io:] [po:eight] ||

: Eddy has been playing basketball every Friday for the game.

{Eddy N} {has V} {been V} {playing V} {basketball N} {every D} {Friday N} {for P} {the D} {game N}

 :  1 [Sent]
  :  2 [Subj]
   :  3 [Noun Phr]
    :  4 [Noun]
     :  5 "Eddy"
  :  2 [Pred]
   :  3 [Verb Phr]
    :  4 [Verb]
     :  5 "has"
    :  4 [Verb]
     :  5 "been"
    :  4 [Verb]
     :  5 "playing"
   :  3 [Complem]
    :  4 [Indirect Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Noun]
        :  8 "basketball"
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Artic]
        :  8 "every"
       :  7 [Noun]
        :  8 "Friday"
        :  8 [Prep Phr]
         :  9 [Prep]
          : 10 "for"
         :  9 [Prep Obj]
          : 10 [Simple Obj]
           : 11 [Noun Phr]
            : 12 [Artic]
             : 13 "the"
            : 12 [Noun]
             : 13 "game"

main elements:
has -> [s:Eddy] [do:Friday] [io:basketball] [po:game]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] || [s:Eddy] [do:Friday] [io:basketball] [po:game] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||
took -> [s:Mary] [do:bus] [io:] [po:] ||
eat -> [s:I] [do:breakfast] [io:] [po:eight] ||

: Who has the ball?

{Who r} {has V} {the D} {ball N}

:: John
 :  1 [Sentence]
  :  2 [Pron]
   :  3 "Who"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "has"
   :  3 [Complem]
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Artic]
        :  8 "the"
       :  7 [Noun]
        :  8 "ball"

main elements:
has -> [s:] [do:ball] [io:] [po:]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] || [s:Eddy] [do:Friday] [io:basketball] [po:game] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||
took -> [s:Mary] [do:bus] [io:] [po:] ||
eat -> [s:I] [do:breakfast] [io:] [po:eight] ||

: Who took the bus?

{Who r} {took V} {the D} {bus N}

:: Mary
 :  1 [Sentence]
  :  2 [Pron]
   :  3 "Who"
  :  2 [Pred]
   :  3 [Verb]
    :  4 "took"
   :  3 [Complem]
    :  4 [Obj]
     :  5 [Simple Obj]
      :  6 [Noun Phr]
       :  7 [Artic]
        :  8 "the"
       :  7 [Noun]
        :  8 "bus"

main elements:
took -> [s:] [do:bus] [io:] [po:]

stored:
gave -> [s:Mary] [do:ball] [io:John] [po:] ||
have -> [s:John] [do:ball] [io:] [po:] || [s:They] [do:] [io:] [po:me] ||
has -> [s:John] [do:ball] [io:] [po:] || [s:Eddy] [do:Friday] [io:basketball] [po:game] ||
had -> [s:Mary] [do:ball] [io:] [po:] ||
play -> [s:John] [do:music] [io:] [po:] ||
took -> [s:Mary] [do:bus] [io:] [po:] ||
eat -> [s:I] [do:breakfast] [io:] [po:eight] ||

: x

Press <RETURN> to close this window...
*/
