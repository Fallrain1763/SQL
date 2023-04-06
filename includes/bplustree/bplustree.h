#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "btree_array_funcs.h"
#include <stack>

template <typename T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *()
        {
            assert(it && "b+tree: Iterator *(): !it");

            if(it->size() == 0)
                return T();

            assert(key_ptr<it->data_count && "b+tree: Iterator *() key_ptr>=it->data_count");

            return it->data[key_ptr];
        }

        Iterator operator++(int un_used)
        {
            Iterator hold;
            hold = it;
            key_ptr++;

            if(key_ptr >= it->data_count)
            {
                it = it->next;
                key_ptr = 0;
            }

            return hold;
        }

        Iterator operator++()
        {
            key_ptr++;

            if(key_ptr >= it->data_count)
            {
                it = it->next;
                key_ptr = 0;
            }

            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if((lhs.it == rhs.it) && (lhs.key_ptr == rhs.key_ptr))
                return true;

            else
                return false;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            if((lhs.it != rhs.it)||(lhs.key_ptr != rhs.key_ptr))
                return true;

            else
                return false;

        }

        void print_Iterator()
        {
            cout << *it << endl;
        }
        
        bool is_null()
        {
            return !it;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, stack<BPlusTree<T>*>& s);
    bool contains(const T& entry) const;        //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry)const;          //return a reference to entry in the tree
    T& get_existing(const T& entry);            //return a reference to entry in the tree
    const T& get_existing(const T& entry)const;

    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.


    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();
    string in_order();
    Iterator begin();
    Iterator end();

    void test()
    {
        BPlusTree<T>* temp = get_smallest_node();

        while(temp)
        {
            cout<<"........................................"<<endl;
            cout << "===============" <<temp->data[0]<<endl;
            cout << "===============" <<temp->data_count<<endl;
            for(int i=0; i<3; i++)
            {
                if(temp->subset[i] == nullptr)
                    cout<<i<<": is null"<<endl;
                else
                {
                    cout<<i<<": is not null"<<endl;
                    cout<<subset[i]->data[0]<<endl;
                }
            }
            cout<<"........................................"<<endl;

            temp = temp->next;
        }
    }



private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const
        {return child_count==0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
                                        // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i);    //merge subset i with subset i+1

};


//CTOR
template <typename T>
BPlusTree<T>::BPlusTree(bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    next = nullptr;

    for(int i=0; i<(MAXIMUM+1); i++)
    {
        data[i] = T();
    }

    for(int j=0; j<(MAXIMUM+2); j++)
    {
        subset[j] = nullptr;
    }
}

template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    next = nullptr;

     for(int i=0; i<(MAXIMUM+1); i++)
    {
        data[i] = T();
    }

    for(int j=0; j<(MAXIMUM+2); j++)
    {
        subset[j] = nullptr;
    }

    for(int i=0; i<size; i++)
    {
        this->insert(a[i]);
    }
}


//Big Three
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other)
{
    dups_ok = other.dups_ok;
    copy_tree(other);
}

