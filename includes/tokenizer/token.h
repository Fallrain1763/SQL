#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include "constants.h"

using namespace std;

class SToken
{
public:
    SToken();
    SToken(string str, int type);
    
    int type() const { return _type; }
    string token_str() const { return _token;}

    string type_string() const ;

    friend ostream& operator <<(ostream& outs, const SToken& t){
        outs << "|" << t._token <<"|" <<endl;
        return outs;
    }
private:
    string _token;
    int _type;
};

#endif // TOKEN_H
