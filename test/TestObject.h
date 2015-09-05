#ifndef TEST_SMART_POINTERS_TEST_OBJECT_H
#define TEST_SMART_POINTERS_TEST_OBJECT_H

#include <cstring>

using std::string;

class TestObject
{
public:
   TestObject(const char* message)
   {
      msg = string(message);
   }
   string msg;
   virtual ~TestObject()
   {
   }
};

class TestObjectDerive : public TestObject
{
public:
   TestObjectDerive(const char* message, int i): TestObject(message)
   {
      value = i;
   }
   int value;
   virtual ~TestObjectDerive()
   {
   }
};

#endif