template <typename T>
BPlusTree<T>::~BPlusTree()
{
    clear_tree();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator = (const BPlusTree<T>& RHS)
{
    clear_tree();

    copy_tree(RHS);

    dups_ok = RHS.dups_ok;

    return *this;
}


//Public Functions
template <typename T>
void BPlusTree<T>::insert(const T& entry)
{
    //call loose_insert
    loose_insert(entry);
    
    //fix the root excess
    if(data_count > MAXIMUM)
    {
        //create a new node
        BPlusTree<T>* temp = new BPlusTree<T>(dups_ok);

        //copy this to new node
        temp->copy_tree(*this);

        //clear this root node
        clear_tree();

        //make the new node this root's only child (subset[0])
        data_count = 0;
        child_count = 1;
        subset[0] = temp;

        //call fix_excess on this only subset
        fix_excess(0);
    }
}

template <typename T>
void BPlusTree<T>::remove(const T& entry)
{

    //call loose_remove
    loose_remove(entry);

    //fix root shortage
    if(data_count==0 && child_count==1) 
    {
        // point a temporary pointer (shrink_ptr) 
        // and point it to this root's only subset
        BPlusTree<T>* temp = subset[0];

        // copy all the data and subsets of this subset into the root 
        // (through shrink_ptr)
        copy_tree(*temp);
        
        // now, simply delete shrink_ptr (blank out child), 
        // and the tree has shrunk by one level.
        temp->child_count=0;
        delete temp;
    }

    //this may fix empty error
    //when there is only one node, subsets not all point to null
    //even child_count is 0
    for(int i=child_count; i<(MAXIMUM+2); i++)
    {
        subset[i] = nullptr;
    }

}

template <typename T>
void BPlusTree<T>::clear_tree()
{
    //go through whole tree until leaf
    if(!is_leaf())
    {
        //delete children
        for(int i=0; i<child_count; i++)
        {
            subset[i]->clear_tree();

            delete subset[i];
            subset[i] = nullptr;
        }
    }

    //set count to 0
    data_count = 0;
    child_count = 0;
    next = nullptr;
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)
{
    stack<BPlusTree<T>*> s;

    copy_tree(other, s);
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, stack<BPlusTree<T>*>& s)
{
    dups_ok = other.dups_ok;
    
    //when a new btree ptr create, data_count will be 0
    //data_count will pass by ref during this function
    copy_array(data, other.data, data_count, other.data_count);
    child_count = other.child_count;

    //if other is not a leaf
    //for every child of the other, copy other's child as my own
    if(!other.is_leaf())
    {
        for(int i=0; i<other.child_count; i++)
        {
            subset[i] = new BPlusTree<T>(dups_ok);

            subset[i]->copy_tree(*other.subset[i], s);
        }
    }

    //if other is a leaf
    //point last_leaf's next to this
    //node if last_leaf is not null
    //set last_leaf to this node
    else
    {
        if(!s.empty())
        {
            BPlusTree<T>* last_leaf = s.top();
            s.pop();
            last_leaf->next = this;
        }

        s.push(this);
    }

}

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
            return true;

        //found && !leaf
        else
            return subset[i+1]->contains(entry);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
            return false;

        //!found && !leaf
        else
            return subset[i]->contains(entry);
    }

}

template <typename T>
T& BPlusTree<T>::get(const T& entry)
{
    if(!contains(entry))
        insert(entry);

    return get_existing(entry);
}

template<typename T>
const T& BPlusTree<T>::get(const T& entry) const
{
    if(!contains(entry))
    {
        assert(false && "const get: entry does not exist");
    }

    return get_existing(entry);   
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
            return data[i];

        //found && !leaf
        else
            return subset[i+1]->get_existing(entry);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            cout<<"SHOULD NOT BE HERE"<<endl;
            return data[0];
        }

        //!found && !leaf
        else
            return subset[i]->get_existing(entry);
    }
}

template <typename T>
const T& BPlusTree<T>::get_existing(const T& entry) const
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
            return data[i];

        //found && !leaf
        else
            return subset[i+1]->get_existing(entry);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            cout<<"SHOULD NOT BE HERE"<<endl;
            return data[0];
        }

        //!found && !leaf
        else
            return subset[i]->get_existing(entry);
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
            return Iterator(this, i);

        //found && !leaf
        else
            return subset[i+1]->find(entry);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
            return Iterator();

        //!found && !leaf
        else
            return subset[i]->find(entry);
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key)
{/*
    Iterator it;

    for (it = begin(); it != end(); it++)
    {
        if(*it >= key)
            return it;
    }

    return Iterator();
    */

   int i = first_ge(data, data_count, key);
   bool found = (i<data_count && data[i]==key);

    if(found)
    {
        //found && leaf
        if(is_leaf())
            return Iterator(this, i);

        //found && !leaf
        else
            return subset[i+1]->lower_bound(key);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            //first_ge return the data count is the key is largest
            if(i < data_count)
                return Iterator(this, i);

            else
                return Iterator(next, 0);

        }

        //!found && !leaf
        else
            return subset[i]->lower_bound(key);
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key)
{
    
    /*
    Iterator it;

    for (it = begin(); it != end(); it++)
    {
        if(*it > key)
            return it;
    }

    return Iterator();
    */
   
   int i = first_ge(data, data_count, key);
   bool found = (i<data_count && data[i]==key);

    if(found)
    {
       
        //found && leaf
        if(is_leaf())
            return ++Iterator(this, i);

        //found && !leaf
        else
            return subset[i+1]->upper_bound(key);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            //first_ge return the data count is the key is largest
            if(i < data_count)
                return Iterator(this, i);

            else
                return Iterator(next, 0);

        }

        //!found && !leaf
        else
            return subset[i]->upper_bound(key);
    }
}

