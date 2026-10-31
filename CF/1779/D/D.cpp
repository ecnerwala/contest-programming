#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		int M; cin >> M;
		std::vector<int> razors(M);
		for (auto& m : razors) cin >> m;
		std::sort(razors.begin(), razors.end());

		cout << ([&]() -> bool {
			std::set<std::pair<int, int>> needs;
			std::set<std::pair<int, int>> matches;
			std::vector<int> nxt(N);
			std::vector<int> prv(N);
			for (int i = 0; i < N; i++) {
				nxt[i] = i+1;
				prv[i] = i-1;
			}

			for (int i = 0; i < N; i++) {
				if (A[i] == B[i]) {
					matches.insert({B[i], i});
					A[i] = B[i] = 0;
				} else if (A[i] < B[i]) {
					return false;
				} else if (prv[i] != -1 && B[i] == B[prv[i]]) {
					if (prv[i] != -1) nxt[prv[i]] = nxt[i];
					if (nxt[i] != N) prv[nxt[i]] = prv[i];
					prv[i] = nxt[i] = i;
				} else {
					needs.insert({B[i], i});
				}
			}

			for (int m : razors) {
				while (!matches.empty() && matches.begin()->first <= m) {
					int i = matches.begin()->second;
					matches.erase(matches.begin());
					int p = prv[i];
					int n = nxt[i];
					if (p != -1) nxt[p] = n;
					if (n != N) prv[n] = p;
					nxt[i] = prv[i] = i;
					if (p != -1 && n != N && B[p] != 0 && B[n] != 0 && B[p] == B[n]) {
						// merge them
						needs.erase({B[n], n});
						if (prv[n] != -1) nxt[prv[n]] = nxt[n];
						if (nxt[n] != N) prv[nxt[n]] = prv[n];
						prv[n] = nxt[n] = n;
					}
				}
				if (!needs.empty() && needs.begin()->first < m) {
					return false;
				} else if (!needs.empty() && needs.begin()->first == m) {
					int i = needs.begin()->second;
					needs.erase(needs.begin());
					int p = prv[i];
					int n = nxt[i];
					if (p != -1) nxt[p] = n;
					if (n != N) prv[n] = p;
					nxt[i] = prv[i] = i;
					if (p != -1 && n != N && B[p] != 0 && B[n] != 0 && B[p] == B[n]) {
						// merge them
						needs.erase({B[n], n});
						if (prv[n] != -1) nxt[prv[n]] = nxt[n];
						if (nxt[n] != N) prv[nxt[n]] = prv[n];
						prv[n] = nxt[n] = n;
					}
				}
			}
			return needs.empty();
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
