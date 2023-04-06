#include "table.h"

//default CTOR
Table::Table()
{
    string table_name = "default_table_name";
    vectorstr field_names;
    field_names.push_back("default_field_name");

    _table_name = table_name;
    _file_name = table_name + ".bin";
    _record_count = 0;
    serial = 0;

    open_fileW(_f, _file_name.c_str());

    _record = FileRecord(field_names);
    _record.write(_f);

    _f.close();

    for(int i=0; i<field_names.size(); i++)
    {
        _field_names.push_back(field_names[i]);

        _fields_map.insert(field_names[i], i);

        _index.insert(field_names[i], MMap<string, long>());
    }
}

//rebuild table base on the data in the file
Table::Table(string table_name)
{   
    //Table t("student")
    //table name: student
    //file name: student.bin
    //set record_count and serial to 0
    _table_name = table_name;
    _file_name = table_name + ".bin";
    _record_count = 0;
    serial = 0;

    //open the table's file
    open_fileRW(_f, _file_name.c_str());

    //read first line
    long bytes = _record.read(_f, _record_count);

    //first line is field name, get the vectstr
    for(int i=0; i<_record.get_record().size(); i++)
    {   
        //set the _field_name using the vectstr
        _field_names.push_back(_record.get_record()[i]);

        //set the _fields_map using the vectstr
        //record the order of field names
        _fields_map.insert(_record.get_record()[i], i);
    }

    //while there is a record
    while(bytes > 0)
    {
        //increase the record_count
        _record_count++;

        //read next line
        bytes = _record.read(_f, _record_count);
    }

    //first line is field name;
    _record_count--;

    //close the file
    _f.close();

    rebuild_index();
}

//creat a bin file, file name is table name
//insert field name to first line
Table::Table(string table_name, vectorstr field_names)
{
    //Table t("student")
    //table name: student
    //file name: student.bin
    //set record_count and serial to 0
    _table_name = table_name;
    _file_name = table_name + ".bin";
    _record_count = 0;
    serial = 0;

    //open the table's file
    open_fileW(_f, _file_name.c_str());

    //write the field_names to first line
    _record = FileRecord(field_names);
    _record.write(_f);

    //close the file
    _f.close();

    //use field_names set private members
    for(int i=0; i<field_names.size(); i++)
    {
        _field_names.push_back(field_names[i]);

        _fields_map.insert(field_names[i], i);

        _index.insert(field_names[i], MMap<string, long>());
    }
}

//insert new info to table
void Table::insert_into(vectorstr fields)
{
    //increase the count
    _record_count++;

    //open the file
    open_fileRW(_f, _file_name.c_str());

    //write the data to the file
    //recno - 1 because first line is field name
    _record = FileRecord(fields);
    _recno = _record.write(_f)-1;

    //close the file
    _f.close();

    //update _index
    for(int i=0; i<_field_names.size(); i++)
    {
        //go to each field, insert recno to that key's value_list
        _index[_field_names[i]].insert(fields[i], _recno);
    }
}

//DESTOR
Table::~Table()
{
    for(int i=0; i<_field_names.size(); i++)
    {
        _index[_field_names[i]].clear();
    }

    _index.clear();

    _table_name.clear();

    _file_name.clear();

    _field_names.clear();

    _record_count = 0;

    serial = 0;

    _select_recnos.clear();

    _fields_map.clear();

}

//copy CTOR
Table::Table(const Table& OTHER)
{
    _index = OTHER._index;

    _table_name = OTHER._table_name;

    _file_name = OTHER._file_name;

    _field_names = OTHER._field_names;

    _record_count = OTHER._record_count;

    serial = OTHER.serial;

    _select_recnos = OTHER._select_recnos;

    _fields_map = OTHER._fields_map;
}

Table& Table::operator =(const Table& RHS)
{
    //clear
        for(int i=0; i<_field_names.size(); i++)
    {
        _index[_field_names[i]].clear();
    }

    _index.clear();

    _table_name.clear();

    _file_name.clear();

    _field_names.clear();

    _record_count = 0;

    serial = 0;

    _select_recnos.clear();

    _fields_map.clear();

    //copy
    _index = RHS._index;

    _table_name = RHS._table_name;

    _file_name = RHS._file_name;

    _field_names = RHS._field_names;

    _record_count = RHS._record_count;

    serial = RHS.serial;

    _select_recnos = RHS._select_recnos;

    _fields_map = RHS._fields_map;


    return *this;
}

