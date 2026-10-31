#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> A(N);
	for (auto& a : A) cin >> a, a--;

	vector<vector<int>> inds(N);
	for (int i = 0; i < N; i++) {
		inds[A[i]].push_back(i);
	}

	int ans = 0;
	for (int v = 0; v < N; v++) {
		if (inds[v].empty()) continue;
		ans++;
		int num_ending = 1;
		for (int z = 1; z < int(inds[v].size()); z++) {
			num_ending = max(0, num_ending - (inds[v][z] - inds[v][z-1] - 1));
			if (num_ending < 2) {
				ans++;
				num_ending++;
			}
		}
	}
	cout << ans << '\n';

	return 0;
}

// In this case:
// 1 1 2 1 1 -> 1 2 1 and 1 1
// a b   a b
//  2 can only separate one of the segments
// 1 1 2 3 1 1 -> 1 2 1 and 1 3 1 (any greedy that does 1 2 3 1 is bad)
//
// DP: after processing A[1..k] either the 2 sequences end in A[k] and A[k-1]
//  or they end in A[k] and something else (doesn't matter)
