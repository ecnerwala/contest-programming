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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	std::vector<std::array<int, 2>> edges(N-1);
	std::vector<int> C(N-1);
	std::vector<int> D(N-1);
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		auto& [u, v] = edges[e];
		cin >> u >> v; u--, v--;
		cin >> C[e] >> D[e];
		adj[u].push_back({v, e});
		adj[v].push_back({u, e});
	}

	std::vector<int> dp_down(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		dp_down[cur] = A[cur];
		for (auto [nxt, e] : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			dp_down[cur] = std::max(dp_down[cur], dp_down[nxt]);
		}
	})(0, -1);
	std::vector<int> dp_up(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		int v = A[cur];
		if (prv != -1) v = std::max(v, dp_up[cur]);
		int c1 = -1;
		int c2 = -1;
		for (auto [nxt, e] : adj[cur]) {
			if (nxt == prv) continue;
			if (dp_down[nxt] > c1) {
				c2 = c1;
				c1 = dp_down[nxt];
			} else if (dp_down[nxt] > c2) {
				c2 = dp_down[nxt];
			}
		}
		for (auto [nxt, e] : adj[cur]) {
			if (nxt == prv) continue;
			dp_up[nxt] = std::max(v, dp_down[nxt] == c1 ? c2 : c1);
			C[e] = std::min(C[e], std::min(dp_up[nxt], dp_down[nxt]));
			self(nxt, cur);
		}
	})(0, -1);

	std::vector<std::pair<int, int>> evts; evts.reserve(2*N-1);

	for (int i = 0; i < N; i++) {
		evts.push_back({A[i], i});
	}
	// edges come first
	for (int e = 0; e < N-1; e++) {
		evts.push_back({C[e], ~e});
	}

	std::sort(evts.begin(), evts.end());

	std::vector<int> par(N, -1);

	int num_edges = N-1;
	int num_verts = N;

	std::set<std::pair<int, int>> small_edges;
	std::set<std::pair<int, int>> big_edges;
	int64_t big_edges_tot = 0;

	std::set<std::pair<int, int>> small_verts;
	std::set<std::pair<int, int>> big_verts;
	int64_t big_verts_tot = 0;
	
	int64_t leaf_cnt = 0;
	int64_t leaf_tot = 0;
	auto mark_leaf = [&](std::pair<int, int> l) {
		leaf_cnt++;
		leaf_tot += l.first;
		if (big_verts.count(l)) {
			big_verts_tot -= l.first;
			big_verts.erase(l);
		} else {
			small_verts.erase(l);
		}
	};
	auto unmark_leaf = [&](std::pair<int, int> l) {
		leaf_cnt--;
		leaf_tot -= l.first;
		// insert l in the right place
		if (big_verts.empty() || l < *big_verts.begin()) {
			small_verts.insert(l);
		} else {
			big_verts.insert(l);
			big_verts_tot += l.first;
		}
	};

	auto get_best = [&]() -> int64_t {
		int tgt_verts = std::min(num_edges+1, num_verts);
		assert(tgt_verts >= 1);

		int tgt_edges = tgt_verts - 1;

		assert(tgt_verts >= leaf_cnt);
		while (int(big_verts.size()) > tgt_verts - leaf_cnt) {
			auto it = big_verts.begin();
			small_verts.insert(*it);
			big_verts_tot -= it->first;
			big_verts.erase(it);
		}
		while (int(big_verts.size()) < tgt_verts - leaf_cnt) {
			auto it = std::prev(small_verts.end());
			big_verts_tot += it->first;
			big_verts.insert(*it);
			small_verts.erase(it);
		}

		while (int(big_edges.size()) > tgt_edges) {
			auto it = big_edges.begin();
			small_edges.insert(*it);
			big_edges_tot -= it->first;
			big_edges.erase(it);
		}
		while (int(big_edges.size()) < tgt_edges) {
			auto it = std::prev(small_edges.end());
			big_edges_tot += it->first;
			big_edges.insert(*it);
			small_edges.erase(it);
		}

		/*
		cerr << "tgt_verts " << tgt_verts << '\n';
		cerr << "tgt_edges " << tgt_verts << '\n';
		cerr << "leaf_cnt " << leaf_cnt << '\n';
		cerr << "leaf_tot " << leaf_tot << '\n';
		cerr << "big_verts_tot " << big_verts_tot << '\n';
		cerr << "big_edges_tot " << big_edges_tot << '\n';
		*/
		return leaf_tot + big_verts_tot + big_edges_tot;
	};


	std::vector<int> cur_deg(N);
	std::vector<std::set<std::pair<int, int>>> comp_verts(N);
	for (int i = 0; i < N; i++) {
		cur_deg[i] = int(adj[i].size());
		comp_verts[i].insert({B[i], i});
		big_verts.insert({B[i], i});
		big_verts_tot += B[i];
		if (cur_deg[i] == 1) {
			mark_leaf(*comp_verts[i].rbegin());
		}
	}
	for (int e = 0; e < N-1; e++) {
		big_edges.insert({D[e], e});
		big_edges_tot += D[e];
	}

	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	auto merge = [&](int a, int b) {
		a = get_par(a), b = get_par(b);
		assert(a != b);
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;

		if (cur_deg[a] == 1) {
			assert(!comp_verts[a].empty());
			auto it = *comp_verts[a].rbegin();
			unmark_leaf(it);
		}
		if (cur_deg[b] == 1) {
			assert(!comp_verts[b].empty());
			auto it = *comp_verts[b].rbegin();
			unmark_leaf(it);
		}

		cur_deg[a] += cur_deg[b] - 2;
		for (auto it : comp_verts[b]) {
			comp_verts[a].insert(it);
		}
		comp_verts[b] = {};

		if (cur_deg[a] == 1) {
			assert(!comp_verts[a].empty());
			auto it = *comp_verts[a].rbegin();
			mark_leaf(it);
		}
	};

	int64_t ans = 0;
	for (auto [t, id] : evts) {
		//cerr << "t = " << t << '\n';
		int64_t r = get_best();
		//cerr << "r = " << r << ' ' << "cnd = " << t * r << '\n';
		ans = std::max(ans, int64_t(t) * r);

		if (id >= 0) {
			int i = id;
			//cerr << "erase vert " << i << '\n';

			num_verts--;

			int p = get_par(i);

			if (cur_deg[p] == 1) {
				assert(!comp_verts[p].empty());
				auto it = *comp_verts[p].rbegin();
				unmark_leaf(it);
			}

			comp_verts[p].erase({B[i], i});
			if (small_verts.count({B[i], i})) {
				small_verts.erase({B[i], i});
			} else {
				big_verts.erase({B[i], i});
				big_verts_tot -= B[i];
			}

			if (cur_deg[p] == 1) {
				assert(!comp_verts[p].empty());
				auto it = *comp_verts[p].rbegin();
				mark_leaf(it);
			}
		} else {
			int e = ~id;
			//cerr << "erase edge " << e << '\n';
			num_edges--;
			merge(edges[e][0], edges[e][1]);
			if (small_edges.count({D[e], e})) {
				small_edges.erase({D[e], e});
			} else {
				big_edges.erase({D[e], e});
				big_edges_tot -= D[e];
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