//poor version select
Table Table::select(vectorstr fields, string field, 
                     string op, string conditions)
{
    serial++;

    string name = "_select_table_" + to_string(serial);

    Table t(name, fields);

    _select_recnos.clear();

    //begin() -> !lower_bound() <
    //begin() -> !upper_bound() <=
    //lower_bound() -> !null    >=
    //upper_bound() -> !null    >
    if(op == "=")
    {
        _select_recnos = _index[field][conditions];        
    }

    else if(op == ">")
    {
        MMap<string, long>::Iterator it;

        for(it = _index[field].upper_bound(conditions);
            it != _index[field].end();
            it++ )
        {
            MPair<string, long> mp = *it;

            _select_recnos = vector_union(_select_recnos, mp.value_list);
        }
    }

     else if(op == "<")
    {
        MMap<string, long>::Iterator it;        

        for(it = _index[field].begin();
            it != _index[field].lower_bound(conditions);
            it++ )
        {
            MPair<string, long> mp = *it;

            _select_recnos = vector_union(_select_recnos, mp.value_list);

        }

    }

    else if(op == "<=")
    {
        MMap<string, long>::Iterator it;        

        for(it = _index[field].begin();
            it != _index[field].upper_bound(conditions);
            it++ )
        {
            MPair<string, long> mp = *it;

            _select_recnos = vector_union(_select_recnos, mp.value_list);
        }

    }

    else if(op == ">=")
    {
        MMap<string, long>::Iterator it;

        for(it = _index[field].lower_bound(conditions);
            it != _index[field].end();
            it++ )
        {
            MPair<string, long> mp = *it;

            _select_recnos = vector_union(_select_recnos, mp.value_list);
        }
    }

    else
    {
        assert(false && "input valid ops");
    }

    open_fileRW(_f, _file_name.c_str());
        
    for(int i=0; i<_select_recnos.size(); i++)
    {
        _record.read(_f,(_select_recnos[i]+1));

        vectorstr insert_vector;

        for(int i=0; i<fields.size(); i++)
        {   
            // get_record(): [0]fname_data [1]lname_data [2]age_data
            // _fields_map = [fname:0][lname:1][age:2]
            // fields[0] = age, fields[1] = fname, fields[2] = lname
            // _fields_map[age] = 2, _fields_map[fname] = 0, _fields_map[lname] = 1
            // get_record()[2] = age_data, get_record()[0] = fname_data, get_record()[1] = lname_data;
            // after for loop,
            // insert_vector: [0]age_data [1]fname_data [2]lname_data 
            //  ^ what we want
            insert_vector.push_back( _record.get_record()[ _fields_map[ fields[i] ] ] );
        }

        t.insert_into(insert_vector);
    }

    _f.close();

    return t;
}

