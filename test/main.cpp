#include <cstdio>
#include <cstdlib>
#include <memory>
#include "gtest/gtest.h"
#include "general_tests.h"
#include "linked_ptr.h"

class foo
{
};

struct foo_deleter
{
    void operator()(foo const* ptr) const
    {
        delete ptr;
    }
};

int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   int code = RUN_ALL_TESTS();
   getchar();
   linked_ptr<foo> pInt(new foo, foo_deleter());
   return code;
}
