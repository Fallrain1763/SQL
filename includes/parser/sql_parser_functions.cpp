#include "sql_parser_functions.h"

bool is_space(string str)
{
    for(int i=0; i<str.size(); i++)
    {
        if(str[i] == ' ' || str[i] == '\t')
            return true;
    }

    return false;
}    

//remove space, deal with >=, <=, and "
void temp_to_input(vector<string>& _input, Queue<string>& temp)
{
    while(!temp.empty())
    {
        //remove space
        if(is_space(temp.front()))
            temp.pop();

        //if is > or <
        else if(temp.front() == ">" || temp.front() == "<")
        {
            string str = temp.pop();

            //check next string is = or not, if is  build <=, >=
            if(!temp.empty() && temp.front() == "=")
            {
                str += temp.pop();
            }

            _input.push_back(str);
        }
        
        //if there is a "
        else if(temp.front() == "\"")
        {
            string str;

            //pop left "
            temp.pop();

            //build a str which include all word between ""
            while(temp.front() != "\"" && !temp.empty())
            {
                str += temp.pop();
            }

            //pop right "
            if(!temp.empty())
                temp.pop();

            _input.push_back(str);
        }

        else
            _input.push_back(temp.pop());

    }
}

void initialize_table(int _table[][30])
{
    for(int row=0; row<30; row++)
    {
        for(int columns=0; columns<30; columns++)
        {
            _table[row][columns] = -1;
        }
    }

    for(int i=0; i<21; i++)
    {
        _table[i][0] = 0;
    }
}