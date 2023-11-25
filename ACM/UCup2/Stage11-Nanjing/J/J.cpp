#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

template <typename K, typename V, V default_value> struct sorted_vector_map {
	std::vector<std::pair<K, V>> vec;
	auto begin() {
		return vec.begin();
	}
	auto end() {
		return vec.end();
	}
	auto find(K k) {
		auto it = std::lower_bound(vec.begin(), vec.end(), k, [&](auto a, auto b) { return a.first < b; });
		if (it != vec.end() && it->first == k) return it;
		return vec.end();
	}
	V operator [](K k) {
		auto it = find(k);
		if (it == vec.end()) return default_value;
		else return it->second;
	}
	bool insert(K k, V v) {
		auto it = std::lower_bound(vec.begin(), vec.end(), k, [&](auto a, auto b) { return a.first < b; });
		if (it != vec.end() && it->first == k) {
			return std::exchange(it->second, v);
		} else {
			vec.insert(it, {k, v});
			return default_value;
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = false;
	std::mt19937 mt(48);

	int N_CASES;
	if (TESTING) {
		N_CASES = 1000;
	} else {
		cin >> N_CASES;
	}
	while (N_CASES--) {
		cerr << "Start case" << '\n';
		int N, M;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(1, 10)(mt);
			M = std::uniform_int_distribution<int>(1, 10)(mt);
			cerr << N << ' ' << M << '\n';
		} else {
			cin >> N >> M;
		}
		// P are in dfs order
		const int V = N+2; // 0, 1..N, N+1
		N++;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) {
			if (TESTING) {
				P[i] = std::uniform_int_distribution<int>(0, i-1)(mt);
				cerr << P[i] << " \n"[i+1==N];
			} else {
				cin >> P[i];
			}
		}
		std::vector<int> C(N);
		C[0] = 0;
		for (int i = 1; i < N; i++) {
			if (TESTING) {
				C[i] = std::uniform_int_distribution<int>(1, V-2)(mt);
				cerr << C[i] << " \n"[i+1==N];
			} else {
				cin >> C[i];
			}
		}

		std::vector<int> T(M);
		for (int m = 0; m < M; m++) {
			if (TESTING) {
				T[m] = std::uniform_int_distribution<int>(1, V-2)(mt);
				cerr << T[m] << " \n"[m+1==M];
			} else {
				cin >> T[m];
			}
		}
		std::set<std::pair<int, int>> seen;
		for (int i = 1; i < N; i++) {
			seen.insert({P[i], C[i]});
		}
		if (int(seen.size()) != N-1) {
			cerr << "Skipping" << '\n';
			continue;
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
			std::vector<std::array<int, 2>> par_suff_value(N);
			std::vector<int> cnt(cur_V);
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
		{
			std::vector<int> stk; stk.reserve(N);
			for (auto [i, d] : suffs) {
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

		cerr << "T" << ' '; for (int v : T) { cerr << v << ' '; } cerr << '\n';
		std::vector<int> T_fail(M+1);
		// includes all links to > 0
		std::vector<sorted_vector_map<int, int, 0>> T_next(M+1);
		T_fail[0] = -1;
		for (int i = 0; i < M; i++) {
			// returns the old value
			T_next[i].insert(T[i], i+1);
			T_fail[i+1] = T_fail[i] == -1 ? 0 : T_next[T_fail[i]][T[i]];
			T_next[i+1] = T_next[T_fail[i+1]];
		}
		cerr << "T_fail" << ' '; for (int v : T_fail) { cerr << v << ' '; } cerr << '\n';
		for (auto a : T_next) { for (auto [x, y] : a.vec) { cerr << x << "->" << y << ' '; } cerr << '\n'; }

		std::vector<std::vector<std::pair<int, int>>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[P[i]].push_back({C[i], i});
		}
		std::vector<int> match_dist(N, -1);
		std::vector<int> lcp_len(N, -1);
		match_dist[0] = 0;
		{
			std::vector<int> stk; stk.reserve(N);
			std::y_combinator([&](auto self, int i) -> void {
				stk.push_back(i);
				std::sort(ch[i].begin(), ch[i].end());
				auto ch_it = ch[i].begin();
				for (auto [c, j] : T_next[match_dist[i]].vec) {
					while (ch_it != ch[i].end() && ch_it->first < c) {
						match_dist[ch_it->second] = 0;
						self(ch_it->second);
						ch_it++;
					}
					if (ch_it != ch[i].end() && ch_it->first == c) {
						match_dist[ch_it->second] = j;
						self(ch_it->second);
						ch_it++;
						continue;
					} else {
						for (int a = j; a > 0; a = T_fail[a]) {
							int p = *(stk.end()-1-(a-1));
							assert(lcp_len[p] == -1);
							lcp_len[p] = a-1;
						}
					}
				}
				while (ch_it != ch[i].end()) {
					match_dist[ch_it->second] = 0;
					self(ch_it->second);
					ch_it++;
				}
				if (match_dist[i] == M) {
					int p = *(stk.end()-1-M);
					assert(lcp_len[p] == -1);
					lcp_len[p] = M;
				}
				assert(stk.back() == i);
				stk.pop_back();
			})(0);
		}
		for (int i = 0; i < N; i++) { cerr << i << ' ' << match_dist[i] << '\n'; }
		for (int i = 0; i < N; i++) { cerr << i << ' ' << lcp_len[i] << '\n'; }
		for (int i = 0; i < N; i++) {
			assert(lcp_len[i] != -1);
		}

		std::vector<std::vector<int>> failure_ch(N);
		for (int i = 1; i < N; i++) {
			failure_ch[failure[i]].push_back(i);
		}
		std::vector<std::vector<std::array<int, 2>>> evts(M+1);
		{
			std::vector<std::pair<int, int>> stk(N+1);
			int stk_size = 0;
			std::y_combinator([&](auto self, int cur) -> void {
				int mi = 0;
				int ma = stk_size;
				while (ma - mi > 1) {
					int md = mi + (ma - mi) / 2;
					if (lcp_len[cur] >= stk[md].first) {
						ma = md;
					} else {
						mi = md;
					}
				}
				auto old_path = stk[ma];
				stk[ma] = {lcp_len[cur], cur};
				int old_stk_size = stk_size;
				stk_size = ma + 1;

				if (cur != 0) {
					assert(stk[mi].second == 0 || stk[mi].first > lcp_len[cur]);
					evts[lcp_len[cur]].push_back({cur, stk[mi].second});
				}

				for (int nxt : failure_ch[cur]) {
					self(nxt);
				}

				stk[ma] = old_path;
				stk_size = old_stk_size;
			})(0);
		}

		std::vector<int> depth(N);
		for (int i = 1; i < N; i++) depth[i] = depth[P[i]] + 1;

		int64_t cur_ans = std::accumulate(depth.begin()+1, depth.end(), int64_t(0));
		std::vector<int> w(N, 1);
		w[0] = 0;

		int v0 = 0;

		std::vector<int64_t> answers(M+1);
		for (int m = 1; m <= M; m++) {
			//cerr << "m = " << m << '\n';
			// add the character itself
			cur_ans += N-1;

			// add the character m
			// first, adjust the "depth" of 0
			while (true) {
				if (v0 == -1) {
					v0 = 0;
					break;
				}
				auto it = std::lower_bound(ch[v0].begin(), ch[v0].end(), T[m-1], [&](auto a, auto b) { return a.first < b; });
				if (it != ch[v0].end() && it->first == T[m-1]) {
					v0 = it->second;
					break;
				} else {
					v0 = failure[v0];
				}
			}

			{
				int n_depth_0 = depth[v0] - m;
				cur_ans += int64_t(w[0]) * int64_t(n_depth_0 - depth[0]);
				depth[0] = n_depth_0;
			}
			for (auto [src, dest] : evts[m-1]) {
				//cerr << "move " << src << ' ' << dest << '\n';
				//assert(w[dest] > 0 && w[src] > 0);
				assert(depth[src] > depth[dest]);
				cur_ans += int64_t(w[src]) * int64_t(depth[dest] - depth[src]);
				w[dest] += w[src];
				w[src] = 0;
			}

			/*
			int64_t slo_ans = int64_t(N-1) * m;
			for (int i = 0; i < N; i++) {
				slo_ans += int64_t(depth[i]) * w[i];
			}
			cerr << slo_ans << ' ' << cur_ans << '\n';
			*/
			//assert(slo_ans == cur_ans);
			answers[m] = cur_ans;
		}
		for (int m = 1; m <= M; m++) {
			cout << answers[m] << " \n"[m == M];
		}
	}

	return 0;
}
