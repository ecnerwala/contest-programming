#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<pair<int, int>> incr[2];
	int totTot = 0;
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		int lo = 0;
		int tot = 0;
		for (char c : s) {
			if (c == ')') tot--;
			else if (c == '(') tot++;
			else assert(false);
			lo = min(lo, tot);
		}
		totTot += tot;
		if (tot >= 0) {
			incr[0].push_back({-lo, tot});
		} else {
			incr[1].push_back({tot - lo, -tot});
		}
	}

	if (totTot) {
		cout << "No" << '\n';
		exit(0);
	}

	for (int z = 0; z < 2; z++) {
		sort(incr[z].begin(), incr[z].end());
		int cur = 0;
		for (auto it : incr[z]) {
			if (cur - it.first < 0) {
				cout << "No" << '\n';
				exit(0);
			}
			cur += it.second;
		}
	}
	cout << "Yes" << '\n';

	return 0;
}
