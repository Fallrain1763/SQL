#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include "typedefs.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../token/child_tokens.h"
#include "../binary_files/file_record.h"

using namespace std;

class Table
{
    public:
        //default CTOR
        Table();

        //rebuild table base on the data in the file
        Table(string table_name);

        //creat a bin file, file name is table name
        //insert field name to first line
        Table(string table_name, vectorstr field_names);

        //Big Three
        ~Table();
        Table(const Table& other);
        Table& operator =(const Table& RHS);

        //insert new info to table
        void insert_into(vectorstr fields);

        //poor version select
        Table select(vectorstr fields, string field, 
                     string op, string conditions);
        
        //select base on a post Token queue
        Table select(vectorstr fields, Queue<Token*> post);

        //convert a vector<string> to a post Token queue
        //then call other select function
        Table select(vectorstr fields, vectorstr condition);

        //for where no
        Table select(vectorstr fields);

        vector<long> select_recnos(){return _select_recnos;}

        //rebuild index base on new infos
        void rebuild_index();      

        friend ostream& operator <<( ostream& outs, const Table& t)
        {
            outs << "Table name: " << t._table_name <<", records: " 
                 << t._record_count << endl;

            fstream f;

            FileRecord r2;
            open_fileRW(f, t._file_name.c_str());

            int i = 0;
            long bytes = r2.read(f, i); 
  
            while (bytes>0)
            {
                if(i == 0)
                    outs << setw(25) << "record" << r2 << endl << endl;
                
                else
                    outs << setw(25)<< i-1 << r2 << endl;
                
                i++;
                bytes = r2.read(f, i);
            }

            return outs;
        }

    private:
        //access bin_file
        fstream _f;
        FileRecord _record;
        int _recno;

        //names
        string _table_name;
        string _file_name;
        vectorstr _field_names;

        //how many records in table
        int _record_count;

        //select serial
        int serial;

        //[yao: 2 3]
        vector<long> _select_recnos;

        //record the fields order in bin_file
        Map<string, int> _fields_map;

        //the index
        Map<string, MMap<string, long> > _index;
};

#endif