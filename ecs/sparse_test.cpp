#include "sparse.hpp"
#include <gtest/gtest.h>
#include <string>

using f32 = float;

struct SomeType {
  int a;
  float b;
  std::string str;
};

TEST(trivial, init) {
  SparseSet<f32> s{};
  EXPECT_EQ(s.Count(), 0);
}

TEST(custom_type, init) {
  SparseSet<SomeType> s{};
  EXPECT_EQ(s.Count(), 0);
}

TEST(trivial, add) {
  SparseSet<f32> s{};
  s.Add(42, 1.0f);
  EXPECT_EQ(s.Count(), 1);
  EXPECT_TRUE(s.Has(42));
  EXPECT_FALSE(s.Has(41));

  s.Add(41, 44.0f);
  EXPECT_EQ(s.Count(), 2);
  EXPECT_TRUE(s.Has(41));

  // Add already present element:
  EXPECT_DEBUG_DEATH(s.Add(42, 33.0f), "failed");
}

TEST(trivial, has) {
  SparseSet<f32> s{};
  EXPECT_FALSE(s.Has(2));

  // inbounds check:
  for(int i = 0; i < kSparseBaseLen; ++i) {
    EXPECT_FALSE(s.Has(i));
  }

  // oob:
  EXPECT_FALSE(s.Has(kMaxId - 2));

  s.Add(2, 24.0f);
  EXPECT_TRUE(s.Has(2));
}

TEST(trivial, get) {
  SparseSet<unsigned long> s{};

  // Test with rvalue litteral:
  s.Add(5, 400UL);
  EXPECT_EQ(s.Get(5), 400UL);

  // Test with lvalue:
  unsigned long l = 2000UL;
  s.Add(40, l);
  EXPECT_EQ(s.Get(40), l);

  // Get invalid index:
  EXPECT_DEBUG_DEATH(s.Get(505), "failed");
}

TEST(trivial, remove_has) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_FALSE(s.Has(8));
  EXPECT_TRUE(s.Has(1));

  s.Remove(1);
  EXPECT_FALSE(s.Has(1));
}

TEST(trivial, remove_get) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_DEBUG_DEATH(s.Get(8), "failed");

  EXPECT_EQ(800, s.Get(1));
  s.Remove(1);
  EXPECT_DEBUG_DEATH(s.Get(1), "failed");
}

TEST(trivial, remove_count) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_EQ(s.Count(), 1);
  s.Remove(1);
  EXPECT_EQ(s.Count(), 0);
}
