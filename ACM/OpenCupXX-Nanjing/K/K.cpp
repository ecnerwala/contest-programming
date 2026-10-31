#define NDEBUG
#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
using namespace std;

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

struct bit_segtree {
	vector<int> bit;
	bit_segtree() : bit() {}
	bit_segtree(int N) : bit(N) {}
	int query(int i) {
		int ans = 0;
		for (int a = i+1; a; a -= a & -a) {
			ans += bit[a-1];
		}
		return ans;
	}
	void update1(int l, int z) {
		for (int a = l+1; a <= int(bit.size()); a += a & -a) {
			bit[a-1] += z;
		}
	}

	void update(int l, int r, int z) {
		update1(l, z);
		update1(r, -z);
	}
};

struct fast_update_segtree {
	int N, B;
	vector<int> val;
	vector<int> val_buck;

	fast_update_segtree() : N(0), B(0), val(), val_buck() {}
	fast_update_segtree(int N_) : N(N_), B(max(int(sqrt(N)), 1)), val(N+1), val_buck(N/B+1) {}

	int query(int i) {
		int ans = 0;
		i++;
		while (i % B) {
			ans += val[--i];
		}
		i /= B;
		while (i) {
			ans += val_buck[--i];
		}
		return ans;
	}

	void update1(int l, int z) {
		val[l] += z;
		val_buck[l/B] += z;
	}

	void update(int l, int r, int z) {
		update1(l, z);
		update1(r, -z);
	}
};

struct fast_query_segtree {
	int N, B;
	vector<int> val;
	vector<int> val_buck;

	fast_query_segtree() : N(0), B(0), val(), val_buck() {}
	fast_query_segtree(int N_) : N(N_), B(max(int(sqrt(N)), 1)), val(N+1), val_buck((N-1)/B+1) {}

	int query(int i) {
		return val[i] + val_buck[i/B];
	}

	void update(int l, int r, int z) {
		while (l < r && l % B) {
			val[l++] += z;
		}
		while (l < r && r % B) {
			val[--r] += z;
		}
		if (l == r) return;
		l /= B, r /= B;
		while (l < r) val_buck[l++] += z;
	}
};

template <typename segtree> struct compressed_segtree {
	vector<int> vals;
	segtree seg;

	compressed_segtree() : vals(), seg() {}

	int to_ind(int a) {
		return int(lower_bound(vals.begin(), vals.end(), a) - vals.begin());
	}

	void add_val(int v) { vals.push_back(v); }

	void init() { sort(vals.begin(), vals.end()); seg = segtree(int(vals.size())); }

	int query(int i) {
		//assert(binary_search(vals.begin(), vals.end(), i));
		i = to_ind(i);
		return seg.query(i);
	}

	bool update(int l, int r, int z) {
		l = to_ind(l);
		r = to_ind(r);
		if (l == r) return false;
		seg.update(l, r, z);
		return true;
	}
};

struct level_anc {
	int N;
	vector<int> preorder;
	vector<int> idx;
	vector<pair<int, int>> heavyPar; // heavy parent, distance
	level_anc() : N(0) {}

	level_anc(const vector<int>& par) : N(int(par.size())), preorder(N), idx(N), heavyPar(N) {
		vector<vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) ch[par[i]].push_back(i);
		}
		vector<int> sz(N);
		int nxt_idx = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] == -1) {
				std::y_combinator([&](auto self, int cur) -> void {
					sz[cur] = 1;
					for (int nxt : ch[cur]) {
						self(nxt);
						sz[cur] += sz[nxt];
					}
					if (!ch[cur].empty()) {
						auto mit = max_element(ch[cur].begin(), ch[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; });
						swap(*ch[cur].begin(), *mit);
					}
				})(i);
				std::y_combinator([&](auto self, int cur, int isRoot = true) -> void {
					preorder[idx[cur] = nxt_idx++] = cur;
					if (isRoot) {
						heavyPar[idx[cur]] = {par[cur] == -1 ? -1 : idx[par[cur]], 1};
					} else {
						assert(idx[par[cur]] == idx[cur]-1);
						heavyPar[idx[cur]] = heavyPar[idx[cur]-1];
						heavyPar[idx[cur]].second++;
					}
					bool chRoot = false;
					for (int nxt : ch[cur]) {
						self(nxt, chRoot);
						chRoot = true;
					}
				})(i);
			}
		}
	}

	int get_anc(int a, int k) const {
		assert(k >= 0);
		a = idx[a];
		while (a != -1 && k) {
			if (k >= heavyPar[a].second) {
				k -= heavyPar[a].second;
				assert(heavyPar[a].first <= a - heavyPar[a].second);
				a = heavyPar[a].first;
			} else {
				a -= k;
				k = 0;
			}
		}
		if (a == -1) return -1;
		else return preorder[a];
	}

	int lca(int a, int b) const {
		a = idx[a], b = idx[b];
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				return preorder[a];
			}
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
	}
};

mt19937 mt(48);

