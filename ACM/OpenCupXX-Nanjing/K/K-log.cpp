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

struct segtree {
	vector<int> vals;
	vector<int> bit;

	segtree() : vals(), bit() {}

	int to_ind(int a) {
		return int(lower_bound(vals.begin(), vals.end(), a) - vals.begin());
	}

	void add_val(int v) { vals.push_back(v); }

	void init() { sort(vals.begin(), vals.end()); bit.assign(2 * vals.size(), 0); }

	int query(int i) {
		//assert(binary_search(vals.begin(), vals.end(), i));
		i = to_ind(i);
		int ans = 0;
		for (int a = i+1; a; a -= a & -a) {
			ans += bit[a-1];
		}
		return ans;
	}

	void update1(int l, int z) {
		for (int a = l+1; a <= int(vals.size()); a += a & -a) {
			bit[a-1] += z;
		}
	}

	bool update(int l, int r, int z) {
		l = to_ind(l);
		r = to_ind(r);
		if (l == r) return false;
		update1(l, z);
		update1(r, -z);
		return true;
	}
};

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
	{
		int idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			st[cur] = idx++;
			for (int nxt : ch[cur]) {
				depth[nxt] = depth[cur] + 1;
				self(nxt);
			}
			en[cur] = idx;
		})(0);
	}

	const int SMALL_MOD = 100;
	using update_t = tuple<int, int, int, int, int>; // q a x y z
	int Q = 0;
	vector<int> queries; queries.reserve(M);
	vector<update_t> large_updates; large_updates.reserve(M);
	vector<vector<update_t>> small_updates(SMALL_MOD+1);

	for (int m = 0; m < M; m++) {
		int op; cin >> op;
		if (op == 1) {
			int a, x, y, z; cin >> a >> x >> y >> z; a--;

			if (x <= SMALL_MOD) {
				small_updates[x].emplace_back(Q, a, x, y, z);
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

	for (int m = 1; m <= SMALL_MOD; m++) {
		vector<segtree> small_seg(m);
		for (int i = 0; i < N; i++) {
			small_seg[depth[i]%m].add_val(st[i]);
		}
		for (int v = 0; v < m; v++) {
			small_seg[v].init();
		}
		auto it = small_updates[m].begin();
		for (int q = 0; q < Q; q++) {
			while (it != small_updates[m].end() && get<0>(*it) <= q) {
				auto [_, a, x, y, z] = *it;
				assert(x == m);
				small_seg[(depth[a] + y) % x].update(st[a], en[a], z);
				++it;
			}
			{
				int a = queries[q];
				ans[q] += small_seg[depth[a] % m].query(st[a]);
			}
		}
	}

	{
		vector<segtree> depth_seg(*max_element(depth.begin(), depth.end())+1);
		for (int i = 0; i < N; i++) {
			depth_seg[depth[i]].add_val(st[i]);
		}
		for (int i = 0; i < int(depth_seg.size()); i++) {
			depth_seg[i].init();
		}
		auto it = large_updates.begin();
		for (int q = 0; q < Q; q++) {
			while (it != large_updates.end() && get<0>(*it) <= q) {
				auto [_, a, x, y, z] = *it;
				for (int d = depth[a] + y; d < int(depth_seg.size()); d += x) {
					if (!depth_seg[d].update(st[a], en[a], z)) break;
				}
				++it;
			}
			{
				int a = queries[q];
				ans[q] += depth_seg[depth[a]].query(st[a]);
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
	//T = min(T, 1);
	while (T--) {
		go();
	}

	return 0;
}
