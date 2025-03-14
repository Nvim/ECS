#include "ecs.hpp"
#include <gtest/gtest.h>

using f32 = float;

TEST(ecs, init) {
  ECS ecs{};
  EXPECT_EQ(0, 0);
}
