#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<vector<int64_t>> B(64);
	for (int i = 0; i < N; i++) {
		int64_t b; cin >> b;
		B[63 - __builtin_clzll(b)].push_back(b);
	}

	vector<int64_t> ans;
	for (int l = 63; l >= 0; l--) {
		vector<int64_t> nans; nans.reserve(ans.size() + B[l].size());
		auto b_it = B[l].begin();

		if (b_it != B[l].end()) {
			nans.push_back(*b_it);
			++b_it;
		}

		for (int64_t v : ans) {
			nans.push_back(v);
			if (b_it != B[l].end() && (v & (int64_t(1) << l))) {
				nans.push_back(*b_it);
				++b_it;
			}
		}

		if (b_it != B[l].end()) {
			cout << "No" << '\n';
			exit(0);
		}
		ans = nans;
	}

	cout << "Yes" << '\n';
	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}

// if it's increasing, that means the highest set bit of the next b_i always is 0
// we can group the b_i by the highest set bit
//
// these must form a binary tree
