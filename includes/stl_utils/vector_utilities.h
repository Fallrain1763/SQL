#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H

#include<iostream>
#include<iomanip>
#include <algorithm>

using namespace std;

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
        for(int i=0; i<list.size(); i++)
    {
        outs << list[i] << " "; 
    }

    return outs;
}

template <typename T, typename U>
vector<T>& operator += (vector<T>& list, const U& addme)
{
    list.push_back(addme);

    return list;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const vector<T>& append_me)
{
    for(int i=0; i<append_me.size(); i++)  
    {
        list.push_back(append_me[i]);
    }
    return list;
}

template <typename T>
vector<T> vector_instersection(vector<T> &v1, vector<T> &v2)
{
    vector<T> v3;

    for(auto i = v1.begin(); i != v1.end(); i++)
    {
        if(find(v2.begin(), v2.end(), *i) != v2.end())
            v3.push_back(*i);
    }

    return v3;
}

template <typename T>
vector<T> vector_union(vector<T> &v1, vector<T> &v2)
{
    vector<T> v3;

    v3 += v1;

    v3 += v2;

    auto end = v3.end();

    for(auto i = v3.begin(); i != end; i++)
    {
        end = remove(i+1, end, *i);
    }

    v3.erase(end, v3.end());

    return v3;
}

#endif