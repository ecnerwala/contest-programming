#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	vector<int> A(N); for (auto& a : A) { cin >> a; a--; }

	vector<vector<pair<int, int>>> queries(N);
	for (int q = 0; q < Q; q++) {
		int a, b; cin >> a >> b; a--, b--;
		queries[b].push_back({a, q});
	}

	vector<int> ans(Q);

	vector<pair<int, int>> stk; stk.reserve(N);
	vector<int> bit(N);
	for (int i = 0; i < N; i++) {
		while (!stk.empty() && stk.back().first > A[i]) stk.pop_back();
		if (!stk.empty() && stk.back().first == A[i]) {
			int j = stk.back().second; stk.pop_back();
			// Then we can save one if we include j
			for (int a = j+1; a > 0; a &= a-1) {
				bit[a-1]++;
			}
		}
		stk.push_back({A[i], i});
		for (auto [j, q] : queries[i]) {
			ans[q] = (i - j + 1);
			for (int a = j; a < i; a |= a+1) {
				ans[q] -= bit[a];
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}

// For each consecutive pair of the same color, can you paint them in the same stroke?
// Depends on the lightest thing in between
// We can precompute the set of pairs which save a stroke, and then do a plane sweep
