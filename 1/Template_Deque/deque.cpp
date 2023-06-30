#include <vector>
#include <stdexcept>

//#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Deque
template<typename T>
class Deque {
public:
	Deque() {
	}

	bool Empty() const {
		return forward.empty() && backward.empty();
	}

	size_t Size() const {
		return forward.size() + backward.size();
	}

	void PushFront(const T& value) {
		backward.push_back(value);
	}

	void PushBack(const T& value) {
		forward.push_back(value);
	}

	const T& operator[](size_t index) const {
		if (index < backward.size())
			return backward[backward.size() - 1 - index];
		else
			return forward[index - backward.size()];
	}

	const T& At(size_t index) const {
		if (index >= this->Size())
			throw std::out_of_range("out of range");
		return operator[](index);
	}

	const T& Front() const {
		return backward.back();
	}

	const T& Back() const {
		return forward.back();
	}

	T& operator[](size_t index) {
		if (index < backward.size())
			return backward[backward.size() - 1 - index];
		else
			return forward[index - backward.size()];
	}

	T& At(size_t index) {
		if (index >= this->Size())
			throw std::out_of_range("out of range");
		return operator[](index);
	}

	T& Front() {
		if(!backward.empty())
			return backward.back();
		return forward.at(0);
	}

	T& Back() {
		if (!forward.empty())
			return forward.back();
		return backward.at(0);
	}

private:
	vector<T> forward;
	vector<T> backward;
};

/*
void TestDeque() {
	Deque<int> d;
	ASSERT_EQUAL(d.Size(), 0u);

	d.PushBack(1);
	d.PushBack(2);
	ASSERT_EQUAL(d.Size(), 2u);
	ASSERT_EQUAL(d.At(0), 1);
	ASSERT_EQUAL(d.At(1), 2);
	ASSERT_EQUAL(d.Front(), 1);
	ASSERT_EQUAL(d.Back(), 2);

	d.PushFront(-1);
	d.PushFront(-2);
	ASSERT_EQUAL(d.Size(), 4u);
	ASSERT_EQUAL(d.At(0), -2);
	ASSERT_EQUAL(d.At(1), -1);
	ASSERT_EQUAL(d.At(2), 1);
	ASSERT_EQUAL(d.At(3), 2);

	ASSERT_EQUAL(d.Front(), -2);
	ASSERT_EQUAL(d.Back(), 2);
}*/

int main() {
	//TestRunner tr;
	//RUN_TEST(tr, TestDeque);
  return 0;
}
