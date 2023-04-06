#include "parser.h"

Parser::Parser()
{
    set_column();
    make_table(_table);
}

Parser::Parser(char* input)
{
    set_column();
    make_table(_table);
    set_string(input);  
}

void Parser::set_string(char* input)
{
    //clean prev input
    _input.clear();

    //put the tokenized string in a queue first, dealing space
    Queue<string> temp;
   
    STokenizer stk(input);
    SToken t;
    stk>>t;
    while (stk.more())
    {
        temp.push(t.token_str());
        stk >> t;
    }

    //fix the input
    temp_to_input(_input, temp);


    //build ptree and check command line vaild or not
    _is_vaild = get_parse_tree();
}

void Parser::set_column()
{
    _columns.insert("make", MAKE);
    _columns.insert("insert", INSERT);
    _columns.insert("select", SELECT);
    _columns.insert("table", TABLE);
    _columns.insert("into", INTO);
    _columns.insert("from", FROM);
    _columns.insert("fields", FIELDS);
    _columns.insert("values", VALUES);
    _columns.insert("*", STAR);
    _columns.insert("where", WHERE);
    _columns.insert(",", COMMA);
    _columns.insert("", SYMBOL);
    _columns.insert("batch", BATCH);
}

void Parser::make_table(int _table[][30])
{
    initialize_table(_table);

    //Mark success states 5, 10, 15, 20
    _table[5][0]  = 1;
    _table[10][0] = 1;
    _table[15][0] = 1;
    _table[20][0] = 1;

    //<make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]
    _table[0][MAKE]   = 1;
    _table[1][TABLE]  = 2;
    _table[2][SYMBOL] = 3;
    _table[3][FIELDS] = 4;
    _table[4][SYMBOL] = 5;
    _table[5][COMMA]  = 4;

    //insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]
    _table[0][INSERT] = 6;
    _table[6][INTO]   = 7;
    _table[7][SYMBOL] = 8;
    _table[8][VALUES] = 9;
    _table[9][SYMBOL] = 10;
    _table[10][COMMA] = 9;

    /*
     select <* | FIELD_NAME> [, ,FIELD_NAME>...]
					from <TABLE_NAME>
					where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>
						[<LOGICAL_OPERATOR>
							<FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]
    */
    //select * from student .....
    _table[0][SELECT]  = 11;  
    _table[11][STAR]   = 12;
    _table[12][FROM]   = 13;
    _table[13][SYMBOL] = 15;

    //select <field_name[, <field_name>....]> from ......
    //_table[0][SELECT]  = 11;
    _table[11][SYMBOL] = 16;
    _table[16][COMMA]  = 17;
    _table[17][SYMBOL] = 16;
    _table[16][FROM]   = 13;
    //_table[13][SYMBOL] = 15;

    //where <symbol>[<symbol><symbol>.....]
    _table[15][WHERE]  = 14;
    _table[14][SYMBOL] = 20;
    _table[20][SYMBOL] = 20;

    //batch <file_name>
    _table[19][0] = 1;
    _table[0][BATCH]   = 18;
    _table[18][SYMBOL] = 19;


/*
0  1  6  11  -1  -1  -1  -1  -1  -1  -1  -1  -1  18  

0  -1  -1  -1  2  -1  -1  -1  -1  -1  -1  -1  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  3  -1  

0  -1  -1  -1  -1  -1  -1  4  -1  -1  -1  -1  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  5  -1  

1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  4  -1  -1  

0  -1  -1  -1  -1  7  -1  -1  -1  -1  -1  -1  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  8  -1  

0  -1  -1  -1  -1  -1  -1  -1  9  -1  -1  -1  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  10  -1  

1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  9  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  12  -1  -1  16  -1  

0  -1  -1  -1  -1  -1  13  -1  -1  -1  -1  -1  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  15  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  20  -1  

1  -1  -1  -1  -1  -1  -1  -1  -1  -1  14  -1  -1  -1  

0  -1  -1  -1  -1  -1  13  -1  -1  -1  -1  17  -1  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  16  -1  

0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  19  -1  

1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  

1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  20  -1 
*/

/*
    for (int r = 0; r < 21; r++)
    {
        for (int c = 0; c < 14; c++)
        {
            cout << _table[r][c] << "  ";
        }
      
        cout << endl << endl;
    }
*/

}

bool Parser::get_parse_tree()
{
    //clear prev _ptree
    _ptree.clear();

    //start at state 0
    int state = 0;
    int val = 0;

    //go through the input line
    for(int i=0; i<_input.size(); i++)
    {
        string str = _input[i];

        //see does the keywords contaion the string
        //if contion then the columu value  is _column[str]
        //if not then it is a SYMBOL
        if(_columns.contains( Pair<string, int>(str) ))
            val = _columns[str];
        
        else
            val = SYMBOL;

        //go to next state
        state = _table[state][val];

        //if go to other space, command line is not valid
        if(state == -1)
            return false;

        switch(state)
        {
            case 1:
                _ptree.insert("command", str);
                break;
            
            case 3:
                _ptree.insert("table_name", str);
                break;

            case 5:
                _ptree.insert("col", str);
                break;
            
            case 6:
                _ptree.insert("command", str);
                break;
            
            case 8:
                _ptree.insert("table_name", str);
                break;
            
            case 10:
                _ptree.insert("values", str);
                break;
            
            case 11:
                _ptree.insert("command", str);
                break;
            
            case 12:
                _ptree.insert("fields", str);
                break;
            
            case 14:
                _ptree.insert("where", "yes");
                break;
            
            case 15:
                _ptree.insert("table_name", str);
                break;
            
            case 16:
                _ptree.insert("fields", str);
                break;
            
            case 18:
                _ptree.insert("command", str);
                break;

            case 19:
                _ptree.insert("file_name", str+=".txt");
                break;
            
            case 20:
                _ptree.insert("condition", str);
                break;
            
            default:
                 break;
        }
    }

    //if at the end we are in a success state
    if(_table[state][0] == 1)
        return true;

    else
        return false;
}