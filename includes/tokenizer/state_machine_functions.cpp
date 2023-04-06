#include "state_machine_functions.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for (int row = 0; row < MAX_ROWS; row++)
    {
        for (int columns = 0; columns < MAX_COLUMNS; columns++)
        {
            _table[row][columns] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    if( _table[state][0] == 1)
        return true;

    else
        return false;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, 
                int state)
{
    for (int columns = from; columns <= to; columns++)
    {
        mark_cell(row, _table, columns, state);
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], string columns, 
                int state)
{
    if(columns == "ALPHA")
    {
        mark_cells(row, _table, 'a', 'z', state);
        mark_cells(row, _table, 'A', 'Z', state);
    }

    else if(columns == "PUNC")
    {
        mark_cells(row, _table, '!', '/', state);
        mark_cells(row, _table, ':', '@', state);
        mark_cells(row, _table, '[', ']', state);
        mark_cells(row, _table, '{', '}', state);
    }

    else if(columns == "SPACE")
    {
        mark_cell(row, _table, ' ', state);
        mark_cell(row, _table, '\n', state);
        mark_cell(row, _table, '\t', state);
    }

    else if(columns == "DIGITS")
    {
        mark_cells(row, _table, '0', '9', state);
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for (int r = 0; r < MAX_ROWS; r++)
    {
        for (int c = 0; c < MAX_COLUMNS; c++)
        {
            cout << _table[r][c] << " ";
        }

        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos)
{
    cout << s << endl;

    for (int i = 0; i < _pos; i++)
    {
        cout << " ";
    }

    cout << "^" << endl;
}