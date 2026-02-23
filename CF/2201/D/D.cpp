#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<std::set<int>> locs(N);
		std::map<int, int> vals;
		for (int i = 0; i < N; i++) { locs[A[i]].insert(i); }
		for (int a = 0; a < N; a++) {
			if (int(locs[a].size()) >= 2) {
				int i = *locs[a].begin();
				int j = *locs[a].rbegin();
				vals[j-i] ++;
			}
		}
		for (int q = 0; q < Q; q++) {
			int x; cin >> x; x--;
			{
				int a = A[x];
				if (int(locs[a].size()) >= 2) {
					int i = *locs[a].begin();
					int j = *locs[a].rbegin();
					auto it = vals.find(j-i);
					if (!--it->second) { vals.erase(it); }
				}
				locs[a].erase(x);
				if (int(locs[a].size()) >= 2) {
					int i = *locs[a].begin();
					int j = *locs[a].rbegin();
					vals[j-i] ++;
				}
			}
			cin >> A[x]; A[x]--;
			{
				int a = A[x];
				if (int(locs[a].size()) >= 2) {
					int i = *locs[a].begin();
					int j = *locs[a].rbegin();
					auto it = vals.find(j-i);
					if (!--it->second) { vals.erase(it); }
				}
				locs[a].insert(x);
				if (int(locs[a].size()) >= 2) {
					int i = *locs[a].begin();
					int j = *locs[a].rbegin();
					vals[j-i] ++;
				}
			}
			if (vals.empty()) {
				cout << 0 << ' ' << 0 << '\n';
			} else {
				auto [k, f] = *vals.rbegin();
				cout << k << ' ' << f << '\n';
			}
		}
	}

	return 0;
}
