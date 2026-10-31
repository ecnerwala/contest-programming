#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ValueDivision {

public:

	vector <int> getArray(vector <int> A) {
		int N = int(A.size());
		vector<pair<int, int>> vals(N);
		for (int i = 0; i < N; i++) {
			vals[i] = {A[i], i};
		}
		sort(vals.begin(), vals.end());
		reverse(vals.begin(), vals.end());

		priority_queue<int, vector<int>, less<int>> pq;
		int v = *max_element(A.begin(), A.end())+1;
		vector<int> ans(N);
		for (auto it : vals) {
			while (!pq.empty() && it.first < v) {
				ans[pq.top()] = max(1, v); pq.pop();
				v--;
			}
			assert(v >= it.first);
			v = it.first;
			pq.push(it.second);
		}
		while (!pq.empty()) {
			ans[pq.top()] = max(1, v); pq.pop();
			v--;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ValueDivision;

// vim:ft=cpp
