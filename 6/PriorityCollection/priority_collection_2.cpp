#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = size_t; /* тип, используемый для идентификаторов */

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
    	id_element.emplace(make_pair(last_id, move(object)));
    	id_priority[last_id] = 0;
    	priority_id.insert(make_pair(0, last_id));
    	last_id++;
    	return last_id - 1;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
    	for (auto it = range_begin; it != range_end; ++it) {
    		id_element.emplace(make_pair(last_id, move(*it)));
    		id_priority[last_id] = 0;
    		priority_id.insert(make_pair(0, last_id));
    		*(ids_begin++) = last_id;
    		last_id++;
    	}
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
    	return static_cast<bool>(id_element.count(id));
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
    	return id_element.at(id);
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
    	priority_id.erase({id_priority[id]++, id});
    	priority_id.insert({id_priority[id], id});


//    	auto tmp = priority_id.extract({id_priority[id]++, id});
//    	tmp.value().first++;
//    	priority_id.insert(move(tmp));
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
    	Id id = priority_id.rbegin()->second;
    	return {id_element.at(id), id_priority.at(id)};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
    	auto tmp = make_move_iterator(priority_id.rbegin());
    	int pr = move(tmp->first);
    	Id id = move(tmp->second);
    	pair<T, int> res = make_pair(move(id_element[id]), move(pr));
    	id_element.erase(id);
    	id_priority.erase(id);
    	priority_id.erase(*priority_id.rbegin());
    	return res;
    }

private:
    // Приватные поля и методы
    Id last_id = 0;
    unordered_map<Id, T> id_element;
    unordered_map<Id, int> id_priority;
    set<pair<int, Id>> priority_id;
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
