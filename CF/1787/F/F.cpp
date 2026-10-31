#include <bits/stdc++.h>

int powmod(int a, int e, int m) {
	int r = 1;
	while (e) {
		if (e & 1) r = int(int64_t(r) * int64_t(a) % m);
		a = int(int64_t(a) * int64_t(a) % m);
		e >>= 1;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		/*
		cerr << "start" << '\n';
		for (int v : A) { cerr << v << ' '; } cerr << '\n';
		cerr << "K = " << K << '\n';
		*/

		auto solve = [&]() -> std::optional<std::vector<int>> {
			std::vector<std::vector<std::vector<int>>> cycles(N+1);
			std::vector<bool> vis(N);
			for (int i = 0; i < N; i++) {
				if (vis[i]) continue;
				std::vector<int> cyc;
				int cur = i;
				while (!vis[cur]) {
					vis[cur] = true;
					cyc.push_back(cur);
					cur = A[cur];
				}
				//cerr << "cyc "; for (int v : cyc) { cerr << v << ' '; } cerr << '\n';
				cycles[int(cyc.size())].push_back(std::move(cyc));
			}
			std::vector<int> res(N, -1);
			std::vector<int> cur_cyc; cur_cyc.reserve(N);
			for (int l = 1; l <= N; l++) {
				if (cycles[l].empty()) continue;
				int cnt = int(cycles[l].size());
				while (cnt) {
					int b = std::min(K, __builtin_ctz(cnt));
					cnt -= (1 << b);
					if (l % 2 == 0 && b != K) return std::nullopt;

					cur_cyc.clear();
					int mult = l % 2 == 0 ? 1 : powmod((l+1)/2, K-b, l);
					for (int i = 0; i < l; i++) {
						int pos = int(int64_t(i) *int64_t(mult) % l);
						for (int j = 0; j < (1 << b); j++) {
							cur_cyc.push_back(cycles[l][cnt+j][pos]);
						}
					}
					//cerr << "res_cyc "; for (int v : cur_cyc) { cerr << v << ' '; } cerr << '\n';
					for (int i = 0; i < int(cur_cyc.size()); i++) {
						res[cur_cyc[i]] = cur_cyc[(i+1)%int(cur_cyc.size())];
					}
				}
			}
			return res;
		};

		auto ans = solve();
		if (ans) {
			cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				cout << (*ans)[i] + 1 << " \n"[i+1==N];
			}

			//for (int v : *ans) { cerr << v << ' '; } cerr << '\n';
			if (false && K <= 1000) {
				std::vector<int> cur = *ans;
				for (int z = 0; z < K; z++) {
					std::vector<int> nxt(N);
					for (int i = 0; i < N; i++) nxt[i] = cur[cur[i]];
					cur = nxt;
				}
				assert(cur == A);
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
