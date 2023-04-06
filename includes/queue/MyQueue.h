#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

//For copying queue
template <typename T>
node<T>* _copy_queue(node<T>* copyThis, node<T>* &back)
{
    // declare a new header_ptr
    node<T>* front = NULL;

    //set a walker for order list
    node<T>* walker = copyThis;

    // if older list is not nullptr
    if(copyThis != NULL)
    {
        // insert the first element in older list to the new list
        _insert_head<T>(front, walker->_item);

        // set a walker for new list
        node<T>* c_walker = front;

        // point order list walker to next
        walker = walker->_next;

        // go through whole ord list
        while(walker != NULL)
        {
            // insert the rest of element after the first element one by one
            _insert_after(front, c_walker, walker->_item);

            //point new list walker to the node just insert
            c_walker = c_walker->_next;

            //point order list walker to the next
            walker = walker->_next;
        }

        back = c_walker;
    }

    // if order list is null, just set the new list nullptr
    else
    {
        front = nullptr;
        back = nullptr;
    }

    return front;
}

//Queue Class
template <typename T>
class Queue
{
public:
    class Iterator{
        public:

        //give access to list to access _ptr
        friend class Queue;

        //default ctor
        Iterator()
        {
            _ptr = NULL;
        }

        //Point Iterator to where p is pointing to
        Iterator(node<T>* p)
        {
            _ptr = p;
        }

        //dereference operator
        T operator *()
        {
            return _ptr->_item;
        }

        //member access operator
        node<T>* operator ->()
        {
            return _ptr;
        }

        //casting operator: true if _ptr not NULL
        operator bool()
        {
            return _ptr != NULL;
        }

        //true if _ptr is NULL
        bool is_null()
        {
            return _ptr == NULL;
        }

        //true if left != right
        friend bool operator !=(const Iterator& left, const Iterator& right)
        {
            return left._ptr != right._ptr;
        }

        //true if left == right
        friend bool operator ==(const Iterator& left, const Iterator& right)
        {
            return left._ptr == right._ptr;
        }

        //member operator: ++it; or ++it = new_value
        Iterator& operator++()
        {
            _ptr = _ptr->_next;

            return *this;
        }

        //friend operator: it++
        friend Iterator operator++(Iterator& it, int unused)
        {
            Iterator hold;
            hold = it;
            it._ptr = it._ptr->_next;

            return hold;
        }

        private:
            node<T>* _ptr;                        //pointer being encapsulated
        };

    //CTOR
    Queue();

    //Big Three:
    Queue(const Queue<T> &copyMe);      //copy ctor
    ~Queue();                             //destor
    Queue& operator =(const Queue& RHS);  //assign operator

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();

    Iterator begin() const;                         //Iterator to the head node
    Iterator end() const;                           //Iterator to NULL
    void print_pointers();
    int size() const { return _size; }

    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    {
        node<T>* walker = printMe._front;

        cout<<"H->";

        while(walker != nullptr)
        {
            outs << *walker;
            walker = walker->_next;
        }

        outs<<"|||"<<endl;

        return outs;
    }

private:
    node<T>* _front;
    node<T>* _rear;
    int _size;

};

//CTOR
template <typename T>
Queue<T>::Queue()
{
    _front = NULL;
    _rear = NULL;
    _size = 0;
}

//Big Three
template <typename T>
Queue<T>::Queue(const Queue<T> &copyMe)
{
    _front = _copy_queue(copyMe._front,_rear);
    _size = copyMe._size;
}

template <typename T>
Queue<T>::~Queue()
{
    if(_front != NULL && _rear != NULL && _size != 0)
    {
        node<T> * node = NULL;
	    while (_front != NULL)
	    {
		    node = _front;
		    _front = _front->_next;
		    delete node;
	    }

        _front = NULL;
        _rear = NULL;
        _size = 0;
    }
}


template <typename T>
Queue<T> & Queue<T>::operator=(const Queue& RHS)
{
    if(this == &RHS)
        return *this;

    else
    {
        //delete LHS
        if(_front != NULL && _rear != NULL && _size != 0)
        {
            node<T> * node = NULL;
	        while (_front != NULL)
	        {
		        node = _front;
		        _front = _front->_next;
		        delete node;
	        }

            _front = NULL;
            _rear = NULL;
            _size = 0;
        }

        //copy from RHS to LHS
        _front = _copy_queue(RHS._front,_rear);
        _size = RHS._size;
    }

    return *this;
}

template <typename T>
bool Queue<T>::empty()
{
    assert((_front != NULL && _rear != NULL && _size != 0) ||
           (_front == NULL && _rear == NULL && _size == 0));

    if(_front == NULL && _rear == NULL && _size == 0)
    {
        return true;
    }

    else
    {
        return false;
    }

}

template <typename T>
T Queue<T>::front()
{
    return _front->_item;
}

template <typename T>
T Queue<T>::back()
{
    return _rear->_item;
}

template <typename T>
void Queue<T>::push(T item)
{
    if(_rear)
    {
        _rear = _insert_after(_front, _rear, item);
    }
    else
    {
        _rear = _insert_head(_front, item);
    }

    _size++;
}

template <typename T>
T Queue<T>::pop()
{
    assert(_front != nullptr && _rear != nullptr && _size != 0);

    _size--;

    if(_size == 0)
    {
        _rear = nullptr;
    }

    return _delete_node(_front,_front);
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const
{
    return Iterator(_front);
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const
{
    return Iterator();
}

template <typename T>
void Queue<T>::print_pointers()
{
    _print_list(_front);
}

#endif // MYQUEUE_H

