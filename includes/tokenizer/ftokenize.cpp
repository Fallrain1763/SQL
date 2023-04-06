#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname);
    _pos = 0;            
    _blockPos = 0; 
    _more = false;       
}

SToken FTokenizer::next_token()
{
    SToken next_token;

    _stk >> next_token;

    if(!_stk.more())
        _more = get_new_block();
    
    return next_token;
}

bool FTokenizer::more()
{
    if(_stk.more())
        _more = true;
    
    else
        _more = false;


    return _more;
}

bool FTokenizer::get_new_block()
{
      // declare and create a cstring
    char* new_block = new char[MAX_BLOCK];

    // read max_block bytes char from the file
    _f.read(new_block, MAX_BLOCK - 1); 

    // set end of cstring to null char or it cause infnite loop
    new_block[_f.gcount()] = '\0';    

    //if count of characters read is greater than 0
    if(_f.gcount())
    {
        //put cstring in the string tokenizer
        _stk.set_string(new_block); 
        
        // put the pos to the end of new block
        _pos += _f.gcount();    
        
    }

    // delete from meomery
    delete[] new_block;
    
    cout << "----- New Block ---------------------[199] bytes" <<endl;

    return false;
}