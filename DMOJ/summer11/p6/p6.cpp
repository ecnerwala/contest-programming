#include "trilib.h"

#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N = get_n();
	assert(N >= 3);
	vector<int> above; above.reserve(N);
	vector<int> below; below.reserve(N);
	for (int i = 3; i <= N; i++) {
		if (is_clockwise(1, 2, i)) {
			above.push_back(i);
		} else {
			below.push_back(i);
		}
	}
	sort(above.begin(), above.end(), [&](int i, int j) {
		return is_clockwise(1, i, j);
	});
	sort(below.begin(), below.end(), [&](int i, int j) {
		return is_clockwise(1, i, j);
	});

	vector<int> cnds; cnds.reserve(N);
	cnds.push_back(2);
	cnds.insert(cnds.end(), above.begin(), above.end());
	cnds.push_back(1);
	cnds.insert(cnds.end(), below.begin(), below.end());

	vector<int> hull(N, -1);
	int lo = 0, hi = 0;
	for (int i : cnds) {
		while (hi >= 2 && !is_clockwise(hull[hi-2], hull[hi-1], i)) {
			--hi;
		}
		hull[hi++] = i;
	}
	while (hi - lo >= 3) {
		if (!is_clockwise(hull[hi-1], hull[lo], hull[lo+1])) {
			++lo;
			continue;
		}
		if (!is_clockwise(hull[hi-2], hull[hi-1], hull[lo])) {
			--hi;
			continue;
		}
		break;
	}
	give_answer(hi - lo);

	return 0;
}
