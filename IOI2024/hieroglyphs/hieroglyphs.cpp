#include "hieroglyphs.h"
#include <bits/stdc++.h>

std::vector<int> ucs(std::vector<int> _A, std::vector<int> _B) {
	std::array<std::vector<int>, 2> S{std::move(_A), std::move(_B)};

	int V = 0;
	for (int z = 0; z < 2; z++) {
		V = std::max(V, 1 + *std::max_element(S[z].begin(), S[z].end()));
	}

	std::array<std::vector<int>, 2> matches;
	for (int z = 0; z < 2; z++) {
		matches[z] = std::vector<int>(S[z].size(), -1);
	}

	int num_matches = 0;

	// First, let's go make some linked lists
	for (int z = 0; z < 2; z++) {
		int M = int(S[!z].size());
		std::vector<int> prv_y(M, -1);
		std::vector<int> nxt_y(M, -1);
		std::vector<int> first_occ_y(V, M);
		std::vector<int> last_occ_y(V, -1);
		std::vector<int> cnt_y(V, 0);
		for (int j = M-1; j >= 0; j--) {
			int v = S[!z][j];
			cnt_y[v]++;
			nxt_y[j] = std::exchange(first_occ_y[v], j);
			if (nxt_y[j] != M) prv_y[nxt_y[j]] = j;
			else last_occ_y[v] = j;
		}

		int N = int(S[z].size());
		std::vector<int> prv_x(N, -1);
		std::vector<int> last_occ_x(V, -1);
		std::vector<int> cnt_x(V, 0);
		std::vector<int> first_match(N);

		// DSU
		std::vector<int> par(N, -1);
		std::vector<int> root_val(N);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> int {
			assert(a != b);
			assert(par[a] < 0 && par[b] < 0);
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return a;
		};
		std::vector<std::pair<int, int>> stk; stk.reserve(N);

		for (int i = 0; i < N; i++) {
			int v = S[z][i];
			cnt_x[v]++;
			prv_x[i] = std::exchange(last_occ_x[v], i);
			int j;
			int m;
			if (prv_x[i] == -1) {
				j = -1;
				m = -1;
			} else {
				// find the min up to prv_x[i]
				j = root_val[get_par(prv_x[i])];
				m = first_match[prv_x[i]];
			}
			assert(m >= j);
			while (m > j) {
				m = (m == M) ? last_occ_y[v] : prv_y[m];
			}
			assert(m <= j);
			m = m == M ? M : m == -1 ? first_occ_y[v] : nxt_y[m];
			assert(m == M || m > j);
			first_match[i] = m;
			int cur = i;
			while (!stk.empty() && m <= stk.back().first) {
				cur = merge(cur, stk.back().second);
				stk.pop_back();
			}
			root_val[cur] = m;
			stk.push_back({m, cur});
		}

		// Now, we have all the first matches
		for (int v = 0; v < V; v++) {
			if (cnt_y[v] > 0 && std::pair<int, bool>(cnt_x[v], z) > std::pair<int, bool>(cnt_y[v], !z)) {
				// match them up
				int cur_y = last_occ_y[v];
				for (int cur_x = last_occ_x[v]; cur_y != -1; cur_x = prv_x[cur_x]) {
					assert(cur_x != -1);
					if (first_match[cur_x] <= cur_y) {
						matches[z][cur_x] = cur_y;
						matches[!z][cur_y] = cur_x;
						num_matches++;
						cur_y = prv_y[cur_y];
					}
				}
			}
		}
	}

	std::vector<int> res; res.reserve(num_matches);
	int prv = -1;
	for (int i = 0; i < int(S[0].size()); i++) {
		int v = matches[0][i];
		if (v == -1) continue;
		if (v < std::exchange(prv, v)) return {-1};
		res.push_back(S[0][i]);
	}
	return res;
}
