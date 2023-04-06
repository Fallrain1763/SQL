#ifndef SQL_PARSER_FUNCTIONS_H
#define SQL_PARSER_FUNCTIONS_H

#include <iostream>
#include <vector>
#include "../queue/MyQueue.h"

using namespace std;

bool is_space(string str);

void temp_to_input(vector<string>& _input, Queue<string>& temp);

void initialize_table(int _table[][30]);

#endif