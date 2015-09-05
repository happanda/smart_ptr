#include <stdio.h>
#include "linked_ptr.h"
#include "TestObject.h"

using std::cout;
using std::endl;

class Linked_Ptr_General_Tests : public ::testing::Test
{
protected:
    static std::string const WRONG_DATA;
    static std::string const ERROR_COPY;
    static std::string const ERROR_ASSIGN;
    static std::string const ERROR_UNIQUE;
    static std::string const ERROR_NOT_UNIQUE;
    static std::string const ERROR_FUNC;
    static std::string const ERROR_EQUALITY;
    static std::string const ERROR_INEQUALITY;
    static std::string const ERROR_LESS;
    static std::string const ERROR_FOUND;
    static std::string const ERROR_NOT_FOUND;
protected:
    int const MAX_ITERATIONS;
    char const* hello;
    char const* goodbye;
    int const value1;
    int const value2;
    linked_ptr<TestObject> p_to;
    linked_ptr<TestObject> q_to;
    linked_ptr<TestObjectDerive> r_tod;
    linked_ptr<TestObjectDerive> s_tod;

public:
    Linked_Ptr_General_Tests()
        : MAX_ITERATIONS(10000)
        , hello("Hello")
        , goodbye("Goodbye")
        , value1(100)
        , value2(200)
        , p_to(new TestObject(hello))
        , q_to(new TestObject(goodbye))
        , r_tod(new TestObjectDerive(hello, value1))
        , s_tod(new TestObjectDerive(goodbye, value2))
    {
        cout << "Created several smart pointers" << endl;
        cout << "Created simple object with message: " << p_to.get()->msg << endl;
        cout << "Created simple object with message: " << q_to->msg << endl;
        cout << "Created object with message: " << (*r_tod).msg << " and value: " << r_tod.get()->value << endl;
        cout << "Created object with message: " << s_tod.get()->msg << " and value: " << s_tod->value << endl;
        cout << "Constructor finished" << endl;
    }
};

