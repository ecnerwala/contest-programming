#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi mo(vector<pii> Q, auto&& add, auto&& del, auto&& calc) {
	int L = 0, R = 0, blk = 350; // ~N/sqrt(Q)
	vi s(sz(Q)), res = s;
#define K(x) pii(x.first/blk, x.second ^ -(x.first/blk & 1))
	iota(all(s), 0);
	sort(all(s), [&](int s, int t){ return K(Q[s]) < K(Q[t]); });
	for (int qi : s) {
		pii q = Q[qi];
		while (L > q.first) add(--L, 0);
		while (R < q.second) add(R++, 1);
		while (L < q.first) del(L++, 0);
		while (R > q.second) del(--R, 1);
		res[qi] = calc(qi);
	}
	return res;
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::tuple<int, int, int>> edges; edges.reserve(2*M);
		std::vector<int> deg(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			assert(u != v);
			edges.push_back({u, v, e});
			edges.push_back({v, u, e});
			deg[u]++;
			deg[v]++;
		}
		std::sort(edges.begin(), edges.end());
		// maximum label
		const int V = 1 << (32 - __builtin_clz(N));

		std::vector<int> deg_pref(N+1);
		deg_pref[0] = 0;
		for (int i = 0; i < N; i++) deg_pref[i+1] = deg_pref[i] + deg[i];
		assert(deg_pref[N] == 2*M);

		std::vector<int> alive(M, 0);

		struct query_t {
			int l, r, k;
		};
		int Q; cin >> Q;
		std::vector<query_t> queries(Q);
		std::vector<std::pair<int, int>> query_pos; query_pos.reserve(Q);
		for (auto& [l, r, k] : queries) {
			cin >> l >> r >> k; l--;
			query_pos.push_back({deg_pref[l], deg_pref[r]});
		}

		const int BUCKET_SIZE = 512;
		std::vector<int> val_freq(V);
		std::vector<int> bucket_freq(((V-1) / BUCKET_SIZE) + 1);

		std::vector<int> val(N);
		for (int i = 0; i < N; i++) {
			val_freq[val[i]]++;
			bucket_freq[val[i] / BUCKET_SIZE]++;
		}

		auto update_val = [&](int u, int v) -> void {
			val_freq[val[u]] --;
			bucket_freq[val[u] / BUCKET_SIZE] --;
			val[u] ^= v+1;
			val_freq[val[u]] ++;
			bucket_freq[val[u] / BUCKET_SIZE] ++;
		};

		auto add = [&](int ind, int _) {
			auto [src, dst, e] = edges[ind];
			alive[e]++;
			if (alive[e] == 2) {
				update_val(src, dst);
				update_val(dst, src);
			}
		};
		auto del = [&](int ind, int _) {
			auto [src, dst, e] = edges[ind];
			if (alive[e] == 2) {
				update_val(src, dst);
				update_val(dst, src);
			}
			alive[e]--;
		};
		auto calc = [&](int q) -> int {
			auto [l, r, k] = queries[q];
			// the prefix is all 0's
			k += N - (r - l);
			int b = 0;
			while (k > bucket_freq[b]) {
				k -= bucket_freq[b];
				b++;
			}
			int i = b * BUCKET_SIZE;
			while (k > val_freq[i]) {
				k -= val_freq[i];
				i++;
			}
			return i;
		};
		auto ans = kactl::mo(query_pos, add, del, calc);
		for (int i = 0; i < Q; i++) {
			cout << ans[i] << '\n';
		}
	}

	return 0;
}
