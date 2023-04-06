#ifndef STOKENIZE_H
#define STOKENIZE_H
#include "token.h"
#include "state_machine_functions.h"

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, SToken& t)
    {
        //declare a string
        string token_string;

        //build an token with right string type
        if(s.get_token(ALPHA_STATE, token_string))
            t = SToken(token_string, ALPHA);

        else if(s.get_token(PUNC_STATE, token_string))
            t = SToken(token_string, PUNC);

        else if(s.get_token(SPACE_STATE, token_string))
            t = SToken(token_string, SPACE);
        
        else if(s.get_token(NUMBER_STATE, token_string))
            t = SToken(token_string, NUMBER);
        
        else
        { 
                token_string.push_back(s._buffer[s._pos]);

                t = SToken(token_string, 0);

                s._pos++;
        }

        const bool debug = false;
        
        if(debug)
            cout << "Pos: "<< s._pos <<endl;

        return s;
    }

    //set a new string as the input string
    void set_string(char str[]);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif // STOKENIZE_H