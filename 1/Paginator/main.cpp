#include <iostream>
#include <vector>

using namespace std;

template <typename Iterator>
struct IteratorRange {
	Iterator first, last;

	Iterator begin() {
		return first;
	}

	Iterator end() {
		return last;
	}
};

template <typename T>
IteratorRange<typename vector<T>::iterator> Head(vector<T>& v, size_t top) {
	return {v.begin(), next(v.begin(), min(top, v.size()))};
}

template <typename T>
IteratorRange<T> MakeRange(T begin, T end) {
	return IteratorRange<T>{begin, end};
}

int main() {
	vector<int> v = {1, 2, 3, 4, 5};
	for (int& x : Head(v, 3)) {
		++x;
	}

	for(int x : v) {
		cout << x << ' ';
	}
	cout << endl;

	auto second_half = MakeRange(v.begin() + v.size() / 2, v.end());

	for (int x : second_half) {
		cout << x << ' ';
	}

	return 0;
}
