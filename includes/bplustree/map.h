#ifndef MAP_H
#define MAP_H

#include<iostream>
#include<iomanip>
#include "bplustree.h"
#include "../stl_utils/vector_utilities.h"

using namespace std;

//pair struct
template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V())
    {
        key = k;
        value = v;
    }

    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        outs<<"["<<print_me.key<<":"<<print_me.value<<"]";

        return outs;
    }

    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key == rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key < rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key > rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key <= rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key >= rhs.key)
            return true;

        else
            return false;
    }

    friend bool operator != (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        if(lhs.key != rhs.key)
            return true;

        else
            return false;
    }

    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        //overwriting lhs.value
        return Pair(lhs.key, rhs.value);
    }
};

 
//map class
template <typename K, typename V>
class Map
{
    public:
        typedef BPlusTree<Pair<K, V> > map_base;
    
        class Iterator
        {
            public:
            friend class Map;

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

            Pair<K, V> operator *()
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
                    return true;
                
                else
                    return false;
            }

            private:
                typename map_base::Iterator _it;
        };

    Map();
//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    const V& operator[]( const K& key ) const;
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    // I have not writtent hese yet, but we will need them:
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

    int count(const K& key);

    bool is_valid(){return map.is_valid();}

     void print_lookup()
    {
        Iterator it;

        for(it = begin(); it != end(); it++)
        {
            Pair<K, V> p = *it;

            cout << setw(20) <<p.key << " :   " << p.value << endl;
        }
    }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me)
    {
        outs<<print_me.map<<endl;
        return outs;
    }
    
    private:
        int key_count;
        BPlusTree<Pair<K, V> > map;
};

//CTOR
template <typename K, typename V>
Map<K,V>::Map(): map(false)
{
    key_count = 0;
}


//Public Functions
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin() 
{
    return map.begin();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end() 
{
    return map.end();
}

template <typename K, typename V>
int Map<K,V>::size() const
{
    return key_count;
}

template <typename K, typename V>
bool Map<K,V>::empty() const
{
    return (key_count == 0);
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key)
{
    if(find(key) == Iterator(nullptr))
        key_count++;

    return map.get(Pair<K, V> (key)).value;
}

template <typename K, typename V>
const V& Map<K,V>::operator[](const K& key) const
{
    return map.get(Pair<K, V> (key)).value;
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key)
{
    if(find(key) == Iterator(nullptr))
        key_count++;

    return map.get(Pair<K, V> (key)).value;
}
 
template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const
{
    return map.get(Pair<K, V> (key)).value;
}

template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v)
{
    Pair<K, V> p(k, v);

    //if contain, then key_count will not increase
    if(!contains(p))
        key_count++;

    map.insert(p);
}

template <typename K, typename V>
void Map<K,V>::erase(const K& key)
{
    map.remove(Pair<K, V> (key));
    key_count--;
}

template <typename K, typename V>
void Map<K,V>::clear()
{
    map.clear_tree();
    key_count = 0;
}

template <typename K, typename V>
V Map<K,V>::get(const K& key)
{
    //if key is not in the map, after call the get() in bpulstree, there will
    //be a pair insert to map, so increase key_count
    if(find(key) == Iterator(nullptr))
        key_count++;

    return map.get(Pair<K, V> (key)).value;
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key)
{
    return Iterator(map.find(Pair<K, V> (key)));
}

template < typename K, typename V >
bool Map<K,V>::contains(const Pair<K,V>& target) const
{
    return map.contains(target);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K& key)
{
    return map.lower_bound(Pair<K, V> (key));
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K& key)
{
    return map.upper_bound(Pair<K, V> (key));
}

template <typename K, typename V>
int Map<K, V>::count(const K& key)
{
    //empty value
    if(map.find(Pair<K, V> (key)) == Iterator(nullptr))
        return 0;

    else
        return 1;
}

#endif 