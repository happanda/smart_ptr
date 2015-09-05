#include <cstdio>
#include <cstdlib>
#include <memory>
#include "gtest/gtest.h"
#include "general_tests.h"
#include "linked_ptr.h"

using std::shared_ptr;

class foo
{
};

struct foo_deleter
{
    void operator()(foo* ptr) const
    {
        delete ptr;
    }
};

int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   int code = RUN_ALL_TESTS();
   getchar();
   shared_ptr<foo> pFoo(new foo, foo_deleter());
   shared_ptr<foo> qFoo(pFoo);
   return code;
}
