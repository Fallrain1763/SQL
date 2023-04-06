#include  "token.h"

SToken::SToken()
{
    _token = "";
    _type = 0;
}  

SToken::SToken(string str, int type)
{
    _token = str;
    _type = type;
}

string SToken::type_string() const
{
    //print out the string type
    if( _type == ALPHA)
        return "ALPHA";

    else if( _type == PUNC)
        return "PUNC";

    else if( _type == SPACE)
        return "SPACE";

    else if( _type == NUMBER)
        return "NUMBER";

    else
        return "UNKNOWN";
}
