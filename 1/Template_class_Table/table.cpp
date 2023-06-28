#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table {
public:
	Table(size_t rows, size_t cols) {
		vector<vector<T>> res(rows);
		for (size_t i = 0; i < rows; ++i) {
			res[i].resize(cols);
		}
		table = res;
	}

	const vector<T>& operator[](size_t i) const {
		return table[i];
	}

	vector<T>& operator[](size_t i) {
		return table[i];
	}

	void Resize(size_t new_r, size_t new_c) {
		table.resize(new_r);
		for (size_t i = 0; i < new_r; ++i) {
			table[i].resize(new_c);
		}
	}

	pair<size_t, size_t> Size() const {
		if (!table.empty() && !table[0].empty())
			return {table.size(), table.at(0). size()};
		return {0, 0};
	}

private:
	vector<vector<T>> table;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
