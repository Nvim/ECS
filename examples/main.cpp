#include "ecs.hpp"
#include "sparse.hpp"
#include <cassert>
#include <iostream>

using f32 = float;

void TestSparse() {
  SparseSet<f32> s{};

  assert(s.Count() == 0);

  s.Add(42, 1.0f);
  assert(!s.Has(2));
  assert(s.Has(42));

  f32 a = s.Get(42);
  assert(a == 1.0f);

  s.Add(128, 888.0f);
  assert(s.Count() == 2);
  assert(s.Has(128));
  assert(s.Get(128) == 888.0f);

  s.Remove(42);
  assert(!s.Has(42));

  s.Remove(128);
  assert(s.Count() == 0);
}

struct Position {
  float x, y;
};

struct Transform {
  float scale;
  float rotation;
  float translation;
};

void TestECS() {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();

  Position pos{10.0f, 42.0f};
  Position pos2{11.0f, 64.9f};
  Transform tr{0.2f, 10.4f, 33.0f};

  ecs.AddComponent(e1, pos);
  ecs.AddComponent(e1, tr);
  ecs.AddComponent(e2, pos2);
  auto e1pos = ecs.GetComponent<Position>(e1);
  auto e2pos = ecs.GetComponent<Position>(e2);
  auto e1tr = ecs.GetComponent<Transform>(e1);
  assert(e1pos.x == pos.x);
  assert(e2pos.y == pos2.y);
  assert(e1tr.rotation == tr.rotation);

  auto query = ecs.Query<Position, Transform>();
  assert(query.size() == 1);
  assert(query.at(0) == e1);
}

void debugme() {

  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();
  auto e3 = ecs.CreateEntity();

  ecs.AddComponent(e1, Position{20.0f, 4.0f});
  ecs.AddComponent(e2, Position{22.0f, 6.0f});
  ecs.AddComponent(e3, Position{22.0f, 6.0f});

  ecs.AddComponent(e1, Transform{20.0f, 4.0f, 3.0f});
  ecs.AddComponent(e2, Transform{22.0f, 6.0f, 8.8f});
  ecs.AddComponent(e3, Transform{22.0f, 6.0f, 67.3f});

  auto result = ecs.Query<Position, Transform>();
  assert(result.size() == 3);

  ecs.DestroyEntity(e1);
  auto result2 = ecs.Query<Position, Transform>();

  assert(ecs.Count() == 2);
  assert(result2.size() == 2);
}

int main() {
  std::cout << "Sparse Test\n";
  TestSparse();

  std::cout << "ECS Test\n";
  TestECS();

  std::cout << "Oui test\n";
  debugme();

  return 0;
}