void go() {
	int N, M; cin >> N >> M;
	vector<int> par(N);
	vector<vector<int>> ch(N);
	par[0] = 0;
	for (int i = 1; i < N; i++) {
		cin >> par[i]; par[i]--;
		ch[par[i]].push_back(i);
	}

	vector<int> depth(N);
	vector<int> st(N);
	vector<int> en(N);
	vector<int> preorder(N);
	{
		int idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			preorder[st[cur] = idx++] = cur;
			for (int nxt : ch[cur]) {
				depth[nxt] = depth[cur] + 1;
				self(nxt);
			}
			en[cur] = idx;
		})(0);
	}

	const int SMALL_MOD = 100;
	//const int SMALL_MOD = 0;
	using update_t = tuple<int, int, int, int, int>; // q a x y z
	int Q = 0;
	vector<int> queries; queries.reserve(M);
	vector<update_t> large_updates; large_updates.reserve(M);
	vector<vector<vector<update_t>>> small_updates(SMALL_MOD+1);
	for (int m = 1; m <= SMALL_MOD; m++) small_updates[m].resize(m);

	for (int m = 0; m < M; m++) {
		int op; cin >> op;
		if (op == 1) {
			int a, x, y, z; cin >> a >> x >> y >> z; a--;

			if (x <= SMALL_MOD) {
				small_updates[x][(depth[a]+y) % x].emplace_back(Q, a, x, y, z);
			} else {
				large_updates.emplace_back(Q, a, x, y, z);
			}
		} else if (op == 2) {
			int a; cin >> a; a--;
			queries.push_back(a);
			Q++;
		} else assert(false);
	}

	assert(int(queries.size()) == Q);
	vector<int> ans(Q);

	{
		fast_query_segtree small_seg(N);
		vector<int> small_queries(Q);
		vector<int> small_query_inds(SMALL_MOD+1);
		for (int m = 1; m <= SMALL_MOD; m++) {
			for (int v = 0; v <= m; v++) small_query_inds[v] = 0;
			for (int q = 0; q < Q; q++) {
				int a = queries[q];
				small_query_inds[depth[a]%m] ++;
			}
			for (int v = 0; v < m; v++) small_query_inds[v+1] += small_query_inds[v];
			for (int q = Q-1; q >= 0; q--) {
				int a = queries[q];
				small_queries[--small_query_inds[depth[a]%m]] = q;
			}
			for (int v = 0; v < m; v++) {
				auto it = small_updates[m][v].begin();
				for (int qi = small_query_inds[v]; qi < small_query_inds[v+1]; qi++) {
					int q = small_queries[qi];
					while (it != small_updates[m][v].end() && get<0>(*it) <= q) {
						auto [_, a, x, y, z] = *it;
						small_seg.update(st[a], en[a], z);
						++it;
					}
					{
						int a = queries[q];
						ans[q] += small_seg.query(st[a]);
					}
				}
				while (it != small_updates[m][v].begin()) {
					--it;
					auto [_, a, x, y, z] = *it;
					small_seg.update(st[a], en[a], -z);
				}
			}
		}
	}

	{
		int D = *max_element(depth.begin(), depth.end())+1;
		vector<int> depth_st(D+1);
		for (int i = 0; i < N; i++) {
			depth_st[depth[i]]++;
		}
		for (int d = 0; d < D; d++) {
			depth_st[d+1] += depth_st[d];
		}
		vector<int> depth_nodes(N);
		for (int i = 0; i < N; i++) {
			depth_nodes[--depth_st[depth[i]]] = st[i];
		}
		for (int d = 0; d < D; d++) {
			sort(depth_nodes.begin() + depth_st[d], depth_nodes.begin() + depth_st[d+1]);
		}

		vector<int> depth_idx(N);
		for (int i = 0; i < N; i++) {
			depth_idx[preorder[depth_nodes[i]]] = i;
		}

		level_anc st_anc;
		level_anc en_anc;
		{
			vector<int> nxt_st(N, -1);
			vector<int> nxt_en(N, -1);
			for (int d = 0; d+1 < D; d++) {
				int lo = depth_st[d+1];
				int hi = depth_st[d+1];
				for (int i = depth_st[d]; i < depth_st[d+1]; i++) {
					int node = preorder[depth_nodes[i]];
					while (lo < depth_st[d+2] && depth_nodes[lo] < st[node]) lo++;
					while (hi < depth_st[d+2] && depth_nodes[hi] < en[node]) hi++;
					nxt_st[i] = (lo == depth_st[d+2] ? -1 : lo);
					nxt_en[i] = (hi == depth_st[d+1] ? -1 : hi-1);
				}
			}
			st_anc = level_anc(nxt_st);
			en_anc = level_anc(nxt_en);
		}

		vector<fast_update_segtree> depth_seg(D);
		for (int d = 0; d < D; d++) {
			depth_seg[d] = fast_update_segtree(depth_st[d+1] - depth_st[d]);
		}
		auto it = large_updates.begin();
		for (int q = 0; q < Q; q++) {
			while (it != large_updates.end() && get<0>(*it) <= q) {
				auto [_, a, x, y, z] = *it;
				for (int d = depth[a]+y, lo = st_anc.get_anc(depth_idx[a], y), hi = en_anc.get_anc(depth_idx[a], y); lo != -1 && hi != -1 && lo <= hi; d += x, lo = st_anc.get_anc(lo, x), hi = en_anc.get_anc(hi, x)) {
					depth_seg[d].update(lo - depth_st[d], hi+1 - depth_st[d], z);
				}
				++it;
			}
			{
				int a = queries[q];
				ans[q] += depth_seg[depth[a]].query(depth_idx[a] - depth_st[depth[a]]);
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		go();
	}

	return 0;
}
