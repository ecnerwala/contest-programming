#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto solve = [&](const std::vector<int>& P, const std::vector<int>& Q) -> std::vector<int> {
		int N = int(P.size());
		assert(N == int(Q.size()));
		std::vector<int> invP(N, -1);
		for (int i = 0; i < N; i++) invP[P[i]] = i;

		assert(Q >= P);

		std::vector<int> ans; ans.reserve(N);

		bool is_match_Q = true;
		bool finished_pref = false;
		struct cond_t {
			int max_P_idx; // max_P_idx will never be <= max_val, so whatever
			int max_val;
		};

		std::vector<int> pref_mins(N+1);
		pref_mins[0] = N;
		for (int i = 0; i < N; i++) pref_mins[i+1] = std::min(pref_mins[i], invP[i]);

		std::vector<cond_t> conds; conds.reserve(N);
		for (int i = 0; i < N; i++) {
			auto try_extend = [&](int v) -> bool {
				if (!finished_pref && conds.empty() && v == P[i]) {
					// just in the matching prefix, do nothing
					return true;
				}

				bool n_finished_pref = finished_pref;
				if (!finished_pref && !conds.empty() && v < Q[i-1] && invP[v] < invP[Q[i-1]]) {
					// we're now finished
					n_finished_pref = true;
				}

				if (!n_finished_pref) {
					// if i == N-1, we should've hit the finished_pref condition to satisfy the condition at Q[i-1]
					assert(i < N-1);

					int cap = v - 1;
					if (is_match_Q) {
						assert(v == Q[i]);
						cap = std::min(cap, Q[i+1]);
					}

					if (!conds.empty() && invP[v] < conds.back().max_P_idx) {
						cap = std::min(cap, conds.back().max_val);
					}

					// we need there to still exist something <= invP[v] and <= cap
					if (pref_mins[cap+1] > invP[v]) return false;

					// now, update things
					while (!conds.empty() && invP[v] <= conds.back().max_P_idx && cap <= conds.back().max_val) {
						conds.pop_back();
					}

					conds.push_back({invP[v], conds.back().max_val});

					ans.push_back(v);
					return true;
				} else {
					if (!conds.empty() && v > conds.back().max_val && invP[v] < conds.back().max_P_idx) {
						if (invP[v] <= pref_mins[conds.back().max_val+1]) {
							return false;
						}
						cond_t new_cond{invP[v], conds.back().max_val};
						while (!conds.empty() && conds.back().max_P_idx >= new_cond.max_P_idx) conds.pop_back();
						conds.push_back(new_cond);

						ans.push_back(v);
						return true;
					} else {
						// just delete things we're included in
						while (!conds.empty() && v <= conds.back().max_val && invP[v] < conds.back().max_P_idx) {
							conds.pop_back();
						}

						ans.push_back(v);
						return true;
					}
					assert(false);
				}
				assert(false);
			};
			if (is_match_Q) {
				bool good = try_extend(Q[i]);
				if (good) {
					continue;
				}
				is_match_Q = false;
				int v = Q[i] - 1;
				while (!try_extend(v)) {
					v--;
				}
			} else {
				// we're already not matching Q, so we're free to be anything
				assert(finished_pref || !conds.empty());
				int min_idx;
				if (conds.empty()) {
					min_idx = 0;
				} else {
					min_idx = pref_mins[conds.back().max_val+1];
				}
				// pick the biggest unused thing after min_idx
				int v;
				bool worked = try_extend(v);
				assert(worked);
			}
		}
		return ans;
	};

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> P(N); for (auto& p : P) { std::cin >> p; p--; }
		std::vector<int> Q(N); for (auto& q : Q) { std::cin >> q; q--; }

		auto resp = solve(P, Q);
		for (int i = 0; i < N; i++) std::cout << resp[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
