#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // Напишите реализацию функции,
    // не копируя сортируемые элементы
	if (range_end - range_begin < 2)  return;

	vector<typename RandomIt::value_type> elements(
			make_move_iterator(range_begin),
			make_move_iterator(range_end)
			);
	RandomIt it1 = elements.begin() + (elements.end() - elements.begin()) / 3;
	RandomIt it2 = it1 + (elements.end() - elements.begin()) / 3;
	MergeSort(elements.begin(), it1);
	MergeSort(it1, it2);
	MergeSort(it2, elements.end());

	// merge
	vector<typename RandomIt::value_type> tmp;
	merge(make_move_iterator(elements.begin()), make_move_iterator(it1),
	      make_move_iterator(it1), make_move_iterator(it2),
		  back_inserter(tmp));
	merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
		  make_move_iterator(it2), make_move_iterator(elements.end()),
		  range_begin);
}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
