#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include "utilities.h"

using namespace std;

struct FileRecord
{
        //when you construct a Record, it's either empty or it
        //  contains a word
        FileRecord();
        
        FileRecord(vector<string>& v);

        long write(fstream& outs);          //returns the record number
        
        long read(fstream& ins, long recno);   
                                            //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file

        vector<string> get_record()
        {
            vector<string> v;

            int i = 0;

            while(_record[i][0])
            {
                v.push_back(string(_record[i]));

                i++;
            }

            return v;
        }

        friend ostream& operator<<(ostream& outs, const FileRecord& r)
        {
            int i = 0;

            while(r._record[i][0])
            {
                outs << setw(30) << r._record[i];

                i++;
            }
            
            return outs;
        }
    
        static const int MAX_ROW = 50;
        static const int MAX_COL = 100;
        int recno;
        char _record[MAX_ROW][MAX_COL+1];
};

#endif