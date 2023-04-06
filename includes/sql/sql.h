#ifndef SQL_H
#define SQL_H

#include <iostream>
#include <cstring>
#include "../table/table.h"
#include "../parser/parser.h"

using namespace std;

class SQL
{
    public:
        //CTOR
        SQL();

        //application
        void run();

        //for testing
        Table command(char* s);
        Table command(const string& s);
        vector<long> select_recnos(){return _sql_sr;}

    private:
        //parse the command line
        Parser _parser;

        //manage tables
        Map<string, Table> _tables;

        //command functions
        Table make();
        Table insert();
        Table select();
        Table batch();

        //rebuild _tables
        void rebuild_tables();
        void print_table_names();

        //batch No.
        int batch_num;

        //for testing
        vector<long> _sql_sr;
};

#endif