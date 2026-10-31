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

	const int64_t INF = 1e10;

	int N; std::array<int, 2> E; int64_t K; cin >> N >> E[0] >> E[1] >> K;
	std::vector<std::array<int, 2>> coords(N, {-1, -1});
	std::array<std::vector<int>, 2> weights;
	struct evt_t {
		int64_t w;
		int vert;
		int64_t pair_cnt;
	};
	std::array<std::vector<evt_t>, 2> evts;
	for (int z = 0; z < 2; z++) {
		std::vector<std::pair<int64_t, std::array<int, 2>>> edges;
		edges.reserve(E[z] + N-1);
		edges.resize(E[z]);
		for (auto& e : edges) {
			int u, v; cin >> u >> v; u--, v--;
			e.second = {u, v};
			cin >> e.first;
		}
		std::sort(edges.begin(), edges.end());
		for (int i = 0; i+1 < N; i++) {
			edges.push_back({INF, {i, i+1}});
		}

		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				a = par[a];
			}
			return a;
		};
		std::vector<std::array<int, 2>> ends(N);
		for (int i = 0; i < N; i++) ends[i] = {i, i};
		std::vector<std::pair<int, int>> nxt(N, {-1, -1});

		int64_t cur_pair_cnt = 0;
		evts[z].push_back({0, -1, cur_pair_cnt});
		for (auto [w, endpts] : edges) {
			auto [u, v] = endpts;
			u = get_par(u), v = get_par(v);
			if (u == v) continue;
			if (par[u] > par[v]) std::swap(u, v);

			cur_pair_cnt += int64_t(par[u]) * int64_t(par[v]);

			evts[z].push_back({w, ends[u][1], cur_pair_cnt});

			nxt[ends[u][1]] = {int(evts[z].size())-1, ends[v][0]};
			ends[u][1] = ends[v][1];

			par[u] += par[v];
			par[v] = u;
		}

		assert(int(evts[z].size()) == N);
		assert(cur_pair_cnt == int64_t(N) * int64_t(N-1) / 2);

		int cur = ends[get_par(0)][0];
		coords[cur][z] = 0;
		weights[z].reserve(N-1);
		for (int i = 0; i+1 < N; i++) {
			weights[z].push_back(nxt[cur].first);
			cur = nxt[cur].second;
			assert(cur != -1);
			coords[cur][z] = i+1;
		}
		assert(int(weights[z].size()) == N-1);

		for (auto& [_, v, c] : evts[z]) {
			if (v == -1) continue;
			v = coords[v][z];
		}
	}

	int64_t ans = INF;

	RangeMaxQuery<int> w1_rmq(weights[1]);
	//cerr << "weights1" << '\n'; for (int w : weights[1]) { cerr << w << '\n'; }
	auto get_w1 = [&](int i, int j) {
		assert(i <= j);
		return w1_rmq.query(i, j-1);
	};

	struct treap_node {
		std::mt19937::result_type pri;
		std::array<treap_node*, 2> c{nullptr, nullptr};
		int own_y;
		int first_y;
		int last_y;
		int64_t num_pairs;
		int pref_cnt;
		int suff_cnt;
		bool is_empty;
		int max_break;
	};

	int64_t cur_y_bound = N-1; // we allow everything <= cur_y_bound

	auto update_node = [&](treap_node* a) {
		a->first_y = a->last_y = a->own_y;
		a->num_pairs = 0;
		a->pref_cnt = a->suff_cnt = 1;
		a->is_empty = true;
		a->max_break = -1;

		if (a->c[0]) {
			int w = get_w1(a->c[0]->last_y, a->first_y);
			//cerr << "checking " << a->c[0]->last_y << ' ' << a->own_y << ' ' << "got " << w << ' ' << cur_y_bound << '\n';
			if (w > cur_y_bound) {
				a->is_empty = false;
				a->pref_cnt = 0;
			} else {
				a->max_break = std::max(a->max_break, w);
			}
			a->max_break = std::max(a->max_break, a->c[0]->max_break);
			a->num_pairs += int64_t(a->c[0]->suff_cnt) * int64_t(a->pref_cnt);
			a->num_pairs += a->c[0]->num_pairs;
			a->first_y = a->c[0]->first_y;
			if (a->is_empty) a->suff_cnt += a->c[0]->suff_cnt;
			if (a->c[0]->is_empty) {
				a->pref_cnt += a->c[0]->pref_cnt;
			} else {
				a->pref_cnt = a->c[0]->pref_cnt;
				a->is_empty = false;
			}
		}

		if (a->c[1]) {
			int w = get_w1(a->last_y, a->c[1]->first_y);
			//cerr << "checking " << a->own_y << ' ' << a->c[1]->first_y << ' ' << "got " << w << ' ' << cur_y_bound << '\n';
			if (w > cur_y_bound) {
				a->is_empty = false;
				a->suff_cnt = 0;
			} else {
				a->max_break = std::max(a->max_break, w);
			}
			a->max_break = std::max(a->max_break, a->c[1]->max_break);

			a->num_pairs += int64_t(a->suff_cnt) * int64_t(a->c[1]->pref_cnt);
			a->num_pairs += a->c[1]->num_pairs;
			a->last_y = a->c[1]->last_y;

			if (a->is_empty) a->pref_cnt += a->c[1]->pref_cnt;
			if (a->c[1]->is_empty) {
				a->suff_cnt += a->c[1]->suff_cnt;
			} else {
				a->suff_cnt = a->c[1]->suff_cnt;
				a->is_empty = false;
			}
		}
	};

	auto dump_treap = [&](treap_node* a) {
		return;
		std::y_combinator([&](auto self, treap_node* c) -> void {
			if (!c) return;
			self(c->c[0]);
			cerr << c->own_y << " ";
			self(c->c[1]);
		})(a);
		cerr << " value = " << a->num_pairs;
		cerr << '\n';
	};

	auto treap_split = std::y_combinator([&](auto self, treap_node* a, int y) -> std::pair<treap_node*, treap_node*> {
		if (!a) return {nullptr, nullptr};
		treap_node *l, *r;
		if (a->own_y <= y) {
			l = a;
			std::tie(a->c[1], r) = self(a->c[1], y);
		} else {
			r = a;
			std::tie(l, a->c[0]) = self(a->c[0], y);
		}
		update_node(a);
		return {l, r};
	});
	auto treap_union = std::y_combinator([&](auto self, treap_node* a, treap_node* b) -> treap_node* {
		if (!a) return b;
		if (!b) return a;

		if (a->pri > b->pri) std::swap(a, b);

		auto [l, r] = treap_split(b, a->own_y);
		assert(!l || l->last_y < a->own_y);
		assert(!r || r->first_y > a->own_y);
		a->c[0] = self(a->c[0], l);
		a->c[1] = self(a->c[1], r);
		update_node(a);
		return a;
	});

	std::vector<treap_node> nodes(N);
	std::mt19937 mt(50);

	std::vector<treap_node*> roots(N);
	for (int i = 0; i < N; i++) {
		int x = coords[i][0], y = coords[i][1];
		//cerr << x << ' ' << y << '\n';
		nodes[x].pri = mt();
		nodes[x].own_y = y;
		nodes[x].c = {nullptr, nullptr};
		update_node(&nodes[x]);
		roots[x] = &nodes[x];
		//cerr << roots[x]->own_y << '\n';
	}
	std::vector<int> nxt(N);
	std::vector<int> prv(N);
	for (int i = 0; i < N; i++) {
		nxt[i] = i+1;
		prv[i] = i-1;
	}

	std::set<std::pair<int, int>> breaks;
	for (int i = 0; i < N; i++) {
		breaks.insert({-roots[i]->max_break, i});
	}

	/*
	cerr << "coords" << '\n';
	for (int i = 0; i < N; i++) {
		cerr << coords[i][0] << ' ' << coords[i][1] << '\n';
	}
	for (int z = 0; z < 2; z++) {
		cerr << "events " << z << '\n';
		for (int i = 0; i < N; i++) {
			cerr << evts[z][i].w << ' ' << evts[z][i].vert << ' ' << evts[z][i].pair_cnt << '\n';
		}
	}
	*/

	int64_t doubled_pair_cnt = 0;
	for (int i = 0, j = N-1; true; ) {
		int64_t cur_pair_cnt = evts[0][i].pair_cnt + evts[1][j].pair_cnt - doubled_pair_cnt;
		//cerr << "got " << i << ' ' << evts[0][i].w << ' ' << j << ' ' << evts[1][j].w << ' ' << cur_pair_cnt << ' ' << doubled_pair_cnt << '\n';
		if (cur_pair_cnt >= K) {
			ans = std::min(ans, evts[0][i].w + evts[1][j].w);

			// decrement j
			if (j == 0) break;
			// split things apart at idx .. idx+1
			int idx = evts[1][j].vert;
			j--;
			cur_y_bound = j;

			while (!breaks.empty() && -breaks.begin()->first > j) {
				int x = breaks.begin()->second;
				breaks.erase(breaks.begin());
				assert(roots[x]->max_break == j+1);
				doubled_pair_cnt -= roots[x]->num_pairs;
				//cerr << "split" << j << ' ' << idx << '\n';
				dump_treap(roots[x]);
				auto [l, r] = treap_split(roots[x], idx);
				assert(!l || l->last_y <= idx);
				assert(!r || r->first_y > idx);
				roots[x] = treap_union(l, r);
				dump_treap(roots[x]);
				//cerr << roots[x]->max_break << '\n';
				assert(roots[x]->max_break <= j);

				doubled_pair_cnt += roots[x]->num_pairs;
				breaks.insert({-roots[x]->max_break, x});
			}
		} else {
			// increment i
			if (i == N-1) break;
			i++;
			// merge things together at idx .. idx+1
			int idx = evts[0][i].vert;
			assert(nxt[idx] != N);
			int nidx = nxt[idx];

			assert(roots[idx] && roots[nidx]);

			prv[nidx] = prv[idx];
			if (prv[nidx] != -1) nxt[prv[nidx]] = nidx;

			//cerr << "merge " << idx << " into " << nidx << '\n';

			doubled_pair_cnt -= roots[idx]->num_pairs;
			doubled_pair_cnt -= roots[nidx]->num_pairs;
			breaks.erase({-roots[idx]->max_break, idx});
			breaks.erase({-roots[nidx]->max_break, nidx});

			dump_treap(roots[idx]);
			dump_treap(roots[nidx]);
			roots[nidx] = treap_union(roots[idx], roots[nidx]);
			roots[idx] = nullptr;
			dump_treap(roots[nidx]);

			breaks.insert({-roots[nidx]->max_break, nidx});
			doubled_pair_cnt += roots[nidx]->num_pairs;
		}
	}

	if (ans == INF) {
		cout << -1 << '\n';
	} else {
		assert(ans <= int(2e9));
		cout << ans << '\n';
	}

	return 0;
}
