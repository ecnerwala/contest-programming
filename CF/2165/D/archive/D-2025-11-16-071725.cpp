#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> idx(2*N+1);
		for (int i = 0; i < N; i++) { int a; cin >> a; idx[a].push_back(i); }
		// just greedy from the top
		int ans = N;
		for (int iter = 0; iter < 2; iter++) {
			std::vector<int> cur; cur.reserve(N);
			std::vector<int> nxt; nxt.reserve(N);
			for (int v = 0; v < int(idx.size()); v++) {
				auto& cnd = idx[v];
				if ((v & 1) == iter) {
					std::reverse(cnd.begin(), cnd.end());
					for (auto& a : cnd) a = N-1-a;
				}
				std::reverse(cur.begin(), cur.end());
				for (auto& a : cur) a = N-1-a;
				int z = 0;
				int stk = 0;
				for (int a : cnd) {
					while (z < int(cur.size()) && cur[z] < a) { z++, stk++; }
					if (stk > 0) {
						stk--;
						ans--;
					} else {
						nxt.push_back(a);
					}
				}
				cur.clear();
				std::swap(cur, nxt);

				if ((v & 1) == iter) {
					std::reverse(cnd.begin(), cnd.end());
					for (auto& a : cnd) a = N-1-a;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
