#include <iostream>
#include "gtest/gtest.h"
#include "../src/Hashmap.h"

using namespace std;

using namespace ::testing;

TEST_F(Test, testConstructor) {
    auto hashmap = new Hashmap<int32_t, int32_t>(10);
    delete hashmap;
}

TEST_F(Test, testHasFalse) {
    auto hashmap = new Hashmap<int32_t, int32_t>(10);
    ASSERT_FALSE(hashmap->has(1));
    ASSERT_FALSE(hashmap->has(11));
    ASSERT_FALSE(hashmap->has(9));
    auto var = new int32_t;
    *var = 4;
    ASSERT_FALSE(hashmap->has(*var));
    delete var;
    delete hashmap;
}

TEST_F(Test, testPut) {
    auto hashmap = new Hashmap<int32_t, int32_t>(10);
    hashmap->put(1, 9);
    hashmap->put(2, 9);
    hashmap->put(3, 8);
    hashmap->put(4, 8);
    hashmap->put(0, 1);
    hashmap->put(1, 1);
    hashmap->put(1, 2);
    hashmap->put(1, 4);
    delete hashmap;
}

TEST_F(Test, testHas) {
    auto hashmap = new Hashmap<int32_t, int32_t>(10);
    hashmap->put(1, 9);
    hashmap->put(2, 9);
    hashmap->put(3, 8);
    hashmap->put(4, 8);
    hashmap->put(0, 1);
    hashmap->put(1, 1);
    hashmap->put(1, 2);
    hashmap->put(1, 4);

    ASSERT_TRUE(hashmap->has(0));
    ASSERT_TRUE(hashmap->has(1));
    ASSERT_TRUE(hashmap->has(2));
    ASSERT_TRUE(hashmap->has(3));
    ASSERT_TRUE(hashmap->has(4));
    ASSERT_FALSE(hashmap->has(5));
    ASSERT_FALSE(hashmap->has(6));

    delete hashmap;
}

TEST_F(Test, testGet) {
    auto hashmap = new Hashmap<int32_t, int32_t>(10);

    hashmap->put(1, 9);
    hashmap->put(2, 9);
    hashmap->put(3, 8);
    hashmap->put(4, 8);
    hashmap->put(0, 1);
    ASSERT_EQ(hashmap->get(1), 9);
    hashmap->put(1, 1);
    hashmap->put(1, 2);
    hashmap->put(1, 4);

    ASSERT_EQ(hashmap->get(0), 1);
    ASSERT_EQ(hashmap->get(1), 4);
    ASSERT_EQ(hashmap->get(2), 9);
    ASSERT_EQ(hashmap->get(3), 8);
    ASSERT_EQ(hashmap->get(4), 8);

    delete hashmap;
}

TEST_F(Test, testResize) {
    auto hashmap = new Hashmap<int32_t, int32_t>(4);

    hashmap->put(1, 4);
    hashmap->put(2, 9);
    hashmap->put(3, 8);
    hashmap->put(4, 8);

    ASSERT_EQ(hashmap->get(1), 4);
    ASSERT_EQ(hashmap->get(2), 9);
    ASSERT_EQ(hashmap->get(3), 8);
    ASSERT_EQ(hashmap->get(4), 8);

    hashmap->put(5, 7);
    hashmap->put(6, 7);
    hashmap->put(7, 6);

    ASSERT_EQ(hashmap->get(1), 4);
    ASSERT_EQ(hashmap->get(2), 9);
    ASSERT_EQ(hashmap->get(3), 8);
    ASSERT_EQ(hashmap->get(4), 8);
    ASSERT_EQ(hashmap->get(5), 7);
    ASSERT_EQ(hashmap->get(6), 7);
    ASSERT_EQ(hashmap->get(7), 6);

    delete hashmap;
}
