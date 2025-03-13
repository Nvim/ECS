#pragma once

#include "sparse.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

enum class ComponentType : char {
  COMP_TRANSFORM,
  COMP_DIRTY,
  COMP_HIERARCHY,
  COMP_VERTICES,
};

class ECS {

private:
  u32 entity_count_{0};
  BaseSparseSet entities_;
  std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> pools_;

public:
  u32 CreateEntity() const {
    // TODO: recycle at some point
    assert(entity_count_ < kMaxId - 1);
    u32 id = NewId();
    return id;
  }

  void DestroyEntity(u32 entity) {
    // TODO: recycle at some point
    entities_.Remove(entity);
  }

  template <typename T>
  void AddComponent(u32 entity, T &component) {
    auto &p = GetOrCreatePool<T>();
    p.Add(entity, component);
  }

  template <typename T>
  void AddComponent(u32 entity, T &&component) {
    auto &p = GetOrCreatePool<T>();
    p.Add(entity, std::forward<T>(component));
  }

  template <typename T>
  bool HasComponent(u32 entity) {
    return entities_.Has(entity);
  }

  template <typename T>
  void RemoveComponent(u32 entity) {
    assert(entities_.Has(entity));
    entities_.Remove(entity);
  }

  template <typename T>
  T &GetComponent(u32 entity) {
    assert(pools_.find(std::type_index(typeid(T))) != pools_.end());
    auto *pool = static_cast<SparseSet<T> *>(
        pools_.at(std::type_index(typeid(T))).get());

    return pool->Get(entity);
  }

  template <typename... Ts>
  void Query(std::vector<u32> &result) {
    u32 min_comps = kMaxId;
    u32 min_idx{0};
    std::vector<ISparseSet *> required_pools;

    // Find smallest pool to iterate:
    for (auto &t :
         std::vector<std::type_index>{std::type_index(typeid(Ts))...}) {
      std::cout << "Typeid " << t.name() << " was requested.\n";
      auto p = pools_.find(t);
      if (p == pools_.end()) {
        std::cout << "No pool for type :(\n";
        return;
      }

      required_pools.push_back(p->second.get());
      if (p->second->Count() < min_comps) {
        min_comps = p->second->Count();
        min_idx = required_pools.size() - 1;
      }
    }

    // Iterate smallest pool and add ID to result if it's in all others
    ISparseSet *small_pool = required_pools.at(min_idx);
    for (const auto &id : small_pool->GetAllIds()) {
      bool good = true;
      for (auto *p : required_pools) {
        if (!p->Has(id)) {
          good = false;
          break;
        }
      }
      if (good) {
        result.push_back(id);
      }
    }
  }

private:
  static u32 NewId() {
    static u32 s_id = 0;
    return ++s_id;
  }
  template <typename T>
  void AddPool() {
    assert(pools_.find(std::type_index(typeid(T))) == pools_.end());
    std::unique_ptr<ISparseSet> s =
        std::unique_ptr<ISparseSet>(new SparseSet<T>);
    // ISparseSet* s = new SparseSet<T>;
    pools_[std::type_index(typeid(T))] = std::move(s);
  }

  template <typename T>
  SparseSet<T> &GetOrCreatePool() {
    if (pools_.find(std::type_index(typeid(T))) == pools_.end()) {
      AddPool<T>();
    }
    SparseSet<T> *s = static_cast<SparseSet<T> *>(
        pools_.at(std::type_index(typeid(T))).get());
    return *s;
  }
};
