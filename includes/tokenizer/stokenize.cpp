#include "stokenize.h"
#include <cstring>

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
    strcpy(_buffer, "");
    _pos = 0;
    make_table(_table);
}

STokenizer::STokenizer(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
    make_table(_table);
}

bool STokenizer::done()
{
    if(_pos > strlen(_buffer))
        return true;
    
    else
        return false;
}

bool STokenizer::more()
{
    return !(done()); 
}

void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);

    //ALPHA
    mark_fail(_table, ALPHA_STATE);
    mark_success(_table,ALPHA_STATE + 1);

    mark_cells(ALPHA_STATE, _table, "ALPHA", ALPHA_STATE + 1);
    mark_cells(ALPHA_STATE + 1, _table, "ALPHA", ALPHA_STATE + 1);

    //PUNC
    mark_fail(_table, PUNC_STATE);
    mark_success(_table, PUNC_STATE + 1);

    mark_cells(PUNC_STATE, _table, "PUNC", PUNC_STATE + 1);

    //SPACE
    mark_fail(_table, SPACE_STATE);
    mark_success(_table, SPACE_STATE + 1);
    mark_success(_table, ALPHA_STATE + 1);

    mark_cells(SPACE_STATE, _table, "SPACE", SPACE_STATE + 1);
    mark_cells(SPACE_STATE + 1, _table, "SPACE", SPACE_STATE + 1);

    //NUMBER
    mark_fail(_table, NUMBER_STATE);           
    mark_success(_table, NUMBER_STATE + 1);         
    mark_fail(_table, NUMBER_STATE + 2);
    mark_success(_table, NUMBER_STATE + 3);

    mark_cells(NUMBER_STATE, _table, "DIGITS", NUMBER_STATE + 1);  
    mark_cells(NUMBER_STATE, _table, '.', '.', NUMBER_STATE+2);  
    mark_cells(NUMBER_STATE + 1, _table, "DIGITS", NUMBER_STATE + 1);  
    mark_cells(NUMBER_STATE + 1, _table, '.', '.', NUMBER_STATE + 2);   
    mark_cells(NUMBER_STATE + 2, _table, "DIGITS", NUMBER_STATE + 3);
    mark_cells(NUMBER_STATE + 3, _table, "DIGITS", NUMBER_STATE + 3); 
}

bool STokenizer::get_token(int start_state, string& token)
{
    if(done())
    {
        token = "";

        return false;
    }
    
    int current_state = start_state;  
    int next_state = 0;               
    int last_state = 0;             

    //stop loop when arrive at fail state or the end of input
    while (_table[current_state][_buffer[_pos]] != -1 && 
           _buffer[_pos] != '\0' &&
           (_buffer[_pos] + 0) >= 0)
    {
        //next state is current_state row's "the char in current pos" col
        next_state = _table[current_state][_buffer[_pos]];
        token = token + _buffer[_pos];

        //keep the last state before current_state go to next state
        last_state = current_state;

        //current_state go to next state
        current_state = next_state;  
        _pos++;                      
    }

    //if next_state is success state, return true
    if (is_success(_table, next_state))
        return true;


    //if last_state is success state
    if (is_success(_table, last_state))
    {
        //pop one char, and move pos back 1;
        token.pop_back();
        _pos --;

        return true;
    }
    
    return false;
    
}