std::string const Linked_Ptr_General_Tests::WRONG_DATA{ "Wrong data pointed!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_COPY{ "Error copying pointer!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_ASSIGN{ "Error assigning pointer!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_UNIQUE{ "Error: pointer is unique!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_NOT_UNIQUE{ "Error: pointer is NOT unique!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_FUNC{ "Error in fucntion!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_EQUALITY{ "Error with equality!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_INEQUALITY{ "Error with INequality!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_LESS{ "Error with less operator!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_FOUND{ "Error: pointer found!!\n" };
std::string const Linked_Ptr_General_Tests::ERROR_NOT_FOUND{ "Error: pointer NOT found!!\n" };

TEST_F(Linked_Ptr_General_Tests, Swapping)
{
    cout << "TEST swapping" << endl;

    cout << "Get underlying objects from pointers" << endl;
    TestObject* to = p_to.get();
    EXPECT_STREQ(hello, to->msg.c_str()) << WRONG_DATA;
    TestObjectDerive* tod = r_tod.get();
    EXPECT_STREQ(hello, tod->msg.c_str()) << WRONG_DATA;
    EXPECT_EQ(value1, tod->value) << WRONG_DATA;

    cout << "Self-swap" << endl;
    p_to.swap(p_to);
    cout << "Message after self-swap: " << p_to.get()->msg << endl;
    r_tod.swap(r_tod);
    cout << "Message after self-swap: " << r_tod->msg << endl;

    cout << "Swap two pointers" << endl;
    p_to.swap(q_to);
    EXPECT_STREQ(goodbye, p_to->msg.c_str()) << WRONG_DATA;
    EXPECT_STREQ(hello, (*q_to).msg.c_str()) << WRONG_DATA;
    cout << "Swap objects and call using -> and * operators:" << endl;
    cout << "'" << p_to->msg << "' and '" << (*q_to).msg << "'" << endl;

    r_tod.swap(s_tod);
    EXPECT_STREQ(goodbye, r_tod->msg.c_str()) << WRONG_DATA;
    EXPECT_STREQ(hello, (*s_tod).msg.c_str()) << WRONG_DATA;
    EXPECT_EQ(value2, r_tod->value) << WRONG_DATA;
    EXPECT_EQ(value1, (*s_tod).value) << WRONG_DATA;

    cout << "Swapping pointers to same data" << endl;
    linked_ptr<TestObject> p_to_copy(p_to);
    EXPECT_EQ(p_to.get(), p_to_copy.get()) << ERROR_COPY;
    EXPECT_FALSE(p_to.unique()) << ERROR_COPY;
    EXPECT_FALSE(p_to_copy.unique()) << ERROR_COPY;
    p_to.swap(p_to_copy);
    EXPECT_EQ(p_to.get(), p_to_copy.get()) << "Swap: " << ERROR_FUNC;
    EXPECT_STREQ(goodbye, p_to->msg.c_str()) << WRONG_DATA;
    EXPECT_STREQ(goodbye, p_to_copy->msg.c_str()) << WRONG_DATA;

    cout << "Swap successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Copying)
{
    cout << "TEST copying" << endl;
    {
        linked_ptr<TestObject> p_to_copy(p_to);
        cout << "Created copy of p_to pointer" << endl;
        EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
        EXPECT_FALSE(p_to_copy.unique()) << ERROR_UNIQUE;
    }
    cout << "Copied pointer destroyed cause out of scope" << endl;
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;

    cout << "Copying inside a big cycle" << endl;
    {
        for (int i = 0; i < MAX_ITERATIONS; ++i)
        {
            linked_ptr<TestObject> p_to_iter_copy1(p_to);
            linked_ptr<TestObject> p_to_iter_copy2(p_to);
            EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
            EXPECT_FALSE(p_to_iter_copy1.unique()) << ERROR_UNIQUE;
            EXPECT_FALSE(p_to_iter_copy2.unique()) << ERROR_UNIQUE;
        }
    }
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    cout << "Copying successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Assignment)
{
    cout << "TEST assignment" << endl;
    {
        linked_ptr<TestObject> p_to_copy = p_to;
        cout << "Assinged p_to to another pointer" << endl;
        EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
        EXPECT_FALSE(p_to_copy.unique()) << ERROR_UNIQUE;
    }
    cout << "Assigned pointer destroyed cause out of scope" << endl;
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;

    cout << "Assigning inside a big cycle" << endl;
    {
        for (int i = 0; i < MAX_ITERATIONS; ++i)
        {
            linked_ptr<TestObject> p_to_iter_copy1;
            linked_ptr<TestObject> p_to_iter_copy2 = p_to_iter_copy1 = p_to;
            EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
            EXPECT_FALSE(p_to_iter_copy1.unique()) << ERROR_UNIQUE;
            EXPECT_FALSE(p_to_iter_copy2.unique()) << ERROR_UNIQUE;
        }
    }
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    cout << "Assignment successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Reset)
{
    cout << "TEST reset" << endl;
    cout << "Copying pointer p_to" << endl;
    linked_ptr<TestObject> p_to_copy1 = p_to;
    EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
    EXPECT_FALSE(p_to_copy1.unique()) << ERROR_UNIQUE;

    cout << "Reset 2nd pointer" << endl;
    p_to_copy1.reset();
    EXPECT_EQ(nullptr, p_to_copy1.get()) << "Pointer not nullptr after reset!!" << endl;
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_TRUE(p_to_copy1.unique()) << ERROR_NOT_UNIQUE;

    linked_ptr<TestObject> p_to_copy2 = p_to_copy1 = p_to;
    p_to_copy1.reset(new TestObject(goodbye));
    p_to_copy2.reset(new TestObject(goodbye));
    cout << "Copied two pointers, reseted them to new data" << endl;
    EXPECT_STREQ(goodbye, p_to_copy1->msg.c_str()) << WRONG_DATA;
    EXPECT_STREQ(goodbye, p_to_copy2->msg.c_str()) << WRONG_DATA;
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_TRUE(p_to_copy1.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_TRUE(p_to_copy2.unique()) << ERROR_NOT_UNIQUE;
    cout << "Reset successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Logical_operators)
{
    cout << "TEST logical operators" << endl;
    cout << "Test on pointers to same data" << endl;
    linked_ptr<TestObject> p_to_copy(p_to);
    EXPECT_TRUE(p_to == p_to_copy) << ERROR_EQUALITY;
    EXPECT_FALSE(p_to != p_to_copy) << ERROR_EQUALITY;
    EXPECT_TRUE(!(p_to < p_to_copy) && !(p_to_copy < p_to)) << ERROR_LESS;

    cout << "Test on pointers to different data" << endl;
    EXPECT_FALSE(p_to == q_to) << ERROR_INEQUALITY;
    EXPECT_TRUE(p_to != q_to) << ERROR_INEQUALITY;
    EXPECT_FALSE(!(p_to < q_to) && !(q_to < p_to)) << ERROR_LESS;

    cout << "Test conversion to bool" << endl;
    EXPECT_TRUE((bool)p_to) << "Coversion to bool gave False while pointer's not nullptr" << endl;
    cout << "Reset pointer" << endl;
    p_to.reset();
    EXPECT_FALSE(p_to) << "Coversion to bool gave True while pointer is nullptr" << endl;
    cout << "Logical operators successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Some_load)
{
    cout << "TEST mass copying/wiping pointers" << endl;
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    linked_ptr<TestObject>* p_to_array = new linked_ptr<TestObject>[MAX_ITERATIONS];
    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        linked_ptr<TestObject> p_to_copy1(p_to);
        linked_ptr<TestObject> p_to_copy2 = p_to;
        EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
        EXPECT_FALSE(p_to_copy1.unique()) << ERROR_UNIQUE;
        EXPECT_FALSE(p_to_copy2.unique()) << ERROR_UNIQUE;
        p_to_copy1.reset();
        p_to_array[i] = p_to;
    }
    cout << "Cycle of repeated copying and wiping pointers is successfull" << endl;
    for (int i = MAX_ITERATIONS - 1; i >= 0; i--)
    {
        EXPECT_FALSE(p_to.unique()) << ERROR_UNIQUE;
        p_to_array[i].reset();
    }
    EXPECT_TRUE(p_to.unique()) << ERROR_NOT_UNIQUE;
    cout << "Mass copying/wiping pointers successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Derivation)
{
    cout << "TEST derivation" << endl;

    cout << "Clone derived class pointer to base" << endl;
    linked_ptr<TestObject> r_tod_cast_copy(r_tod);
    EXPECT_EQ(r_tod.get(), r_tod_cast_copy.get()) << ERROR_EQUALITY;
    EXPECT_STREQ(r_tod->msg.c_str(), r_tod_cast_copy->msg.c_str()) << WRONG_DATA;
    EXPECT_FALSE(r_tod.unique()) << ERROR_UNIQUE;
    EXPECT_FALSE(r_tod_cast_copy.unique()) << ERROR_UNIQUE;

    cout << "Assign to base class ptr" << endl;
    linked_ptr<TestObject> s_tod_cast_copy = s_tod;
    EXPECT_EQ(s_tod.get(), s_tod_cast_copy.get()) << ERROR_EQUALITY;
    EXPECT_STREQ(s_tod->msg.c_str(), s_tod_cast_copy->msg.c_str()) << WRONG_DATA;
    EXPECT_FALSE(s_tod.unique()) << ERROR_UNIQUE;
    EXPECT_FALSE(s_tod_cast_copy.unique()) << ERROR_UNIQUE;

    r_tod.reset();
    EXPECT_TRUE(r_tod_cast_copy.unique()) << ERROR_NOT_UNIQUE;

    cout << "Derivation successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, STLsequentialContainers)
{
    cout << "TEST STLsequentialContainers" << endl;

    cout << "Creating vector of linked_ptr's" << endl;
    std::vector<linked_ptr<TestObjectDerive>> vp;
    vp.push_back(r_tod);
    vp.push_back(s_tod);
    cout << "Pushed 2 pointers into the vector" << endl;
    TestObjectDerive* tod1 = vp[0].get();
    TestObjectDerive* tod2 = vp[1].get();
    cout << "Extracted two pointers" << endl;
    EXPECT_STREQ(hello, tod1->msg.c_str()) << WRONG_DATA;
    EXPECT_STREQ(goodbye, tod2->msg.c_str()) << WRONG_DATA;
    EXPECT_EQ(value1, tod1->value) << WRONG_DATA;
    EXPECT_EQ(value2, tod2->value) << WRONG_DATA;
    cout << "Push another object" << endl;
    linked_ptr<TestObjectDerive> r(new TestObjectDerive(goodbye, value2));
    vp.push_back(r);
    EXPECT_STREQ(goodbye, vp[2]->msg.c_str()) << WRONG_DATA;
    EXPECT_EQ(value2, vp[2]->value) << WRONG_DATA;
    cout << "Clear vector" << endl;
    vp.clear();
    vp.resize(0);
    EXPECT_TRUE(r_tod.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_TRUE(s_tod.unique()) << ERROR_NOT_UNIQUE;

    cout << "STLsequentialContainers successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, STLassociativeContainers)
{
    cout << "TEST STLassociativeContainers" << endl;

    std::set<linked_ptr<TestObject>> sp;
    sp.insert(r_tod);
    sp.insert(s_tod);
    cout << "Created set with two elements" << endl;
    linked_ptr<TestObject> r_tod_copy(r_tod);
    linked_ptr<TestObject> s_tod_copy = s_tod;
    cout << "Created their copies and trying to find them" << endl;
    EXPECT_FALSE(sp.find(r_tod_copy) == sp.end()) << ERROR_NOT_FOUND;
    EXPECT_FALSE(sp.find(s_tod_copy) == sp.end()) << ERROR_NOT_FOUND;
    cout << "Remove elements by their copies" << endl;
    sp.erase(r_tod_copy);
    EXPECT_TRUE(sp.find(r_tod) == sp.end()) << ERROR_FOUND;
    sp.erase(s_tod_copy);
    EXPECT_TRUE(sp.find(s_tod) == sp.end()) << ERROR_FOUND;
    EXPECT_TRUE(sp.empty()) << "Set isn't empty when it must be!!" << endl;

    cout << "STLassociativeContainers successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, FromStandard)
{
    cout << "TEST construction from standard smart pointers" << endl;

    {
        cout << "Construct from auto_ptr" << endl;
        std::auto_ptr<TestObject> autoObj(new TestObject(hello));
        TestObject* to = autoObj.get();
        linked_ptr<TestObject> linkedObj(std::move(autoObj));

        EXPECT_EQ(nullptr, autoObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
    }

    {
        cout << "Construct from unique_ptr" << endl;
        std::unique_ptr<TestObject> uniObj(new TestObject(hello));
        TestObject* to = uniObj.get();
        linked_ptr<TestObject> linkedObj(std::move(uniObj));

        EXPECT_EQ(nullptr, uniObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
    }

    {
        cout << "Assign from auto_ptr" << endl;
        std::auto_ptr<TestObject> autoObj(new TestObject(hello));
        TestObject* to = autoObj.get();
        linked_ptr<TestObject> linkedObj;
        linkedObj = std::move(autoObj);

        EXPECT_EQ(nullptr, autoObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
    }

    {
        cout << "Assign from unique_ptr" << endl;
        std::unique_ptr<TestObject> uniObj(new TestObject(hello));
        TestObject* to = uniObj.get();
        linked_ptr<TestObject> linkedObj;
        linkedObj = std::move(uniObj);

        EXPECT_EQ(nullptr, uniObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
    }

    {
        cout << "Assign from auto_ptr while not empty" << endl;
        std::auto_ptr<TestObject> autoObj(new TestObject(hello));
        TestObject* to = autoObj.get();
        linked_ptr<TestObject> linkedObj(new TestObject(goodbye));
        linked_ptr<TestObject> linkedObjCopy(linkedObj);
        linkedObj = std::move(autoObj);

        EXPECT_EQ(nullptr, autoObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(hello, linkedObj->msg) << ERROR_EQUALITY;
        EXPECT_EQ(goodbye, linkedObjCopy->msg) << ERROR_EQUALITY;
        EXPECT_TRUE(linkedObj.unique()) << ERROR_NOT_UNIQUE;
        EXPECT_TRUE(linkedObjCopy.unique()) << ERROR_NOT_UNIQUE;
    }

    {
        cout << "Assign from unique_ptr while not empty" << endl;
        std::unique_ptr<TestObject> uniObj(new TestObject(hello));
        TestObject* to = uniObj.get();
        linked_ptr<TestObject> linkedObj(new TestObject(goodbye));
        linked_ptr<TestObject> linkedObjCopy(linkedObj);
        linkedObj = std::move(uniObj);

        EXPECT_EQ(nullptr, uniObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(to, linkedObj.get()) << ERROR_EQUALITY;
        EXPECT_EQ(hello, linkedObj->msg) << ERROR_EQUALITY;
        EXPECT_EQ(goodbye, linkedObjCopy->msg) << ERROR_EQUALITY;
        EXPECT_TRUE(linkedObj.unique()) << ERROR_NOT_UNIQUE;
        EXPECT_TRUE(linkedObjCopy.unique()) << ERROR_NOT_UNIQUE;
    }

    cout << "Construction from standard smart pointers successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, MakeLinked)
{
    cout << "TEST make_linked" << endl;

    linked_ptr<TestObject> linkedObj = make_linked<TestObject>(hello);
    EXPECT_NE(nullptr, linkedObj.get()) << ERROR_INEQUALITY;
    EXPECT_EQ(hello, linkedObj->msg) << ERROR_EQUALITY;
    EXPECT_TRUE(linkedObj.unique()) << ERROR_NOT_UNIQUE;

    cout << "make_linked successful" << endl;
}

TEST_F(Linked_Ptr_General_Tests, Move)
{
    cout << "TEST move construction and assignment" << endl;

    linked_ptr<TestObject> linkedObj(new TestObject(hello));
    EXPECT_NE(nullptr, linkedObj.get()) << ERROR_INEQUALITY;

    linked_ptr<TestObject> linkedObjMoved(std::move(linkedObj));
    EXPECT_EQ(nullptr, linkedObj.get()) << ERROR_EQUALITY;
    EXPECT_NE(nullptr, linkedObjMoved.get()) << ERROR_INEQUALITY;
    EXPECT_TRUE(linkedObjMoved.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_EQ(hello, linkedObjMoved->msg) << ERROR_EQUALITY;

    linkedObj = std::move(linkedObjMoved);
    EXPECT_EQ(nullptr, linkedObjMoved.get()) << ERROR_EQUALITY;
    EXPECT_NE(nullptr, linkedObj.get()) << ERROR_INEQUALITY;
    EXPECT_TRUE(linkedObj.unique()) << ERROR_NOT_UNIQUE;
    EXPECT_EQ(hello, linkedObj->msg) << ERROR_EQUALITY;

    cout << "Move successful" << endl;
}