//select base on a post Token queue
Table Table::select(vectorstr fields, Queue<Token*> post)
{
    //increase serial num
    serial++;

    //give select table a name
    string name = _table_name;
    name += "_select_table_";
    name += to_string(serial);

    if(fields[0] == "*")
    {
        fields.clear();

        for(int i=0; i<_field_names.size(); i++)
        {
            fields.push_back(_field_names[i]);
        }
    }

    //declare a new table
    Table t(name, fields);

    //set select_recnos empty
    _select_recnos.clear();
        
    //stack for RPN
    Stack<Token*> s;

    //until the postfix queue is empty
    while(!post.empty())
    {
        //read a token from queue

        //if the token is tokenstr
        if(post.front()->getType() == 1)
        {
            //push it to the stack
            s.push(post.pop());
        }

        ////if the token is operator
        else if(post.front()->getType() == 3)
        {
            int op_type = static_cast<Operator*>(post.front())->get_op_type();
            string op = static_cast<Operator*>(post.pop())->get_operator();
            
            //Relational
            //Take two TokenStr, lhs is field name, rhs is condition
            //find a range in _index base on lhs and rhs
            //get all value_list from that range, union them, get a vector
            //use the vector create a Resultset
            if(op_type == 1)
            {
                //pop two token from stack
                //first is condition(rhs)
                //second is field(lhs)
                string conditions = static_cast<TokenStr*>(s.pop())->get_string();
                string field = static_cast<TokenStr*>(s.pop())->get_string();

                //declare a vector for build a Resultset
                vector<long> v;

                MMap<string, long>::Iterator it;

                //begin() -> !lower_bound() <
                //begin() -> !upper_bound() <=
                //lower_bound() -> !null    >=
                //upper_bound() -> !null    >
                if(op == "=")
                {   
                    v = _index[field][conditions];
                }  

                else if(op == ">")
                {   
                    for(it = _index[field].upper_bound(conditions);
                        it != _index[field].end();
                        it++ )
                    {
                        MPair<string, long> mp = *it;

                        v = vector_union(v, mp.value_list);
                    }
                } 

                else if(op == "<")
                {        
                    for(it = _index[field].begin();
                        it != _index[field].lower_bound(conditions);
                        it++ )
                    {
                        MPair<string, long> mp = *it;

                        v = vector_union(v, mp.value_list);
                    }
                } 

                else if(op == "<=")
                {   
                    for(it = _index[field].begin();
                        it != _index[field].upper_bound(conditions);
                        it++ )
                    {
                        MPair<string, long> mp = *it;

                        v = vector_union(v, mp.value_list);
                    }
                } 

                else if(op == ">=")
                {   
                    for(it = _index[field].lower_bound(conditions);
                        it != _index[field].end();
                        it++ )
                    {
                        MPair<string, long> mp = *it;

                        v = vector_union(v, mp.value_list);
                    }
                } 

                else
                {
                    assert(false && "select(fields, post): input valid rela ops");
                }

                s.push(new Resultset(v));
            }

            //Logical
            //Take two Resultset
            //get two vector from two Resultset
            //"and": instersection, "or": union， get a vector
            //use the vector create a Resultset
            if(op_type == 2)
            {
                //pop two token from the stack
                vector<long> v1 = static_cast<Resultset*>(s.pop())->get_result();
                vector<long> v2 = static_cast<Resultset*>(s.pop())->get_result();
                vector<long> v3;
                
                if(op == "and")
                {
                   v3 = vector_instersection(v1, v2); 
                }

                else if(op == "or")
                {
                   v3 = vector_union(v1, v2);
                }

                else
                {
                    assert(false && "select(fields, post): input valid log ops");
                }

                s.push(new Resultset(v3));
            }
        }

        else
        {
            cout<<"----------------"<<post.front()->getType()<<endl;
            assert(false && "select(fields, post): there should only ops and tokenstr in post");
        }
    }

    //after the while loop there should be only one Resultset Token in the stack
    //pop it from the stack and get the _select_recnos
    _select_recnos = static_cast<Resultset*>(s.pop())->get_result();

    //open the file
    open_fileRW(_f, _file_name.c_str());
        
    //go over the select_recno vector
    for(int i=0; i<_select_recnos.size(); i++)
    {
        //find the line of data we want
        //+1 because the first line is field name
        _record.read(_f,(_select_recnos[i]+1));

        //declare a vector for fixing the input order
        vectorstr insert_vector;

        //select_table field name order may differet from ori table
        //fix the order here
        for(int i=0; i<fields.size(); i++)
        {   
            // get_record(): [0]fname_data [1]lname_data [2]age_data
            // _fields_map = [fname:0][lname:1][age:2]
            // fields[0] = age, fields[1] = fname, fields[2] = lname
            // _fields_map[age] = 2, _fields_map[fname] = 0, _fields_map[lname] = 1
            // get_record()[2] = age_data, get_record()[0] = fname_data, get_record()[1] = lname_data;
            // after for loop,
            // insert_vector: [0]age_data [1]fname_data [2]lname_data 
            //  ^ what we want
            insert_vector.push_back( _record.get_record()[ _fields_map[ fields[i] ] ] );
        }

        //insert the data which is fixed order to the select table
        t.insert_into(insert_vector);
    }

    //close file
    _f.close();

    return t;
}