template <typename T>
int BPlusTree<T>::size() const
{
    int _size = 0;

    //go though the tree, add every node's data count together
    for (int i=0; i<=data_count; i++)
    {
        if (!is_leaf())
        {
            _size += subset[i]->size();
        }
    }

    _size += data_count;

    return _size;
}

template <typename T>
bool BPlusTree<T>::empty() const
{
    return (data_count == 0);
}

template <typename T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const
{   
    if(!is_leaf())
    {
        for(int i=data_count; i>0; i--)
        {
            subset[i]->print_tree(level+1, outs);

            //print last subtree
            outs << setw(level*4) << "" << data[i-1] << endl;
        }

        subset[0]->print_tree(level+1, outs);
    }

    else
    {
        for(int i=(data_count-1); i>=0; i--)
        {
            outs << setw(level*4) << "" << data[i] << endl;
        }
    }
    
}

template <typename T>
bool BPlusTree<T>::is_valid()
{
    if(is_leaf())
        return true;

    //data[data_count-1] must be less than equal to 
    //  every subset[child_count-1]->data[ ]
    if(!is_le(subset[child_count -1]->data, 
              subset[child_count - 1]->data_count, 
              data[data_count -1]))
        return false;

    //every data[i] is greater than every subset[i]->data[ ]
    for(int i=0; i<data_count; i++)
    {
         if(!is_gt(subset[i]->data, subset[i]->data_count, data[i]))
            return false;
    }

    //Every data[i] is equal to subset[i+1]->smallest
    for(int i=0; i<data_count; i++)
    {
        //get subset[i+1]->smallest
        T entry;
        subset[i+1]->get_smallest(entry);

        //compare with data[i]
        if(data[i] != entry)
            return false;
    }

    //Recursively validate every subset[i]
    for(int i=0; i<child_count; i++)
    {
        //if there is a child not valid
        if(!(subset[i]->is_valid()))
            return false;
    }

    return true;    
}

template <typename T>
string BPlusTree<T>::in_order()
{
    string str = "";
        
    int i = 0;

    for(i=0; i<data_count; i++)
    {
        if(!is_leaf())
            str += subset[i]->in_order();
            
        str += to_string(data[i]);
        str += "|";
    }

    if(!is_leaf())
        str += subset[i]->in_order();

    return str;
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin()
{
    return Iterator(get_smallest_node());
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end()
{
    return Iterator();
}


//Private Functions
template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
        {
            T* ptr = data;

            ptr += i;

            return ptr;     
        }

        //found && !leaf
        else
            return subset[i+1]->find_ptr(entry);

    }

    else
    {
        //!found && leaf
        if(is_leaf())
            return nullptr;

        //!found && !leaf
        else
            return subset[i]->find_ptr(entry);
    }
}

template <typename T>
void BPlusTree<T>::loose_insert(const T& entry)
{  
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
        {
            if(dups_ok == false)
            {
                //overwriting
                data[i] = entry;
            }

            else
            {
                //for mmap
                data[i] = data[i] + entry;
            }
        }

        //found && !leaf
        else
        {
            subset[i+1]->loose_insert(entry);
            fix_excess(i+1);
        }

    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            //insert entry at data[i]
            insert_item(data, i, data_count, entry);
        }

        //!found && !leaf
        else
        {   
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }
}

