#include "sql.h"

SQL::SQL()
{
    batch_num = 0;

    rebuild_tables();
}

void SQL::run()
{   
    cout<<"****************************************"<<endl;
    cout<<"* Welcome to MySQL.                    *"<<endl;
    cout<<"* Enter \"manual\" to view the manual.   *"<<endl;
    cout<<"****************************************"<<endl;

    cout<<endl<<endl;

    //record this no.<?> command user use
    int command_count = 0;
    
    //used input
    string command_line;

    //loop until user input close
    do{
        cout<<"["<<command_count<<"]>";
        getline(cin, command_line);

        cout<<endl;

        //input is close tell user program is end
        if(command_line == "close")
        {   
            cout<<endl<<endl;
            cout<<"MySQL is closed."<<endl;
        }

        //output manual
        else if(command_line == "manual")
        {
            cout<<"****************************************************************************************************"<<endl;
            cout<<"* Grammar:                                                                                         *"<<endl;
            cout<<"*                                                                                                  *"<<endl;
            cout<<"* <CREATE | MAKE> : { <create | make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...] } *"<<endl;
            cout<<"* <INSERT> : { insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...] }                          *"<<endl;
            cout<<"* <SELECT> : { select <* | FIELD_NAME> [, FIELD_NAME>...]                                          *"<<endl;
			cout<<"*                                      from <TABLE_NAME>                                           *"<<endl;
			cout<<"*                                      where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>            *"<<endl;
		    cout<<"*                                     [<LOGICAL_OPERATOR>                                          *"<<endl;
	        cout<<"*                                      <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]              *"<<endl;
            cout<<"* <BATCH> : { batch <FILE_NAME> }                                                                  *"<<endl;
            cout<<"*                                                                                                  *"<<endl;
            cout<<"* <VALUE>  : A string of alpha characters,                                                         *"<<endl;
            cout<<"*            or a string of alpha characters and spaces enclosed by double quotation marks:        *"<<endl;
 			cout<<"*           \"Jean Luise\", Finch, 1923                                                              *"<<endl;
            cout<<"* <RELATIONAL OPERATOR> : [ = | > | < | >= | <= ]                                                  *"<<endl;
            cout<<"* <LOGICAL OPERATOR>    : [and | or]                                                               *"<<endl;
            cout<<"* <FILE_NAME> : without filename extension, need to be enclosed by double quotation marks          *"<<endl;
            cout<<"*                                                                                                  *"<<endl;
            cout<<"* Other commands:                                                                                  *"<<endl;
            cout<<"*                                                                                                  *"<<endl;
            cout<<"* <tables>: view which tables are being managed                                                    *"<<endl;
            cout<<"* <manual>: view the manual                                                                        *"<<endl;
            cout<<"* <close> : end the program                                                                        *"<<endl;
            cout<<"****************************************************************************************************"<<endl;
            
            cout<<endl<<endl;
            cout<<"SQL: DONE."<<endl;
        }

        else if(command_line == "tables")
        {
            print_table_names();

            cout<<endl<<endl;
            cout<<"SQL: DONE."<<endl;
        }

        //user input is a command
        else
        {
            char s[300];
            strcpy(s, command_line.c_str());
            
            //parse the command
            _parser.set_string(s);
            
            //if it is a invalid command, tell user
            if(_parser.fail())
            {
                cout<<endl<<endl;
                cout<<"SQL: invalid command. See 'manual'"<<endl;
            }
            
            //if valid command
            else
            {
                MMap<string, string> ptree = _parser.parse_tree();

                //make
                if(ptree["command"][0] == "make")
                {
                    make();

                    cout<<endl<<endl;
                    cout<<"SQL: DONE."<<endl;
                }
                
                //insert
                else if(ptree["command"][0] == "insert")
                {
                    if(!_tables.contains(ptree["table_name"][0]))
                    {
                        cout<<endl<<endl;
                        cout<<"SQL: Table does not exist."<<endl;
                    }
                    
                    else
                    {
                        insert();

                        cout<<endl<<endl;
                        cout<<"SQL: DONE."<<endl;
                    }
                }
                
                //select
                else if(ptree["command"][0] == "select")
                {
                    if(!_tables.contains(ptree["table_name"][0]))
                    {
                        cout<<endl<<endl;
                        cout<<"SQL: Table does not exist."<<endl;
                    }

                    else
                    {
                        cout<<select()<<endl;

                        cout<<endl<<endl;
                        cout<<"SQL: DONE."<<endl;
                    }
                }

                //batch
                else if(ptree["command"][0] == "batch")
                {
                    batch();

                    cout<<endl<<endl;
                    cout<<"SQL: DONE."<<endl;
                }

                else
                {
                    assert(false && "SQL run: unknown command");
                }
            }
        }

        cout<<endl;

        //increase command_count
        command_count++;

    }while(command_line != "close");
}

Table SQL::command(char* s)
{   
    //parse the string
    _parser.set_string(s);
    MMap<string, string> ptree = _parser.parse_tree();

    assert(_parser.success());

    if(ptree["command"][0] == "make")
        return make();

    else if(ptree["command"][0] == "insert")
        return insert();

    else if(ptree["command"][0] == "select")
        return select();

    else 
        return Table();
}

Table SQL::command(const string& s)
{   
    char cstr[300];

    strcpy(cstr, s.c_str());

    return command(cstr);
}

