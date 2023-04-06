# Work Report

## Name: <ins> Liu, Shuchen </ins>

## Features:

- Not Implemented:
  - No features have been not implemented

<br><br>

- Implemented:
<pre>
****************************************************************************************************
* Grammar:                                                                                         *
*                                                                                                  *
* CREATE | MAKE : { create | make table TABLE_NAME fields FIELD_NAME [, FIELD_NAME...] }           *
* INSERT : { insert INTO TABLE_NAME values VALUE [, VALUE...] }                                    *
* SELECT : { select * | FIELD_NAME [, FIELD_NAME...]                                               *
*                                      from TABLE_NAME                                             *
*                                      where FIELD_NAME RELATIONAL_OPERATOR VALUE                  *
*                                     [LOGICAL_OPERATOR                                            *
*                                      FIELD_NAME RELATIONAL_OPERATOR VALUE...]                    *
* BATCH : { batch FILE_NAME }                                                                      *
*                                                                                                  *
* VALUE  : A string of alpha characters,                                                           *
*            or a string of alpha characters and spaces enclosed by double quotation marks:        *
*           "Jean Luise", Finch, 1923                                                              *
* RELATIONAL OPERATOR : [ = | > | < | >= | <= ]                                                    *
* LOGICAL OPERATOR    : [and | or]                                                                 *
* FILE_NAME : without filename extension, need to be enclosed by double quotation marks            *
*                                                                                                  *
* Other commands:                                                                                  *
*                                                                                                  *
* close : end the program                                                                          *
* manual: view the manual                                                                          *
****************************************************************************************************
<pre>

<br><br>

- Partly implemented:
  - batch file read .txt only

<br><br>

- Bugs
  - No known bugs until now

<br><br>

# Reflections:

- "and" has higher prec that "or"

# **output**

<br/><br/>

# basic_test.cpp output:

<pre>

shuchenliu@shuchendeiMac build % ./bin/basic_test


----------running basic_test.cpp---------


[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from SQL_BASIC
[ RUN      ] SQL_BASIC.SQLBasic
>make table employee fields  last,       first,         dep,      salary, year
basic_test: table created.

>make table employee fields  last,       first,         dep,      salary, year
>insert into employee values Blow,       Joe,           CS,       100000, 2018
>insert into employee values Blow,       JoAnn,         Physics,  200000, 2016
>insert into employee values Johnson,    Jack,          HR,       150000, 2014
>insert into employee values Johnson,    "Jimmy",     Chemistry,140000, 2018
>make table student fields  fname,          lname,    major,    age
>insert into student values Flo,            Yao,        Art,    20
>insert into student values Bo,                      Yang,      CS,             28
>insert into student values "Sammuel L.", Jackson,      CS,             40
>insert into student values "Billy",         Jackson,   Math,   27
>insert into student values "Mary Ann",   Davis,        Math,   30



>select * from employee
basic_test: records selected: 0 1 2 3 

>select last, first, age from employee
basic_test: records selected: 0 1 2 3 

>select last from employee
basic_test: records selected: 0 1 2 3 

>select * from employee where last = Johnson
basic_test: records selected: 2 3 

>select * from employee where last=Blow and major="JoAnn"
basic_test: records selected: 

>select * from student
basic_test: records selected: 0 1 2 3 4 

>select * from student where (major=CS or major=Art)
basic_test: records selected: 0 1 2 

>select * from student where lname>J
basic_test: records selected: 2 3 1 0 

>select * from student where lname>J and (major=CS or major=Art)
basic_test: records selected: 0 1 2 
----- END TEST --------
[       OK ] SQL_BASIC.SQLBasic (76 ms)
[----------] 1 test from SQL_BASIC (76 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (76 ms total)
[  PASSED  ] 1 test.
<pre>
