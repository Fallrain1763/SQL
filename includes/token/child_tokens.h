#ifndef CHILD_TOKENS_H
#define CHILD_TOKENS_H

#include <iostream>
#include "../stl_utils/vector_utilities.h"

using namespace std;

class Token
{
    public:
        Token(int type)
        {
            _type = type;
        }

        int getType(){return _type;}

        //for debug
        virtual void print(){}

        friend ostream& operator<< (ostream& outs, Token& printMe)
        {
            printMe.print();

            return outs;
        }

    private:
        int _type; // Type 1 is TokenStr, Type 2 is Resultset, Type 3 is operator，
                   // Type 4 is (, Type 5 is )

};


// "lname", "Yao", "age", "20"
class TokenStr : public Token
{
    public:
        TokenStr(string str) : Token(1)
        {
            _str = str;
        }

        string get_string()
        {
            return _str;
        }

        void print()
        {
            cout << _str << endl;
        }

    private:
        string _str;
};


//take a vector long
//a TokenStr and a TokenStr after Relational op is a Resultset
//a Resultset and a Resultset after Logical op is a Resultset
class Resultset : public Token
{
    public:
        Resultset(vector<long> result) : Token(2)
        {
            _result = result;
        }

        vector<long> get_result()
        {
            return _result;
        }

        void print()
        {
            cout << _result << endl;
        }

    private:
        vector<long> _result;
};


//two type:
//Releational: <, >, <=, =>, ==
//Logical: and, or
class Operator : public Token
{
    public:
        Operator(string op, int op_type) : Token(3)
        {
            _op = op;
            _op_type = op_type;

            if(op == "and")
                _prec = 2;

            else if(op == "or")
                _prec = 3;

            else
                _prec = 1;
        }

        int get_op_type()
        {
            return _op_type;
        }

        int get_prec()
        {
            return _prec;
        }

        string get_operator()
        {
            return _op;
        }

        void print()
        {
            cout<<_op<<endl;
        }

    private:
        string _op;         
        int _op_type;       // Releational is 1, logical is 2
        int _prec;          
};


//Take two TokenStr, lhs is field name, rhs is condition
//find a range in _index base on lhs and rhs
//get all value_list from that range, union them, get a vector
//use the vector create a Resultset
class Relational : public Operator
{
    public:
        Relational(string op) : Operator(op, 1)
        {
            //CTOR
        }
};


//Take two Resultset
//get two vector from two Resultset
//"and": instersection, "or": union， get a vector
//use the vector create a Resultset
class Logical : public Operator
{
    public:
        Logical(string op) : Operator(op, 2)
        {
            //CTOR
        }
};


//use in shunting-yard algorithm
class Lparen : public Token
{
    public:
        Lparen() : Token(4)
        {
            //CTOR
        }

        void print()
        {
            cout<<"("<<endl;
        }
};


//use in shunting-yard algorithm
class Rparen : public Token
{
    public:
        Rparen() : Token(5)
        {
            //CTOR
        }

        void print()
        {
            cout<<")"<<endl;
        }
};

#endif