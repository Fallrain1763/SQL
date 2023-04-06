#ifndef CONSTANTS_H
#define CONSTANTS_H
const int ALPHA   = 1;
const int PUNC    = 2;
const int SPACE   = 3;
const int NUMBER  = 4; 

const int MAX_COLUMNS = 128;
const int MAX_ROWS    = 100;
const int MAX_BUFFER  = 201;
const int MAX = 30;

const int ALPHA_STATE   = 1;
const int PUNC_STATE    = 10;
const int SPACE_STATE   = 20;
const int NUMBER_STATE  = 30;

enum Col
{
    MAKE   = 1,
    INSERT = 2,
    SELECT = 3,
    TABLE  = 4,
    INTO   = 5,
    FROM   = 6,
    FIELDS = 7,
    VALUES = 8,
    STAR   = 9,
    WHERE  = 10,
    COMMA  = 11,
    SYMBOL = 12,
    BATCH  = 13
};


#endif // CONSTANTS_H