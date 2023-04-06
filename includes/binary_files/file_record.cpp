#include "file_record.h"

FileRecord::FileRecord()
{
    for(int i=0; i<(MAX_ROW); i++)
    {
        for(int j=0; j<(MAX_COL+1); j++)
        {
            _record[i][j] = '\0';
        }
    }

    recno = -1;
}

FileRecord::FileRecord(vector<string>& v)
{
    for(int i=0; i<(MAX_ROW); i++)
    {
        for(int j=0; j<(MAX_COL+1); j++)
        {
            _record[i][j] = '\0';
        }
    }

    recno = -1;

    for(int i=0; i<v.size(); i++)
    {
        //.c_str(), use string as cstring
        strcpy(_record[i], v[i].c_str());
    }
}

long FileRecord::write(fstream &outs)
{
    //write to the end of the file
    outs.seekg(0, outs.end);

    //retrieves the current position of the file pointer
    long pos = outs.tellp();

    //outs.write(&_record[0], sizeof(_record));
    //2d ver
    outs.write(&_record[0][0], sizeof(_record));

    //record number
    return pos/sizeof(_record);
}

long FileRecord::read(fstream &ins, long recno)
{
    //returns the number of bytes read
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    //2d ver
    ins.read(&_record[0][0], sizeof(_record));

    return ins.gcount();
}
