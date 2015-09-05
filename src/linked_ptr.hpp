#ifndef SMART_POINTERS_LINKED_PTR_CPP
#define SMART_POINTERS_LINKED_PTR_CPP

#include <utility> // for swap

/*********************************************************/
/*                      list_node                        */
list_node::list_node()
{
}

list_node::~list_node()
{
    unlink();
}

list_node::list_node(list_node& rhs)
{
    link(rhs);
}

list_node const& list_node::operator=(list_node& rhs)
{
    link(rhs);
    return *this;
}

void list_node::link(list_node& rhs)
{
    if (this != &rhs)
    {
        unlink();
        prev = rhs.prev;
        next = &rhs;
        if (prev != nullptr)
            prev->next = this;
        rhs.prev = this;
    }
}

void list_node::unlink()
{
    if (prev != nullptr)
        prev->next = next;
    if (next != nullptr)
        next->prev = prev;
    prev = nullptr;
    next = nullptr;
}

bool list_node::unique() const
{
    return (prev == nullptr && next == nullptr);
}

void list_node::swap(list_node& rhs)
{
    if (this == &rhs)
        return;
    {
        list_node thisCopy(*this);
        list_node rhsCopy(rhs);
        rhs = thisCopy;
        *this = rhsCopy;
    }
}

/*********************************************************/
/*                      deleter                          */
struct custom_deleter_base
{
    virtual void destroy(void const* ptr) const = 0;
};

template<class T, class D>
struct custom_deleter : public custom_deleter_base
{
    custom_deleter(D deleter)
        : mDeleter(deleter)
    {
    }

    void destroy(void const* ptr) const
    {
        mDeleter(static_cast<T const*>(ptr));
    }

private:
    D const mDeleter;
};

/*********************************************************/
/*                     linked_ptr                        */

template<class T>
linked_ptr<T>::linked_ptr()
{
}

template<class T>
linked_ptr<T>::linked_ptr(linked_ptr<T> const& rhs)
    : mData(rhs.mData)
    , mNode(rhs.mNode)
{
}

template<class T>
linked_ptr<T> const& linked_ptr<T>::operator=(linked_ptr<T> const& rhs)
{
    if (mData != rhs.mData)
    {
        this_type(rhs).swap(*this);
    }
    return *this;
}

template<class T>
linked_ptr<T>::~linked_ptr()
{
    reset();
}

template<class T>
template<class S>
linked_ptr<T>::linked_ptr(S* data)
    : mData(data)
{
}

template<class T>
template<class S>
linked_ptr<T>::linked_ptr(linked_ptr<S> const& rhs)
    : mData(rhs.mData)
    , mNode(rhs.mNode)
{
}

template<class T>
template<class S>
linked_ptr<T> const& linked_ptr<T>::operator=(linked_ptr<S> const& rhs)
{
    if (mData != rhs.mData)
    {
        this_type(rhs).swap(*this);
    }
    return *this;
}

template<class T>
template<class S, class D>
linked_ptr<T>::linked_ptr(S* data, D deleter)
    : mData(data)
    , mDeleter(new custom_deleter<T, D>(deleter))
{
}

template<class T>
void linked_ptr<T>::reset()
{
    if (mNode.unique())
    {
        if (!mDeleter)
            delete mData;
        else
            mDeleter->destroy(static_cast<void const*>(mData));
    }
    else
        mNode.unlink();
    mData = nullptr;
}

template<class T>
void linked_ptr<T>::reset(T* data)
{
    reset();
    mData = data;
}

template<class T>
T* linked_ptr<T>::get()
{
    return mData;
}

template<class T>
T const* linked_ptr<T>::get() const
{
    return mData;
}

template<class T>
bool linked_ptr<T>::unique() const
{
    return mNode.unique();
}

template<class T>
T& linked_ptr<T>::operator*()
{
    return *mData;
}

template<class T>
T const& linked_ptr<T>::operator*() const
{
    return *mData;
}

template<class T>
T* linked_ptr<T>::operator->()
{
    return mData;
}

template<class T>
T const* linked_ptr<T>::operator->() const
{
    return mData;
}

template<class T>
bool operator==(linked_ptr<T> const& left, linked_ptr<T> const& right)
{
    return left.get() == right.get();
}

template<class T>
bool operator!=(linked_ptr<T> const& left, linked_ptr<T> const& right)
{
    return !(left == right);
}

template<class T>
bool operator<(linked_ptr<T> const& left, linked_ptr<T> const& right)
{
    return left.get() < right.get();
}

template<class T>
void linked_ptr<T>::bool_test_function() const
{
}

template<class T>
linked_ptr<T>::operator bool_type() const
{
    return mData != nullptr ? &linked_ptr<T>::bool_test_function : nullptr;
}

template<class T>
void linked_ptr<T>::swap(linked_ptr<T>& rhs)
{
    if (mData != rhs.mData)
    {
        std::swap(mData, rhs.mData);
        mNode.swap(rhs.mNode);
    }
}

#endif
