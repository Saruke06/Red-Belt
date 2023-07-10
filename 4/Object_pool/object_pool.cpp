#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
    	if (!free.empty()) {
    		T* free_front = free.front();
    		free.pop();
    		busy.insert(free_front);
    		return free_front;
    	} else {
    		T* newone = new T;
    		busy.insert(newone);
    		return newone;
    	}
    }

    T* TryAllocate() {
    	if (!free.empty()) {
    		T* free_front = free.front();
    		free.pop();
    		busy.insert(free_front);
    		return free_front;
    	} else {
    		return nullptr;
    	}
    }

    void Deallocate(T* object) {
    	if (busy.find(object) == busy.end()) {
    		throw invalid_argument("Invalid argument\n");
    	} else {
    		busy.erase(object);
    		free.push(object);
    	}
    }

    ~ObjectPool() {
    	for (auto a : busy)
    		delete a;
    	while(!free.empty()) {
    		T* front = free.front();
    		free.pop();
    		delete front;
    	}
    }

private:
    // Добавьте сюда поля
    set<T*> busy;
    queue<T*> free;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
