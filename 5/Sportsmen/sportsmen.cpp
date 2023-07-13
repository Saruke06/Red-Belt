#include "test_runner.h"
#include <iostream>
#include <list>
#include <deque>
#include <algorithm>

using namespace std;


int main() {
	int n;
	cin >> n;
/*
	list<int> sportsmens;

	for (int i = 0; i < n; ++i) {
		int num, num_to_stay_before;
		cin >> num >> num_to_stay_before;
		list<int> to_insert;
		to_insert.insert(to_insert.begin(), num);
		auto it = find(begin(sportsmens), end(sportsmens), num_to_stay_before);
		sportsmens.splice(it, to_insert);
	}
*/
	deque<int> sportsmens;

	for (int i = 0; i < n; ++i) {
		int num, num_to_stay_before;
		cin >> num >> num_to_stay_before;
		auto it = find(begin(sportsmens), end(sportsmens), num_to_stay_before);
		sportsmens.insert(it, num);
	}

	for(auto elem : sportsmens) {
		cout << elem << ' ';
	}

	return 0;
}
