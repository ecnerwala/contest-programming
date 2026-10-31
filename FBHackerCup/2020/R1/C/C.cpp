#include<bits/stdc++.h>

template <typename T, typename U> struct cnt_max {
	T v = -1; U cnt = 0;
	cnt_max() {}
	explicit cnt_max(T v_) : v(v_), cnt(1) {}
	cnt_max(T v_, U u_) : v(v_), cnt(u_) {}
	friend cnt_max operator + (const cnt_max& a, const cnt_max& b) {
		cnt_max r;
		r.v = std::max(a.v, b.v);
		r.cnt = 0;
		if (a.v == r.v) r.cnt += a.cnt;
		if (b.v == r.v) r.cnt += b.cnt;
		return r;
	}
	cnt_max& operator += (const cnt_max& o) {
		return *this = *this + o;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		//cerr << "Case #" << case_num << ":" << '\n';
		int N, K; cin >> N >> K;
		string S; cin >> S;
		vector<bool> healthy(N);
		for (int i = 0; i < N; i++) {
			assert(S[i] == '*' || S[i] == '#');
			healthy[i] = S[i] == '*';
		}
		vector<int> par(N);
		{
			for (int i = 1; i < K+1; i++) {
				cin >> par[i];
			}
			int64_t A, B, C; cin >> A >> B >> C;
			for (int i = K+1; i < N; i++) {
				par[i] = int((A * par[i-2] + B * par[i-1] + C) % i + 1);
			}
			for (int i = 1; i < N; i++) {
				par[i]--;
			}
		}
		par[0] = -1;

		//cerr << S << '\n';
		//for (int i = 0; i < N; i++) { cerr << par[i] << ' '; } cerr << '\n';

		vector<vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[par[i]].push_back(i);
		}

		int64_t orig_pairs = 0;
		auto c2 = [](int a) -> int64_t {
			return int64_t(a) * int64_t(a-1) / 2;
		};

		struct subtree_state {
			int cur_sz = 0;
			cnt_max<int, int> max_sz;
		};

		vector<subtree_state> dp_down(N);
		for (int i = N-1; i >= 0; i--) {
			if (healthy[i]) {
				dp_down[i].cur_sz = 1;
				dp_down[i].max_sz = cnt_max<int, int>();
				for (int j : ch[i]) {
					dp_down[i].cur_sz += dp_down[j].cur_sz;
					dp_down[i].max_sz += dp_down[j].max_sz;
				}
			} else {
				dp_down[i].cur_sz = 0;
				dp_down[i].max_sz = cnt_max<int, int>();
				for (int j : ch[i]) {
					orig_pairs += c2(dp_down[j].cur_sz);
					dp_down[i].max_sz += dp_down[j].max_sz;
				}
			}
			dp_down[i].max_sz += cnt_max<int, int>(dp_down[i].cur_sz);
		}

		orig_pairs += c2(dp_down[0].cur_sz);
		//cerr << orig_pairs << '\n';

		vector<subtree_state> dp_up(N);
		dp_up[0].cur_sz = 0;
		dp_up[0].max_sz = cnt_max<int, int>();
		for (int i = 0; i < N; i++) {
			// i'll just do prefix/suffix sums
			if (healthy[i]) {
				int tot_sz = 1 + dp_up[i].cur_sz;
				for (int j : ch[i]) {
					tot_sz += dp_down[j].cur_sz;
				}
				for (int j : ch[i]) {
					dp_up[j].cur_sz = tot_sz - dp_down[j].cur_sz;
				}
			} else {
				for (int j : ch[i]) {
					dp_up[j].cur_sz = 0;
				}
			}
			{
				// deal with the max_sz
				int d = int(ch[i].size());
				vector<cnt_max<int, int>> pref(d+1);
				vector<cnt_max<int, int>> suff(d+1);
				pref[0] = dp_up[i].max_sz;
				for (int z = 0; z < d; z++) {
					pref[z+1] = pref[z] + dp_down[ch[i][z]].max_sz;
				}
				suff[d] = cnt_max<int, int>();
				for (int z = d-1; z >= 0; z--) {
					suff[z] = suff[z+1] + dp_down[ch[i][z]].max_sz;
				}
				for (int z = 0; z < d; z++) {
					dp_up[ch[i][z]].max_sz = pref[z] + suff[z+1];
				}
			}
			for (int j : ch[i]) {
				dp_up[j].max_sz += cnt_max<int, int>(dp_up[j].cur_sz);
			}
		}

		vector<int> subtree_sz(N, 1);
		for (int i = N-1; i > 0; i--) {
			subtree_sz[par[i]] += subtree_sz[i];
		}

		auto nodes = [](cnt_max<int, int> c) -> int {
			return c.v * c.cnt;
		};

		cnt_max<int64_t, int64_t> ans;
		for (int i = 1; i < N; i++) {
			int64_t nxt_pairs = orig_pairs;
			nxt_pairs -= int64_t(dp_up[i].cur_sz) * int64_t(dp_down[i].cur_sz);
			nxt_pairs += int64_t(dp_up[i].max_sz.v) * int64_t(dp_down[i].max_sz.v);
			int64_t ways;
			if (dp_up[i].max_sz.v == 0 || dp_down[i].max_sz.v == 0) {
				// anything works
				ways = int64_t(subtree_sz[i]) * int64_t(N - subtree_sz[i]);
			} else {
				ways = int64_t(nodes(dp_up[i].max_sz)) * int64_t(nodes(dp_down[i].max_sz));
			}

			//cerr << "cnd " << i << ' ' << nxt_pairs << ' ' << ways << '\n';

			ans += cnt_max<int64_t, int64_t>(nxt_pairs, ways);
		}

		cout << "Case #" << case_num << ": " << ans.v << ' ' << ans.cnt << '\n';
	}

	return 0;
}