template <typename T>
void BPlusTree<T>::fix_excess(int i)
{
    //if there is excess
    if(subset[i]->data_count > MAXIMUM)
    {
        //1. add a new subset at location i+1 of this node
        insert_item(subset, i+1, child_count, new BPlusTree<T>(dups_ok));

        //2. split subset[i] (both the subset array and the data array) 
        // and move half into 
        // subset[i+1] (this is the subset we created in step 1.) 
        split(subset[i]->data, subset[i]->data_count,
              subset[i+1]->data, subset[i+1]->data_count);
        
        split(subset[i]->subset, subset[i]->child_count, 
              subset[i+1]->subset, subset[i+1]->child_count);
        
        T entry;

         //3. detach the last data item of subset[i] and 
         // bring it and insert it into this node's data[]
        detach_item(subset[i]->data, subset[i]->data_count, entry);
        ordered_insert(data, data_count, entry);

        if(subset[i]->is_leaf())
        {
            //transfer the middle entry to the right first
            ordered_insert(subset[i+1]->data, subset[i+1]->data_count, entry);

            //Deal with next pointers
            subset[i+1]->next = subset[i]->next;
            subset[i]->next = subset[i+1];
        }
    }
}

template <typename T>
void BPlusTree<T>::loose_remove(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if(found)
    {
        //found && leaf
        if(is_leaf())
        {
            //delete target
            T target;
            delete_item(data, i, data_count, target);
        }

        //found && !leaf
        else
        {
            subset[i+1]->loose_remove(entry);

            //no shortage
            if(subset[i+1]->data_count >= MINIMUM)
            {
                //replace data[i] with subset[i+1]->smallest
                subset[i+1]->get_smallest(data[i]);
            }

            //shortage
            else
            {         
                //hold subset[i]
                BPlusTree<T>* temp = fix_shortage(i+1);
                
                
                //search for entry in data[]
                int index = search_item(data, data_count, entry);
                
                //if find entry in data[]
                if(index != -1)
                {
                    //replace it with subset[i+1]->smallest
                    subset[i+1]->get_smallest(data[index]);
                }

                //if not find entry in data[]
                else
                {               
                    //search for entry in subset[i]
                    int index2 = search_item(temp->data, temp->data_count, entry);

                    //if find in subset
                    if(index2 != -1)
                    {
                        //replace it with subset[i+1]->smallest
                        temp->subset[index2+1]->get_smallest(temp->data[index2]);
                    }

                    else
                    {
                        //data[i] gone
                    }
                }
                
            }
            
        }
    }

    else
    {
        //!found && leaf
        if(is_leaf())
        {
            //nothing to do
        }

        //!found && !leaf
        else
        {
            subset[i]->loose_remove(entry);

            fix_shortage(i);
        }
    }

}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i)
{
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */

    if(subset[i]->data_count < MINIMUM)
    {
        if(i+1<child_count && subset[i+1]->data_count>MINIMUM)
        {
            transfer_left(i+1);
            return subset[i];
        }

        else if(i>0 && subset[i-1]->data_count>MINIMUM)
        {
            transfer_right(i-1);
            return subset[i];
        }

        else if(i>0 && subset[i-1]!=nullptr)
            return merge_with_next_subset(i-1);
            
        else
            return merge_with_next_subset(i);
    }

    else
        return subset[i];
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    //keep going to the first subset 
    if(!is_leaf())
        return subset[0]->get_smallest_node();

    //if here is leaf, then this node is smallest node, return this
    else
        return this;
}

template <typename T>
void BPlusTree<T>::get_smallest(T& entry)
{
    //keep going to the first subset 
    if(!is_leaf())
        subset[0]->get_smallest(entry);

    //if here is leaf, then this node is smallest node, return the first element
    else
        entry = data[0];
}

template <typename T>
void BPlusTree<T>::get_biggest(T& entry)
{
    //keep going to the last subset 
    if(!is_leaf())
        return subset[child_count-1]->get_biggest(entry);

    //if here is leaf, then this node is biggest node, return the last element
    else
        entry = data[data_count-1];
}

