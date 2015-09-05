#ifndef SMART_POINTERS_LINKED_PTR_H
#define SMART_POINTERS_LINKED_PTR_H
#include <memory>


struct list_node
{
    list_node();
    ~list_node();
    list_node(list_node& rhs);
    list_node const& operator=(list_node& rhs);

    void link(list_node&);

    void unlink();
    bool unique() const;

    void swap(list_node& rhs);

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

    linked_ptr(linked_ptr<T> const& rhs);
    linked_ptr<T> const& operator=(linked_ptr<T> const& rhs);

    linked_ptr(linked_ptr<T>&& rhs);
    linked_ptr<T> const& operator=(linked_ptr<T>&& rhs);

    ~linked_ptr();

    template<class S> linked_ptr(S* data);
    template<class S> linked_ptr(linked_ptr<S> const& rhs);
    template<class S> linked_ptr<T> const& operator=(linked_ptr<S> const& rhs);
    template<class S, class D> linked_ptr(S* data, D deleter);

    template<class S> linked_ptr(std::auto_ptr<S>&& rhs);
    template<class S, class D> linked_ptr(std::unique_ptr<S, D>&& rhs);

    template<class S> linked_ptr const& operator=(std::auto_ptr<S>&& rhs);
    template<class S, class D> linked_ptr const& operator=(std::unique_ptr<S, D>&& rhs);

    void reset();
    void reset(T* data);
    template<class D>
    void reset(T* data, D d);

    T* get();
    T const* get() const;

    bool unique() const;

    T& operator*();
    T const& operator*() const;
    T* operator->();
    T const* operator->() const;

    operator bool_type() const;

    void swap(linked_ptr<T>& rhs);

private:
    T* mData{ nullptr };
    mutable list_node mNode;
    custom_deleter_base* mDeleter{ nullptr };

    typedef void (linked_ptr<T>::*bool_type)() const;
    void bool_test_function() const;

    template<class S>
    friend class linked_ptr;
};


template<class T, class... Args>
linked_ptr<T> make_linked(Args&&... args);


template<class T>
bool operator==(const linked_ptr<T>& left, const linked_ptr<T>& right);
template<class T>
bool operator!=(const linked_ptr<T>& left, const linked_ptr<T>& right);
template<class T>
bool operator<(const linked_ptr<T>& left, const linked_ptr<T>& right);

#include "linked_ptr.hpp"

#endif
