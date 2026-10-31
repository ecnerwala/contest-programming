#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto can_make_slow = [&](const std::vector<int>& P, const std::vector<int>& Q) -> std::vector<int> {
		int N = int(P.size());
		assert(N == int(Q.size()));
	};
	auto solve = [&](const std::vector<int>& P, const std::vector<int>& Q) -> std::vector<int> {
		int N = int(P.size());
		assert(N == int(Q.size()));
		std::vector<int> invP(N, -1);
		for (int i = 0; i < N; i++) invP[P[i]] = i;

		std::cerr << '\n' << "solve " << N << '\n';
		for (auto p : P) { std::cerr << p << ' '; } std::cerr << '\n';
		for (auto q : Q) { std::cerr << q << ' '; } std::cerr << '\n';

		assert(Q >= P);

		std::vector<int> ans; ans.reserve(N);
		std::vector<bool> used(N, false);

		int last_match_P = -1;
		bool is_match_Q = true;
		bool found_move = false;
		struct cond_t {
			int max_P_idx; // max_P_idx will never be <= max_val, so whatever
			int max_val;
		};

		int pref_mins_st = -1;
		std::vector<int> pref_mins(N+1);
		pref_mins[0] = N;
		for (int i = 0; i < N; i++) pref_mins[i+1] = std::min(pref_mins[i], invP[i]);

		std::vector<cond_t> conds; conds.reserve(N);
		for (int i = 0; i < N; i++) {
			std::cerr << "index " << i << '\n';
			auto try_extend = [&](int v) -> bool {
				std::cerr << "extend " << v << '\n';
				if (i == last_match_P+1 && v == P[i]) {
					// just in the matching prefix, do nothing
					last_match_P = i;
					ans.push_back(v);
					used[v] = true;
					return true;
				}
				if (last_match_P != pref_mins_st) {
					pref_mins_st = last_match_P;
					pref_mins[0] = N;
					for (int j = 0; j < N; j++) {
						pref_mins[j+1] = std::min(pref_mins[j], invP[j] <= pref_mins_st ? N : invP[j]);
					}
				}

				bool n_found_move = found_move;
				if (!found_move && !conds.empty() && v < ans.back() && invP[v] < invP[ans.back()]) {
					// we're now finished
					std::cerr << "  is last move" << '\n';
					n_found_move = true;
				}

				if (!n_found_move) {
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
					std::cerr << "final cap = " << cap << '\n';
					if (pref_mins[cap+1] > invP[v]) return false;

					// now, update things
					while (!conds.empty() && invP[v] <= conds.back().max_P_idx && cap <= conds.back().max_val) {
						conds.pop_back();
					}

					conds.push_back({invP[v], cap});

					ans.push_back(v);
					used[v] = true;
					return true;
				} else {
					if (!conds.empty() && v > conds.back().max_val && invP[v] < conds.back().max_P_idx) {
						if (invP[v] <= pref_mins[conds.back().max_val+1]) {
							return false;
						}
						cond_t new_cond{invP[v], conds.back().max_val};
						while (!conds.empty() && conds.back().max_P_idx >= new_cond.max_P_idx) conds.pop_back();
						conds.push_back(new_cond);

						found_move = n_found_move;
						ans.push_back(v);
						used[v] = true;
						return true;
					} else {
						// just delete things we're included in
						while (!conds.empty() && v <= conds.back().max_val && invP[v] < conds.back().max_P_idx) {
							conds.pop_back();
						}

						found_move = n_found_move;
						ans.push_back(v);
						used[v] = true;
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
				while (used[v] || !try_extend(v)) {
					v--;
				}
			} else {
				// we're already not matching Q, so we're free to be anything
				int min_idx;
				if (conds.empty()) {
					min_idx = 0;
				} else {
					min_idx = pref_mins[conds.back().max_val+1];
				}
				// pick the biggest unused thing after min_idx
				int v = N-1;
				while (used[v] || invP[v] < min_idx) v--;
				bool worked = try_extend(v);
				assert(worked);
			}
		}
		return ans;
	};

	{
		int N = 6;
		std::vector<int> P(N); std::iota(P.begin(), P.end(), 0);
		do {
			std::vector<int> Q = P;
			do {
				solve(P, Q);
			} while (std::next_permutation(Q.begin(), Q.end()));
		} while (std::next_permutation(P.begin(), P.end()));
	}

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
