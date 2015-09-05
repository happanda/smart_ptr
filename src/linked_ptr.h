#ifndef SMART_POINTERS_LINKED_PTR_H
#define SMART_POINTERS_LINKED_PTR_H


struct list_node
{
    list_node();
    ~list_node();
    list_node(list_node& rhs);
    list_node const& operator=(list_node& rhs);

    void link(list_node&);

    void unlink();
    bool unique() const;

    void swap(list_node& other);

private:
    //links to next and previous ptrs in list
    list_node* next{ nullptr };
    list_node* prev{ nullptr };
};

struct custom_deleter_base;

template<class T>
class linked_ptr
{
private:
    typedef void (linked_ptr<T>::*bool_type)() const;
    typedef linked_ptr<T> this_type;

public:
    linked_ptr();
    linked_ptr(linked_ptr<T> const& other);
    linked_ptr<T> const& operator=(linked_ptr<T> const& other);
    ~linked_ptr();

    template<class S> linked_ptr(S* data);
    template<class S> linked_ptr(linked_ptr<S> const& other);
    template<class S> linked_ptr<T> const& operator=(linked_ptr<S> const& other);
    template<class S, class D> linked_ptr(S* data, D deleter);

    void reset();
    void reset(T* data);

    T* get();
    T const* get() const;

    bool unique() const;

    T& operator*();
    T const& operator*() const;
    T* operator->();
    T const* operator->() const;

    operator bool_type() const;

    void swap(linked_ptr<T>& other);

private:
    T* mData{ nullptr };
    mutable list_node mNode;
    custom_deleter_base* mDeleter{ nullptr };

    typedef void (linked_ptr<T>::*bool_type)() const;
    void bool_test_function() const;

    template<class S>
    friend class linked_ptr;
};

template<class T>
bool operator==(const linked_ptr<T>& left, const linked_ptr<T>& right);
template<class T>
bool operator!=(const linked_ptr<T>& left, const linked_ptr<T>& right);
template<class T>
bool operator<(const linked_ptr<T>& left, const linked_ptr<T>& right);

#include "linked_ptr.hpp"

#endif
