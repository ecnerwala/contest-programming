#include <bits/stdc++.h>

bool solve(std::vector<int> A) {
	int N = int(A.size());
	assert(N >= 1);
	int num_odd = 0;
	int num_one = 0;
	std::vector<int> evens; evens.reserve(N);
	std::vector<int> odds; odds.reserve(N);
	for (int a : A) {
		if (a & 1) odds.push_back(a);
		else evens.push_back(a);
	}
	std::sort(odds.begin(), odds.end());
	int cur_sum = 0;
	assert(int(odds.size()) % 2 == 0);
	for (int v : odds) {
		if (v > cur_sum + 1) {
			// we just take an odd number of (cur_sum + 2), and then we're guaranteed to not hit 0
			// If cur_sum is even and v > cur_sum + 1, then v >= cur_sum + 3, so we'll win on the next step
			return true;
		} else {
			cur_sum++;
		}
	}
	// Now the odd ones can form anything in +- cur_sum (which is even)
	for (int v : evens) {
		if (v > cur_sum + 1) {
			return true;
		}
	}
	return false;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		bool ans = solve(A);
		cout << (ans ? "Yes" : "No") << '\n';
	}

	return 0;
}
