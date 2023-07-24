#include <iostream>
#include <vector>
#include <future>

#include "profile.h"

using namespace std;


// Paginator

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

template <typename Container>
int64_t SumSingleThread (const Container& matrix) {
	int64_t sum = 0;
	for (const auto& row : matrix) {
		for (const auto& item : row) {
			sum += item;
		}
	}
	return sum;
}

template <typename Container>
void GenerateSingleThread(Container& result, size_t first_row, size_t column_size) {
	for (auto& row : result) {
		row.reserve(column_size);
		for (size_t column = 0; column < column_size; ++column) {
			row.push_back(first_row ^ column);
		}
		++first_row;
	}
}

// генерация матрицы
vector<vector<int>> GenerateSingleThread(size_t size) {
	vector<vector<int>> result(size);
	GenerateSingleThread(result, 0, size);
	return result;
}

vector<vector<int>> GenerateMultiThread(size_t size, size_t page_size) {
	vector<vector<int>> result(size);
	vector<future<void>> futures;
	size_t first_row = 0;

	for (auto page : Paginate(result, page_size)) {
		futures.push_back( async(
			[size, page, first_row](){
				GenerateSingleThread(page, first_row, size);
			})
		);
		first_row += page_size;
	}

	return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // Реализуйте эту функцию
	vector<future<int64_t>> futures;

	for (const auto& page : Paginate(matrix, 2000)) {
		futures.push_back(
			async([=](){
				return SumSingleThread(page);
			}
		));
	}
	int64_t sum = 0;
	for (auto& item : futures) {
		sum += item.get();
	}

	return sum;
}


int main() {
	LOG_DURATION("Total");

	const size_t matrix_size = 7000;
	const size_t page_size = 2000;
	vector<vector<int>> matrix;

	{
		LOG_DURATION("Single Thread Generation");
		matrix = GenerateSingleThread(matrix_size); // 195928050144
	}

	{
		LOG_DURATION("Multi Thread Generation");
		matrix = GenerateMultiThread(matrix_size, page_size); // 195928050144
	}

	{
		LOG_DURATION("Single thread sum");
		cout << SumSingleThread(matrix) << endl;
	}

	{
		LOG_DURATION("Multi thread sum");
		cout << CalculateMatrixSum(matrix) << endl;
	}

	return 0;
}
