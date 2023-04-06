#ifndef MULTIMAP_H
#define MULTIMAP_H

#include<iostream>
#include<iomanip>
#include<vector>
#include "bplustree.h"
#include "../stl_utils/vector_utilities.h"

using namespace std;

//Mpair struct
template <typename K, typename V>
struct MPair
{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
    }

    MPair(const K& k, const V& v)
    {
        key = k;
        value_list += v;
    }

    MPair(const K& k, const vector<V>& vlist)
    {
        key = k;
        value_list += vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs<<"["<<print_me.key<<":"<<print_me.value_list<<"]";

        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key == rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key < rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key <= rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key > rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key >= rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator != (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        if(lhs.key != rhs.key)
            return true;

        else
            return false;
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        vector<V> v;

        v += lhs.value_list;
        v += rhs.value_list;

        return MPair(lhs.key, v);
    }
};


//mmap class
template <typename K, typename V>
class MMap
{
    public:
        typedef BPlusTree<MPair<K, V> > map_base;
        
        class Iterator
        {
            public:
            friend class MMap;

            Iterator(typename map_base::Iterator it = nullptr)
            {
                _it = it;
            }

            Iterator operator ++(int unused)
            {
                Iterator hold = _it;
                _it++;

                return hold;
            }

            Iterator operator ++()
            {
                 _it++;

                return *this;
            }

            MPair<K, V> operator *()
            {
                return *_it;
            }

            friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
            {
                if(lhs._it == rhs._it)
                    return true;
                
                else
                    return false;
            }

            friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
            {
                if(lhs._it != rhs._it)
                {
                    return true;
                }
                
                else
                {
                    return false;
                }
            }

            private:
                typename map_base::Iterator _it;
        };

    MMap();

//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);

    Iterator find(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

    bool is_valid(){return mmap.is_valid();}

    void print_lookup()
    {
        Iterator it;

        for(it = begin(); it != end(); it++)
        {
            MPair<K, V> mp = *it;

            cout << setw(20) <<mp.key << " :   " << mp.value_list << endl;
        }
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me)
    {
        outs<<print_me.mmap<<endl;
        return outs;
    }

    private:
        int key_count;
        BPlusTree<MPair<K, V> > mmap;
};

//CTOR
template <typename K, typename V>
MMap<K,V>::MMap()
{
    mmap = BPlusTree<MPair<K, V> >(true);
    key_count = 0;
}


//Public Functions
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin() 
{
    return mmap.begin();
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end() 
{
    return mmap.end();
}

template <typename K, typename V>
int MMap<K,V>::size() const
{
    return key_count;
}

template <typename K, typename V>
bool MMap<K,V>::empty() const
{
    return (key_count == 0);
}

template <typename K, typename V>
const vector<V>& MMap<K, V>::operator[](const K& key) const
{
    return mmap.get(MPair<K, V> (key)).value_list;
}

template <typename K, typename V>
vector<V>& MMap<K, V>::operator[](const K& key)
{
    if(find(key) == Iterator(nullptr))
        key_count++;

    return mmap.get(MPair<K, V> (key)).value_list;
}

template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v)
{
    MPair<K, V> mp(k, v);

    if(!contains(k))
        key_count++;
    
    mmap.insert(mp);
}

template <typename K, typename V>
void MMap<K,V>::erase(const K& key)
{
    mmap.remove(MPair<K, V> (key));
    key_count--;
}

template <typename K, typename V>
void MMap<K,V>::clear()
{
    mmap.clear_tree();
    key_count = 0;
}

template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const
{
    return mmap.contains(MPair<K, V> (key));
}

template <typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key)
{
    //if key is not in the map, after call the get() in bpulstree, there will
    //be a pair insert to map, so increase key_count
    if(find(key) == Iterator(nullptr))
        key_count++;

    return mmap.get(MPair<K, V> (key)).value_list;
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key)
{
    return Iterator(mmap.find(MPair<K, V> (key)));
}

template <typename K, typename V>
int MMap<K,V>::count(const K& key)
{
    return get(key).size();
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key)
{
    return mmap.lower_bound(MPair<K, V> (key));
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key)
{
    return mmap.upper_bound(MPair<K, V> (key));
}

#endif