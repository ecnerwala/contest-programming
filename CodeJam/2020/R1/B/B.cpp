#include<bits/stdc++.h>
using namespace std;

vector<pair<int, int>> solve(int N) {
	vector<pair<int, int>> ans;
	if (N <= 30) {
		for (int i = 0; i < N; i++) {
			ans.emplace_back(i, 0);
		}
	} else {
		N -= 30;
		int numExtra = 0;
		bool high = false;
		for (int i = 0; i < 30; i++) {
			if (N & (1 << i)) {
				if (high) {
					for (int z = i; z >= 0; z--) {
						ans.emplace_back(i, z);
					}
				} else {
					for (int z = 0; z <= i; z++) {
						ans.emplace_back(i, z);
					}
				}
				numExtra++;
				high = !high;
			} else {
				if (high) {
					ans.emplace_back(i, i);
				} else {
					ans.emplace_back(i, 0);
				}
			}
		}
		for (int i = 30; numExtra; i++) {
			if (high) {
				ans.emplace_back(i, i);
			} else {
				ans.emplace_back(i, 0);
			}
			numExtra--;
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		auto ans = solve(N);
		cout << "Case #" << case_num << ":" << '\n';
		for (auto it : ans) {
			cout << it.first+1 << ' ' << it.second+1 << '\n';
		}
	}

	return 0;
}
