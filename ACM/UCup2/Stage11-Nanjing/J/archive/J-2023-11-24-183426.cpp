#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_CASES; cin >> N_CASES;
	while (N_CASES--) {
		int N, M; cin >> N >> M;
		// P are in dfs order
		const int V = N+2; // 0, 1..N, N+1
		N++;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) {
			cin >> P[i];
		}
		std::vector<int> C(N);
		C[0] = 0;
		for (int i = 1; i < N; i++) {
			cin >> C[i];
		}

		std::vector<int> T(M);
		for (auto& t : T) {
			 cin >> t;
		}

		std::vector<int> jmp = P;
		std::vector<std::array<int, 2>> suff_value(N);
		suff_value[0] = {0, V-1};
		for (int i = 1; i < N; i++) {
			suff_value[i] = {C[i], C[i]};
		}

		int cur_V = V;

		struct suff_t {
			int i;
			bool d;
		};
		std::vector<suff_t> suffs(2*N);
		{
			std::vector<int> cnt(V);
			for (int i = 0; i < N; i++) {
				for (int d = 0; d < 2; d++) {
					cnt[suff_value[i][d]]++;
				}
			}
			for (int i = 1; i < int(cnt.size()); i++) cnt[i] += cnt[i-1];
			for (int i = 0; i < N; i++) {
				for (int d = 0; d < 2; d++) {
					suffs[--cnt[suff_value[i][d]]] = suff_t{i, bool(d)};
				}
			}
		}

		while ([&]() -> bool {
			for (int i = 0; i < N; i++) {
				if (jmp[i] != -1) return true;
			}
			return false;
		}()) {
			std::vector<int> cnt(cur_V);
			std::vector<std::array<int, 2>> par_suff_value(N);
			for (auto [i, d] : suffs) {
				par_suff_value[i][d] = suff_value[(jmp[i] == -1) ? 0 : jmp[i]][d];
				cnt[par_suff_value[i][d]]++;
			}
			for (int i = 1; i < int(cnt.size()); i++) cnt[i] += cnt[i-1];
			std::vector<suff_t> nsuffs(2*N);
			for (auto [i, d] : suffs) {
				nsuffs[--cnt[par_suff_value[i][d]]] = suff_t{i, d};
			}
			for (int i = 0; i < int(cnt.size()); i++) cnt[i] = 0;
			for (auto [i, d] : nsuffs) {
				cnt[suff_value[i][d]]++;
			}
			for (int i = 1; i < int(cnt.size()); i++) cnt[i] += cnt[i-1];
			std::reverse(nsuffs.begin(), nsuffs.end());
			for (auto [i, d] : nsuffs) {
				suffs[--cnt[suff_value[i][d]]] = suff_t{i, d};
			}

			for (int z = 0; z+1 < int(suffs.size()); z++) {
				auto [i0, d0] = suffs[z];
				auto [i1, d1] = suffs[z+1];
				assert(
					std::make_pair(
						suff_value[i0][d0],
						par_suff_value[i0][d0]
					) <=
					std::make_pair(
						suff_value[i1][d1],
						par_suff_value[i1][d1]
					)
				);
			}
			std::vector<std::array<int, 2>> n_suff_value(N);
			cur_V = 0;
			std::pair<int, int> prev_value;
			for (int z = 0; z < int(suffs.size()); z++) {
				auto [i, d] = suffs[z];
				auto cur_value = std::make_pair(suff_value[i][d], par_suff_value[i][d]);
				if (z == 0) {
					cur_V++;
				} else {
					assert(cur_value >= prev_value);
					if (cur_value > prev_value) cur_V++;
				}
				n_suff_value[i][d] = cur_V - 1;
				prev_value = cur_value;
			}
			std::vector<int> n_jmp(N);
			for (int i = 0; i < N; i++) {
				n_jmp[i] = (jmp[i] == -1 ? -1 : jmp[jmp[i]]);
			}
			jmp = std::move(n_jmp);
			suff_value = std::move(n_suff_value);
		}
		std::vector<int> failure(N);
		std::vector<int> stk; stk.reserve(N);
		for (auto [i, d] : suffs) {
			cerr << i << ' ' << d << '\n';
			if (d == 0) {
				if (i == 0) {
					assert(stk.empty());
					failure[i] = -1;
				} else {
					assert(!stk.empty());
					failure[i] = stk.back();
				}
				stk.push_back(i);
			} else if (d == 1) {
				assert(stk.back() == i);
				stk.pop_back();
			} else assert(false);
		}
	}

	return 0;
}