template <typename T>
void BPlusTree<T>::remove_biggest(T& entry)
{
    //no more remove_biggest
}

template <typename T>
void BPlusTree<T>::transfer_left(int i)
{
    if(i > 0)
    {
        //rotate_left()
        //if subset is not leaf
        if(!(subset[i]->is_leaf()))
        {
            //data[i-1] moves down to the RIGHT of subset[i-1]->data
            T entry;
            delete_item(data, i-1, data_count, entry);
            attach_item(subset[i-1]->data, subset[i-1]->data_count, entry);

            //FIRST item of subset[i]->data moves up to data to replace data[i-1]
            T first;
            delete_item(subset[i]->data, 0, subset[i]->data_count, first);
            ordered_insert(data, data_count, first);

            //shift first subset of subset[i] to end of subset[i-1]
            BPlusTree<T>* first_subset;
            delete_item(subset[i]->subset, 0, subset[i]->child_count, 
                        first_subset);
            attach_item(subset[i-1]->subset, subset[i-1]->child_count, 
                        first_subset);
        }

        //transfer_left()
        //if subset is leaf
        else
        {
            //1. transfer subset[i(1)]->data[0] to end of subset[i-1]
            T entry;
            delete_item(subset[i]->data, 0, subset[i]->data_count, entry);
            attach_item(subset[i-1]->data, subset[i-1]->data_count, entry);

            //2. update parent of the transfered subset
            data[i-1] = subset[i]->data[0]; 
        }
    }

    else
        cout << "transfer_left: i should greater than 0" << endl;
}

template <typename T>
void BPlusTree<T>::transfer_right(int i)
{
    if(i+1 < child_count)
    {
        //rotate_left()
        //if subset is not leaf
        if(!(subset[i]->is_leaf()))
        {
            //data[i] moves down to the LEFT of subset[i+1]->data
            T entry;
            delete_item(data, i, data_count, entry);
            ordered_insert(subset[i+1]->data, subset[i+1]->data_count, entry);

            //LAST item of subset[i]->data moves up to data to replace data[i],
            T last;
            detach_item(subset[i]->data, subset[i]->data_count, last);
            ordered_insert(data, data_count, last);

            //shift last subset of subset[i] to front of subset[i+1]
            BPlusTree<T>* last_subset;
            detach_item(subset[i]->subset, subset[i]->child_count, last_subset);
            insert_item(subset[i+1]->subset, 0, subset[i+1]->child_count, 
                        last_subset);
        }

        //transfer_right()
        //if subset is leaf
        else
        {
            //1. transfer subset[i]->data[last] to left of subset[i+1]->data
            T entry;
            detach_item(subset[i]->data, subset[i]->data_count, entry);
            ordered_insert(subset[i+1]->data, subset[i+1]->data_count, entry);

            //2. update parent of the transfered subset
            data[i] = subset[i+1]->data[0];
        }
    }

    else
        cout << "transfer_right: i+1 should less than child_count" << endl;
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i)
{
    //1. remove data[i] from this object
    T entry;
    delete_item(data, i, data_count, entry);

    //2. if not a leaf, append it to child[i]->data:
    if(!(subset[i]->is_leaf()))
        attach_item(subset[i]->data, subset[i]->data_count, entry);

    //3. Move all data items from subset[i+1]->data to subset[i]->data
    merge(subset[i]->data, subset[i]->data_count,
          subset[i+1]->data, subset[i+1]->data_count);

    //4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
    merge(subset[i]->subset, subset[i]->child_count,
          subset[i+1]->subset, subset[i+1]->child_count);

    //5. delete subset[i+1] (store in a temp ptr)
    BPlusTree<T>* temp;
    delete_item(subset, i+1, child_count, temp);

    //6. if a leaf, point subset[i]->next to temp_ptr->next
    if(subset[i]->is_leaf())
        subset[i]->next = temp->next;
    
    //7. delete temp ptr
    delete temp;
    
    return subset[i];
}

#endif
