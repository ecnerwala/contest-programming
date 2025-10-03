#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Z; cin >> N >> Z;
		cerr << "start case " << N << ' ' << Z << '\n';
		std::vector<int> X(N); for (auto& x : X) cin >> x;
		int Q; cin >> Q;
		std::vector<std::array<int, 2>> queries(Q);
		for (auto& [l, r] : queries) { cin >> l >> r; l--; }

		std::vector<int> depth(N);
		std::vector<int> jmp(N);
		for (int i = N-1, j = N; i >= 0; i--) {
			while (X[j-1] - X[i] > Z) j--;
			assert(j > i);
			jmp[i] = j;
			depth[i] = (j == N ? 0 : depth[j]) + 1;
		}

		std::vector<int> ans(Q);
		std::vector<std::set<std::pair<int, int>>> alive(N);
		for (int q = 0; q < Q; q++) {
			auto [l, r] = queries[q];
			if (r - l == 1) {
				ans[q] = 1;
			} else {
				assert(r - l >= 2);
				ans[q] += depth[l];
				alive[l].insert({r, q});
				ans[q] += depth[l+1];
				alive[l+1].insert({r, q});
			}
		}
		for (int i = 0; i < N; i++) {
			int j = jmp[i];
			while (!alive[i].empty() && alive[i].begin()->first <= j) {
				auto it = alive[i].begin();
				int q = it->second;
				alive[i].erase(it);
				ans[q] -= depth[i]-1;
			}
			// now, merge the two
			if (alive[i].empty()) continue;
			assert(j < N);
			if (alive[i].size() > alive[j].size()) std::swap(alive[i], alive[j]);
			for (auto v : alive[i]) {
				auto [r, q] = v;
				if (alive[j].insert(v).second) {
					// did insert, so just continue
				} else {
					cerr << "collision " << i << '-' << j << ' ' << r << ' ' << q << '\n';
					// insert into j+1
					if (j+1 >= r) {
						ans[q] -= depth[i]-1;
					} else {
						cerr << "bumping" << '\n';
						ans[q] -= depth[j];
						ans[q] += depth[j+1];
						alive[j+1].insert(v);
					}
				}
			}
			alive[i] = {};
		}
		for (int q = 0; q < Q; q++) {
			cout << ans[q] << '\n';
		}
	}

	return 0;
}
