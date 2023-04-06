#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include "typedefs.h"
#include "sql_parser_functions.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../tokenizer/stokenize.h"


using namespace std;

class Parser
{
    public:
        //default CTOR
        Parser();

        //Take a input command line
        Parser(char* input);

        //return ptree
        MMap<string, string> parse_tree(){return _ptree;}

        //success code
        bool success(){return _is_vaild;}

        //success code
        bool fail(){return !_is_vaild;}

        //convert input line to vector<string>
        void set_string(char* input);
        
    private:
        //result
        MMap<string, string> _ptree;

        //input command
        vector<string> _input;

        //command line is vaild or not
        bool _is_vaild;

        //keywords
        Map<string, int> _columns;

        //set keywords
        void set_column();

        //build state machine
        int _table[30][30];
        void make_table(int _table[][30]);

        //set ptree, return is_vaild or not
        bool get_parse_tree();
};

#endif