#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = int;

  Id Add(T object) {
    const Id new_id = objects.size();
    objects.push_back({move(object)});
    sorted_objects.insert({0, new_id});
    return new_id;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    while (range_begin != range_end) {
      *ids_begin++ = Add(move(*range_begin++));
    }
  }

  bool IsValid(Id id) const {
    return id >= 0 && id < objects.size() &&
        objects[id].priority != NONE_PRIORITY;
  }

  const T& Get(Id id) const {
    return objects[id].data;
  }

  void Promote(Id id) {
    auto& item = objects[id];
    const int old_priority = item.priority;
    const int new_priority = ++item.priority;
    sorted_objects.erase({old_priority, id});
    sorted_objects.insert({new_priority, id});
  }

  pair<const T&, int> GetMax() const {
    const auto& item = objects[prev(sorted_objects.end())->second];
    return {item.data, item.priority};
  }

  pair<T, int> PopMax() {
    const auto sorted_objects_it = prev(sorted_objects.end());
    auto& item = objects[sorted_objects_it->second];
    sorted_objects.erase(sorted_objects_it);
    const int priority = item.priority;
    item.priority = NONE_PRIORITY;
    return {move(item.data), priority};
  }

private:
  struct ObjectItem {
    T data;
    int priority = 0;
  };
  static const int NONE_PRIORITY = -1;

  vector<ObjectItem> objects;
  set<pair<int, Id>> sorted_objects;
};

class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
