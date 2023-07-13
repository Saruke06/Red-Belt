#include "test_runner.h"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;


int main() {
	int n;
	cin >> n;
	const int MAX_ATHLETE_NUMBER = 100'000;
	list<int> row;
	vector<list<int>::iterator> position(MAX_ATHLETE_NUMBER + 1, row.end());



	for (int i = 0; i < n; ++i) {
		int num, num_to_stay_before;
		cin >> num >> num_to_stay_before;
		position[num] = row.insert(position[num_to_stay_before], num);
	}

	for(auto elem : row) {
		cout << elem << ' ';
	}

	return 0;
}
