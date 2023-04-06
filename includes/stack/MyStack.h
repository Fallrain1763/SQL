#ifndef MYSTACK_H
#define MYSTACK_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename ITEM_TYPE>
class Stack
{
public:
    class Iterator{
        public:

        //give access to list to access _ptr
        friend class Stack;

        //default ctor
        Iterator()
        {
            _ptr = NULL;
        }

        //Point Iterator to where p is pointing to
        Iterator(node<ITEM_TYPE>* p)
        {
            _ptr = p;
        }

        //dereference operator
        ITEM_TYPE operator *()
        {
            return _ptr->_item;
        }

        //member access operator
        node<ITEM_TYPE>* operator ->()
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
            assert(it._ptr != NULL);

            Iterator hold;
            hold = it;
            it._ptr = it._ptr->_next;

            return hold;
        }

        private:
            node<ITEM_TYPE>* _ptr;                //pointer being encapsulated
        };

        //CTOR
        Stack();

        //Big three
        Stack(const Stack<ITEM_TYPE>& copyMe);
        ~Stack();
        Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);

        ITEM_TYPE top();
        bool empty();
        void push(ITEM_TYPE item);
        ITEM_TYPE pop();
        int size() const { return _size; }

        Iterator begin() const;                   //Iterator to the head node
        Iterator end() const;                     //Iterator to NULL

        template<typename T>
        friend ostream& operator<<(ostream& outs, const Stack<T>& printMe)
        {
            node<T>* walker = printMe._top;

            cout<<"H->";

            while(walker != NULL)
            {
                outs << *walker;
                walker = walker->_next;
            }

            outs<<"|||"<<endl;

            return outs;
        }

    private:
        node<ITEM_TYPE>* _top;
        int _size;

};

//CTOR
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack()
{
    _top = NULL;
    _size = 0;
}

//Big Three
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE> &copyMe)
{
    _top = _copy_list(copyMe._top);
    _size = copyMe._size;
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack()
{
    if(_top != NULL && _size != 0)
    {
        node<ITEM_TYPE> * node = NULL;
	    while (_top != NULL)
	    {
		    node = _top;
		    _top = _top->_next;
		    delete node;
	    }

        _top = NULL;

        _size = 0;

    }
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE> & Stack<ITEM_TYPE>::operator=(const Stack& RHS)
{
    if(this == &RHS)
        return *this;

    else
    {
        //delete LHS
        if(_top != NULL && _size != 0)
        {
            node<ITEM_TYPE> * node = NULL;
	        while (_top != NULL)
	        {
		        node = _top;
		        _top = _top->_next;
		        delete node;
	        }
            
            _top = NULL;

            _size = 0;
        }

        //copy from RHS to LHS
        _top = _copy_list(RHS._top);
        _size = RHS._size;
    }

    return *this;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top()
{
    return _top->_item;
}

template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty()
{
    assert((_top != NULL && _size != 0) ||
           (_top == NULL && _size == 0));

    if(_top == NULL && _size == 0)
    {
        return true;
    }

    else
    {
        return false;
    }

}

template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item)
{
    _size++;

    _insert_head(_top, item);
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop()
{
    assert(_top != NULL && _size != 0);

    _size--;

    return _delete_node(_top,_top);
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const
{
    return Iterator(_top);
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const
{
    return Iterator();
}

#endif // MYSTACK_H