Table SQL::make()
{
    MMap<string, string> ptree = _parser.parse_tree();

    //use table_name and col in ptree create a table
    Table t(ptree["table_name"][0], ptree["col"]);

    //put the table in _tables for using later
    _tables.insert(ptree["table_name"][0], t);

    ofstream foutput;
    ifstream finput;

    finput.open ("_!tables.txt");
    foutput.open ("_!tables.txt", ios::app);

     if(finput.is_open())
     {
        string str;
        bool dup = false;

        //go trough tables file check dups
        while(getline(finput, str))
        {   
            //if there is a dup, set dup to true
            if(str == ptree["table_name"][0])
                dup = true;
        }

        //if not dup, add table name to next line
        if(!dup)
            foutput<<ptree["table_name"][0]<<endl;
     }

    finput.close();
    foutput.close();

    return t;
}

Table SQL::insert()
{
    MMap<string, string> ptree = _parser.parse_tree();

    //use table_name to take the table we want to insert_into from _tables
    //then insert values we get from parser
    _tables[ptree["table_name"][0]].insert_into(ptree["values"]);
    
    return Table();
}

Table SQL::select()
{
    MMap<string, string> ptree = _parser.parse_tree();

    Table t;

    //if there is a condition
    if(!ptree["where"].empty())
    {
        //use table_name to take the table we want to select_from from _tables
        //get fields and condition from parser
        t = _tables[ptree["table_name"][0]].select(ptree["fields"], ptree["condition"]);    
    }

    //if there is not
    else
    {
        //use table_name to take the table we want to select_from from _tables
        //get fields from parser
        t = _tables[ptree["table_name"][0]].select(ptree["fields"]);
    }

    //record the last select recnos for testing
    _sql_sr = _tables[ptree["table_name"][0]].select_recnos();

    return t;
}

Table SQL::batch()
{  
    MMap<string, string> ptree = _parser.parse_tree();

    int command_count = 0;

    //ex: "batch" + "0" = "batch0" 
    string batch_name = "batch";
    batch_name += to_string(batch_num);

    //open file
    ifstream file(ptree["file_name"][0]);

    //if file open
    if(file.is_open())
    {
        string str;
        char s[300];

        
        // 1.read table names first

        //go through batch file, get tables name
        while(getline(file, str))
        {
            if(str[0] == 'm')
            {
                strcpy(s, str.c_str());
                _parser.set_string(s);
                
                make();
            }
        }   

        //close file
        file.close();

        

        // 2. output to file

        //create "batch0.txt", if it exist, clear the data inside.
        batch_name += ".txt";
        ofstream outfile(batch_name, std::ios::out | std::ios::trunc);

        //print tables
        outfile<<"----------------- SQL Tables I Manage: -----------------"<<endl;

                Map<string, Table>::Iterator it;

                for(it = _tables.begin(); it !=_tables.end(); it++)
                {
                    Pair<string, Table> p = *it;

                    outfile<<p.key<<endl;
                }

        outfile<<"--------------------------------------------------------"<<endl;

        outfile<<endl<<endl;
        outfile<<"batch"<<batch_num<<endl;

        //increase batch_num after output it
        batch_num++; 
        
        //open again
        file.open(ptree["file_name"][0]);

        //go throug batch file
        while(getline(file, str))
        {   
            //if // and space line, just cout
            if(str == "" || str[0] =='/')
            {   
                outfile<<str<<endl;
            }

            //other line
            else
            {   
                //output command line
                outfile<<"["<<command_count<<"] "<<str<<endl;
                command_count++;

                strcpy(s, str.c_str());
                _parser.set_string(s);
                
                //insert
                if(str[0] == 'i')
                {
                    insert();
                }

                //select
                else if(str[0] == 's')
                {
                    outfile<<endl;
                    outfile<<select()<<endl;
                }

                //do nothing because made table when last time open file
                else if(str[0] == 'm')
                {
                    //do nothing
                }

                //there is a invalid command in batch file
                else
                {   
                    outfile<<endl;
                    outfile<<"batch file invalid command."<<endl;
                }

                outfile<<endl<<endl<<endl;
                outfile<<"SQL: DONE."<<endl;
                outfile<<endl;
            }   
        }

        //file
        outfile<<"----------------- END OF BATCH PROCESS -----------------"<<endl;

        file.close();
        outfile.close();


        //3. output whole file to screen
        ifstream f(batch_name);

        if(f.is_open())
        {
            string getcontent;

            while(getline(f, getcontent))
            {
                cout<<getcontent<<endl;
            }
        }

        f.close();
    }

    //if file cannot be opened
    else
    {
        cout<<endl<<endl;
        cout<<"SQL: file does not exist."<<endl;
    }

    return Table();
}

void SQL::rebuild_tables()
{
    ifstream infile("_!tables.txt");

    //not exist
    if(infile.fail())
    {
        ofstream outfile("_!tables.txt");

        outfile.close();
    }

    else
    {
        string str;

        while(getline(infile, str))
        {
            _tables.insert(str, Table(str));
        }

        infile.close();
    }
}

void SQL::print_table_names()
{
    cout<<"----------------- SQL Tables I Manage: -----------------"<<endl;

                Map<string, Table>::Iterator it;

                for(it = _tables.begin(); it !=_tables.end(); it++)
                {
                    Pair<string, Table> p = *it;

                    cout<<p.key<<endl;
                }

    cout<<"--------------------------------------------------------"<<endl;
}