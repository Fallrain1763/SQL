#ifndef BTREE_PLUS_ARRAY_FUNCTIONS_H
#define BTREE_PLUS_ARRAY_FUNCTIONS_H

#include <iostream>
#include <iomanip>

using namespace std;

//return the larger of the two items
template <typename T>
T maximal(const T& a, const T& b)
{
    if(a >= b)
        return a;
    
    else
        return b;
}                     

//swap the two items
template <typename T>
void swap(T& a, T& b)
{
    T c = a;

    a = b;

    b = c;
}  

//return index of the largest item in data
template <typename T>
int index_of_maximal(T data[ ], int n)
{
    T max = data[0];
    int index = 0;

    for(int i=0; i<n; i++)
    {
        if(data[i] > max)
        {
            max = data[i];
            index = i;
        }
    }

    return index;
}                      

//return the first element in data that is
//not less than entry
template <typename T>
int first_ge(const T data[ ], int n, const T& entry)
{
    for(int i=0; i<n; i++)
    {
        if(data[i] >= entry)
            return i;
    }

    //return size if entry is the largest
    return n;
}  

//insert entry at index i in data
template <typename T>
void insert_item(T data[ ], int i, int& n, T entry)
{
    int index = n;

    while(index > i)
    {
        //shift all items
        data[index] = data[index - 1];

        index--;
    }

    data[i] = entry;
    n++;
}  

//insert entry into the sorted array
//data with length n
template <typename T>
void ordered_insert(T data[ ], int& n, T entry)
{
    int index = first_ge(data, n, entry);

    insert_item(data, index, n, entry);
}  

//append entry to the right of data
template <typename T>
void attach_item(T data[ ], int& n, const T& entry)
{
    data[n] = entry;
    n++;
}

//remove the last element in data and place
//it in entry
template <typename T>
void detach_item(T data[ ], int& n, T& entry)
{
    n--;
    entry = data[n];
}          

//delete item at index i and place it in entry
template <typename T>
void delete_item(T data[ ], int i, int& n, T& entry)
{
    //record;
    entry = data[i];

    n--;

    int index = i;

    while(index < n)
    {
        data[index] = data[index + 1];

        index++;
    } 
}  

//append data2 to the right of data1
template <typename T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int size = n2;

    for(int i=0; i<size; i++)
    {
        //putt data2's first to data1's last
        data1[n1] = data2[i];

        n2--;
        n1++;
    }
}   

//move n/2 elements from the right of data1
//and move to data2
template <typename T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int size = n1 / 2;

    int data2_first = n1 - size;

    for(int i=0; i<size; i++)
    {
        //n2 start at 0
        data2[n2] = data1[data2_first];

        data2_first++;
        n2++;
        n1--;
    }
}   

//copy src[] into dest[]
template <typename T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size)
{
    for(int i=0; i<src_size; i++)
    {
        dest[i] = src[i];
    }

    dest_size = src_size;
}             

//print array data
template <typename T>
void print_array(const T data[], int n, int pos = -1)
{
    for(int i=0; i<n; i++)
    {
        cout << data[i] << " ";
    }
} 

//item > all data[i]
template <typename T>
bool is_gt(const T data[], int n, const T& item)
{
    for(int i=0; i<n; i++)
    {
        if(item <= data[i])
            return false;
    }

    return true;
}       

//item <= all data[i]
template <typename T>
bool is_le(const T data[], int n, const T& item)
{
    for(int i=0; i<n; i++)
    {
        if(item > data[i])
            return false;
    }

    return true;
}       

//return the item's index
template <typename T>
int search_item(const T data[], int n, const T& item)
{
    for(int i=0; i<n; i++)
    {
        if(data[i] == item)
            return i;
    }

    return -1;
}

#endif