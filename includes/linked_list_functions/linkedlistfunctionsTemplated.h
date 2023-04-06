#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H

#include "../node/node.h"
#include "cassert"

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key);

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this);

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this);

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

//&&&&&&&&&&&&&&&&&&&& Template Function Definition &&&&&&&&&&&&&&&&&&&&

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head){
     node<T>* walker = head;

     cout<<"H->";

     while(walker != nullptr)
     {
         cout << *walker;

         walker = walker->_next;
     }

     cout<<"|||"<<endl;

}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head){
        if( head == NULL)
       {
           cout<<"H->";

           return;
       }

       _print_list_backwards(head->_next);

       cout << " <-[" << head->_item << "]-> ";
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key){

    node<T>* walker = head;

    // processing the program until whole list is checked
    while(walker != NULL)
    {
        // if the element is not found
        if(walker->_item != key)
        {
            // next
            walker = walker->_next;
        }

        else
            return walker; // return the position of the element
    }

    return NULL; // return nullptr if the element is not found
}

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){

    node<T>* temp = new node<T>;

    //temp node's item is what I want to insert
    temp->_item = insert_this;

    //point temp's next to where head_ptr point
    temp->_next = head;
    //prev to null
    temp->_prev = NULL;

    //set ori head's prew as temp
    if(head != NULL)
    head->_prev = temp;

    //point head_ptr to the temp
    head = temp;

    return head;

}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){


    node<T>* walker = head;

    // let walker point to the node which I want to insert something after it
    while(walker != after_this)
    {
        walker = walker->_next;
    }

    node<T>* temp = new node<T>;

    // temp's item is what I want to insert
    temp->_item = insert_this;

    // point temp's next point to where walker's next point
    temp->_next = walker->_next;

    // point walker to the temp
    walker->_next = temp;

    // let walker be the prev node of temp
    temp->_prev = walker;

    // set prev node of the node after new node
    if(temp->_next != NULL)
        temp->_next->_prev = temp;

    return temp;
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){

    if(before_this->_prev == NULL)
        return _insert_head(head, insert_this);

    node<T>* walker = head;

    // let walker point to the node which I want to insert something before it
    while(walker != before_this)
    {
        walker = walker->_next;
    }

    node<T>* temp = new node<T>;

    // temp's item is what I want to insert
    temp->_item = insert_this;

    // point temp's prev point to where walker's prev point
    temp->_prev = walker->_prev;

    // point walker prev to the temp
    walker->_prev = temp;

    // let walker be the next node of temp
    temp->_next = walker;

    // set next node of the node before new node
    if(temp->_next != NULL)
        temp->_prev->_next = temp;

    return temp;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){

    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){

    if (head == NULL || delete_this == NULL)
        return NULL;

    T item = delete_this->_item;

    //delete head node
    if (head == delete_this)
        head = delete_this->_next;

    //delete_this is not the tail
    if (delete_this->_next != NULL)
        delete_this->_next->_prev = delete_this->_prev;

    //delete_this is not the head
    if (delete_this->_prev != NULL)
        delete_this->_prev->_next = delete_this->_next;

    delete delete_this;

    return item;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    // declare a new header_ptr
    node<T>* c_head = NULL;

    //set a walker for order list
    node<T>* walker = head;

    // if older list is not nullptr
    if(head != NULL)
    {
        // insert the first element in older list to the new list
        _insert_head<T>(c_head, walker->_item);

        // set a walker for new list
        node<T>* c_walker = c_head;

        // point order list walker to next
        walker = walker->_next;

        // go through whole ord list
        while(walker != NULL)
        {
            // insert the rest of element after the first element one by one
            _insert_after(c_head, c_walker, walker->_item);

            //point new list walker to the node just insert
            c_walker = c_walker->_next;

            //point order list walker to the next
            walker = walker->_next;
        }

    }

    // if order list is null, just set the new list nullptr
    else
        c_head = nullptr;

    return c_head;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){

    node<T>* walker = head;

    //went through whole list
    while(walker != nullptr)
    {
        // delete node
        _delete_node(head, walker);

        walker = walker->_next;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){

     node<T>* walker = head;

    // point walker to the item
    for(int i=0; i<pos; i++)
    {
        walker = walker->_next;
    }

    // return item
    return walker->_item;
}



#endif // LINKEDLISTFUNCTIONSTEMPLATED_H