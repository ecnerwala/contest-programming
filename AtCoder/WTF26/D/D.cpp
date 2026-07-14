#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto can_make_slow = [&](std::vector<int> P, std::vector<int> Q) -> bool {
		assert(int(P.size()) == int(Q.size()));
		int N = int(P.size());
		std::vector<int> invP(N, -1);
		for (int i = 0; i < N; i++) invP[P[i]] = i;
		int st = 0;
		while (st < N && P[st] == Q[st]) st++;
		if (st == N) return true;
		assert(P[st] != Q[st]);
		assert(P[st] < Q[st]);
		int first_move = st;
		while (first_move + 1 < N && !(Q[first_move+1] < Q[first_move] && invP[Q[first_move+1]] < invP[Q[first_move]])) first_move++;
		if (first_move == N-1) return false;
		struct stk_t {
			int v;
			int p;
		};
		std::vector<stk_t> stk; stk.reserve(N+1);
		stk.push_back({N, -1});
		for (int i = N-1; i >= st; i--) {
			int v = Q[i];
			int p = invP[v];
			while (stk.back().p > p) {
				stk.pop_back();
			}
			if (v < stk.back().v) {
				stk.push_back({v, p});
			}
			if (i <= first_move) {
				int cap = std::min(Q[i] - 1, Q[i+1]);
				if (stk.back().v > cap) return false;
			}
		}
		return true;
	};

	auto solve = [&](std::vector<int> P, std::vector<int> Q) -> std::vector<int> {
		int N = int(P.size());
		assert(N == int(Q.size()));
		std::vector<int> invP(N, -1);
		for (int i = 0; i < N; i++) invP[P[i]] = i;

		std::cerr << '\n' << "solve " << N << '\n';
		for (auto p : P) { std::cerr << p << ' '; } std::cerr << '\n';
		for (auto q : Q) { std::cerr << q << ' '; } std::cerr << '\n';

		assert(Q >= P);

		std::vector<bool> used(N, false);

		int st = 0;
		while (st < N && P[st] == Q[st]) {
			used[Q[st]] = true;
			st++;
		}

		bool found_move = false;
		struct cond_t {
			int max_p; // we will always have used[cond.max_p] == true, so this never ties
			int max_v; // this is inclusive
		};

		std::vector<int> pref_mins(N+1);

		for (int v = 0; v < N; v++) {
			pref_mins[v] = used[v] ? N : invP[v];
			if (v) pref_mins[v] = std::min(pref_mins[v], pref_mins[v-1]);
		}
		pref_mins[N] = -1;

		std::vector<cond_t> conds; conds.reserve(N+1);
		conds.push_back({-1, N});

		int i = st;
		for (; i < N; i++) {
			//std::cerr << "index " << i << '\n';
			//std::cerr << " conds" << '\n';
			//for (const auto& c : conds) { std::cerr << ' ' << c.max_p << ',' << c.max_v << '\n'; } std::cerr << '\n';
			auto try_extend = [&]() -> bool {
				int v = Q[i];
				int p = invP[v];

				//std::cerr << " try_extend " << "v = " << v << ' ' << "p = " << p << '\n';
				if (i == st && v == P[i]) {
					// just in the matching prefix, do nothing
					used[v] = true;
					return true;
				}

				bool n_found_move = found_move;
				if (!found_move && i-1 >= st && v < Q[i-1] && p < invP[Q[i-1]]) {
					// we're now finished
					//std::cerr << "  is last move" << '\n';
					n_found_move = true;
				}

				if (!n_found_move) {
					// if i == N-1, we should've hit the finished_pref condition to satisfy the condition at Q[i-1]
					assert(i < N-1);

					int cap = std::min(Q[i], Q[i+1]);

					if (p < conds.back().max_p) {
						cap = std::min(cap, conds.back().max_v);
					}

					// we need there to still exist something < p and <= cap
					//std::cerr << "  final cap = " << cap << '\n';
					if (p <= pref_mins[cap]) return false;

					// now, update things
					// since we dropped the cap down already, our new one is always tighter

					while (p < conds.back().max_p) conds.pop_back();

					if (cap < conds.back().max_v) {
						conds.push_back({p, cap});
					}
				} else {
					if (v > conds.back().max_v && p < conds.back().max_p) {
						int cap = conds.back().max_v;
						// Check that the range is satisfiable
						if (p <= pref_mins[cap]) return false;
						while (p < conds.back().max_p) conds.pop_back();
						conds.push_back({p, cap});
					} else {
						// just delete things we're included in
						while (p < conds.back().max_p) conds.pop_back();
					}

					found_move = n_found_move;
				}
				used[v] = true;
				return true;
			};
			if (!try_extend()) {
				assert(i+1 < N);
				Q[i+1] = N;
				Q[i]--;
				while (used[Q[i]] || !try_extend()) Q[i]--;
				i++;
				// Finish the rest by looking at conds
				break;
			}
		}
		{
			std::cerr << " conds" << '\n';
			for (const auto& c : conds) { std::cerr << ' ' << c.max_p << ',' << c.max_v << '\n'; } std::cerr << '\n';
			// Now, fill in Q[i:N] greedily
			std::vector<int> block_starts; block_starts.reserve(N+1);
			block_starts.push_back(i);
			std::vector<int> p_to_block(N, -1);
			int last_p_thresh = N;
			while (conds.back().max_p != -1) {
				cond_t cond = conds.back();
				conds.pop_back();
				int p_thresh = pref_mins[cond.max_v];
				while (p_thresh == pref_mins[conds.back().max_v]) {
					cond.max_p = conds.back().max_p;
					conds.pop_back();
				}
				cond.max_p = std::min(cond.max_p, last_p_thresh);
				int min_p = p_thresh;
				int min_p_val = P[min_p];
				for (int p = min_p+1; p < cond.max_p; p++) {
					if (used[P[p]]) continue;
					if (P[p] < min_p_val) continue;
					if (P[p] > cond.max_v) break;
					min_p = p;
					min_p_val = P[p];
				}
				assert(min_p >= p_thresh);
				assert(min_p < cond.max_p && P[min_p] <= cond.max_v);
				while (min_p < conds.back().max_p) conds.pop_back();
				int block_idx = int(block_starts.size())-1;
				int en = block_starts.back();
				assert(min_p < last_p_thresh);
				for (int p = last_p_thresh - 1; p >= min_p; p--) {
					int v = P[p];
					if (used[v]) continue;
					if (v >= min_p_val) {
						p_to_block[last_p_thresh] = block_idx;
						en++;
					}
				}
				block_starts.push_back(en);
				last_p_thresh = p_thresh;
			}
			for (int v = N-1; v >= 0; v--) {
				if (used[v]) continue;
				int b = p_to_block[invP[v]];
				if (b == -1) b = int(block_starts.size())-1;
				Q[block_starts[b]++] = v;
			}
		}
		std::cerr << "ans "; for (auto q : Q) { std::cerr << q << ' '; } std::cerr << '\n';

		return Q;
	};

	for (int N = 1; N <= 8; N++) {
		std::cerr << "testing N = " << N << '\n';
		std::vector<int> P(N); std::iota(P.begin(), P.end(), 0);
		do {
			std::vector<int> slo_ans = P;
			std::vector<int> Q = P;
			do {
				if (can_make_slow(P, Q)) slo_ans = Q;
				auto my_ans = solve(P, Q);
				assert(my_ans == slo_ans);
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