//convert a vector<string> to a post Token queue
//then call other select function
Table Table::select(vectorstr fields, vectorstr condition)
{   
    // 1. vectorstr condition to infix token queue

    Queue<Token*> infix;

    for(int i=0; i<condition.size(); i++)
    {   
        //build Relational token
        if(condition[i]=="<" || condition[i]==">" || condition[i]=="<=" ||
           condition[i]==">=" || condition[i]=="=")
        {
           infix.push(new Relational(condition[i]));
        }

        //build Logical token
        else if(condition[i]=="and" || condition[i]=="or")
        {
            infix.push(new Logical(condition[i]));
        }

        //build ( Token
        else if(condition[i] == "(")
            infix.push(new Lparen());

        //build ) Token
        else if(condition[i] == ")")
            infix.push(new Rparen());

        //build Tokenstr Token
        else
            infix.push(new TokenStr(condition[i]));  
    }


    // 2. infix queue to postfix queue(Shunting-yard algorithm)

    Queue<Token*> post;         //output queue
    Stack<Token*> op_stack;     //operater stack

    //until the infix queue is empty
    while(!infix.empty())
    {
        //read the token type from infix queue
        int token_type = infix.front()->getType();

        //if the token is TokenStr, push it to output queue
        if(token_type == 1)
        {
            post.push(infix.pop());
        }

        //ops: Relational and Logical, Relational go first
        //if the token is operator
        else if(token_type == 3)
        {
            //get the operator type
            //op_prec less means higher prec
            int op_type = static_cast<Operator*>(infix.front())->get_op_type();
            int op_prec = static_cast<Operator*>(infix.front())->get_prec();

            // there is an operator at the top of the operator stack
            // the operator at the top of the operator stack is not a left parenthesis
            // the operator at the top of the operator stack has equal or greater precedence
            // loop until these condition are false
            while(
                  !op_stack.empty() &&
                  op_stack.top()->getType()!=4 && 
                  static_cast<Operator*>(op_stack.top())->get_prec()<=op_prec
                 )
            {
                //pop token from the op_stack and push to output queue
                post.push(op_stack.pop());
            }

            //push the token to op_stack
            op_stack.push(infix.pop());
        }

        //if the token is (
        else if(token_type == 4)
        {
            //push the token to op_stack
            op_stack.push(infix.pop());
        }

        //if the token is )
        else if(token_type == 5)
        {
            //pop the token
            infix.pop();

            //pop all tokens above (, push them all to output queue
            while(op_stack.top()->getType()!=4)
            {
                post.push(op_stack.pop());
            }

            //pop the (
            op_stack.pop();
        }

        else
        {
            assert(false && "select(fields, vectorstr): token type wrong");
        }
    }

    //after infix queue is empty, there may be some ops still in the stack
    //pop them all, and push them all to output queue
    while(!op_stack.empty())
    {
        post.push(op_stack.pop());
    }

/*
    Queue<Token*>::Iterator it;

    for(it=post.begin(); it != post.end(); it++)
    {
        cout<<**it<<endl;
    }
*/

    //use the prev function build a select table
    return select(fields, post);
}

Table Table::select(vectorstr fields)
{   
    //increase serial num
    serial++;

    //give select table a name
    string name = _table_name;
    name += "_select_table_";
    name += to_string(serial);

    if(fields[0] == "*")
    {
        fields.clear();

        for(int i=0; i<_field_names.size(); i++)
        {
            fields.push_back(_field_names[i]);
        }
    }

    //declare a new table
    Table t(name, fields);

    _select_recnos.clear();

    for(int i=0; i<_record_count; i++)
    {
        _select_recnos.push_back(i);
    }

    //open the file
    open_fileRW(_f, _file_name.c_str());
        
    //go over the select_recno vector
    for(int i=0; i<_select_recnos.size(); i++)
    {
        //find the line of data we want
        //+1 because the first line is field name
        _record.read(_f,(_select_recnos[i]+1));

        //declare a vector for fixing the input order
        vectorstr insert_vector;

        //select_table field name order may differet from ori table
        //fix the order here
        for(int i=0; i<fields.size(); i++)
        {   
            // get_record(): [0]fname_data [1]lname_data [2]age_data
            // _fields_map = [fname:0][lname:1][age:2]
            // fields[0] = age, fields[1] = fname, fields[2] = lname
            // _fields_map[age] = 2, _fields_map[fname] = 0, _fields_map[lname] = 1
            // get_record()[2] = age_data, get_record()[0] = fname_data, get_record()[1] = lname_data;
            // after for loop,
            // insert_vector: [0]age_data [1]fname_data [2]lname_data 
            //  ^ what we want
            insert_vector.push_back( _record.get_record()[ _fields_map[ fields[i] ] ] );
        }

        //insert the data which is fixed order to the select table
        t.insert_into(insert_vector);
    }

    //close file
    _f.close();

    return t;
}

//rebuild index base on new infos
void Table::rebuild_index()
{
    //_index key is filed name strings
    //value is empty mmap for now
    for(int i=0; i<_field_names.size(); i++)
    {
        _index.insert(_field_names[i], MMap<string, long>());
    }

    //open the file
    open_fileRW(_f, _file_name.c_str());

    //read datas, first line is field name, start from second line
    _recno = 1;
    
    long bytes = _record.read(_f, _recno);

    //go to every line of data in file
    while(bytes > 0)
    {
        //put data in _index
        for(int i=0; i<_field_names.size(); i++)
        {
            _index[_field_names[i]].insert(_record.get_record()[i], (_recno-1));
        }

        _recno++;

        bytes = _record.read(_f, _recno);
    }

    //close the file
    _f.close();
}