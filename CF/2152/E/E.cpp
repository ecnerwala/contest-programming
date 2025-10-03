#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N == -1) exit(0);
		int L = N*N+1;
		std::vector<int> lcp(L, N);
		std::vector<int> ans; ans.reserve(N+1);
		for (int i = 0; i <= N; i++) {
			if (i == N) {
				// just look for the decreasing subsequence
				int v = int(std::find(lcp.begin(), lcp.end(), N) - lcp.begin());
				for (int j = N; j >= 0; j--) {
					while (lcp[v] != j) v--;
					assert(lcp[v] == j);
					ans.push_back(v);
				}
				std::reverse(ans.begin(), ans.end());
				break;
			}
			std::vector<int> q; q.reserve(L);
			for (int v = 0; v < L; v++) if (lcp[v] == N) q.push_back(v);
			cout << '?' << ' ' << q.size();
			for (auto v : q) cout << ' ' << v+1;
			cout << '\n' << flush;
			int cnt; cin >> cnt; if (cnt == -1) exit(0);
			for (int z = 0; z < cnt; z++) {
				int v; cin >> v;
				if (v == -1) exit(0);
				v--;
				assert(lcp[v] == N);
				lcp[v] = i;
			}
			if (cnt > N) {
				for (int v = 0; int(ans.size()) < N+1; v++) {
					if (lcp[v] == i) ans.push_back(v);
				}
				break;
			}
		}
		assert(int(ans.size()) == N+1);
		cout << '!';
		for (int v : ans) cout << ' ' << v+1;
		cout << '\n' << flush;
	}

	return 0;
}
