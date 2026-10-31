#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri;
	std::pair<int, int> key;
	int sz;
	std::array<int, 2> own_vals;
	std::array<int, 2> best_vals;
	void update() {
		sz = 1;
		best_vals = own_vals;
		for (int z = 0; z < 2; z++) {
			if (!c[z]) continue;
			sz += c[z]->sz;
			for (int w = 0; w < 2; w++) {
				setmax(best_vals[w], c[z]->best_vals[w]);
			}
		}
	}
};

std::pair<treap_node*, treap_node*> split(treap_node* r, std::pair<int, int> key) {
	if (!r) return {nullptr, nullptr};
	treap_node *a, *b;
	if (r->key < key) {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], key);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], key);
	}
	r->update();
	return {a, b};
}

treap_node* treap_union(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	if (a->pri > b->pri) std::swap(a, b);
	auto [l, r] = split(b, a->key);
	a->c[0] = treap_union(a->c[0], l);
	a->c[1] = treap_union(a->c[1], r);
	a->update();
	return a;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> all_ans(N);
		std::vector<treap_node> nodes(N);
		[&](this auto&& self, int cur, int prv, std::array<int, 2> val) -> treap_node* {
			auto sub_val = val;
			val[S[cur] - '0']++;
			for (int z = 0; z < 2; z++) {
				sub_val[z] += val[z];
				sub_val[z] = -sub_val[z];
			}
			treap_node* rt;
			{
				treap_node* n = &nodes[cur];
				n->c = {nullptr, nullptr};
				n->pri = mt();
				n->key = {val[1] - val[0], cur};
				n->own_vals[0] = 3 * val[1] - val[0]; // when 0 is bigger
				n->own_vals[1] = 3 * val[0] - val[1]; // when 1 is bigger
				n->update();
				rt = n;
			}
			int ans = -4*N;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				treap_node* o = self(nxt, cur, val);
				assert(rt);
				assert(o);
				if (o->sz > rt->sz) std::swap(o, rt);
				[&](this auto&& dfs, treap_node* a) -> void {
					if (!a) return;
					dfs(a->c[0]);
					dfs(a->c[1]);
					int cur_key = a->key.first + sub_val[1] - sub_val[0];
					std::array<int, 2> vals;
					vals[0] = a->own_vals[0] + 3 * sub_val[1] - sub_val[0];
					vals[1] = a->own_vals[1] + 3 * sub_val[0] - sub_val[1];

					treap_node* b = rt;
					while (b) {
						if (!b) return;
						int d = cur_key + b->key.first >= 0;
						setmax(ans, vals[d] + b->own_vals[d]);
						if (b->c[d]) {
							setmax(ans, vals[d] + b->c[d]->best_vals[d]);
						}
						b = b->c[!d];
					}
				}(o);
				rt = treap_union(rt, o);
			}
			all_ans[cur] = ans;
			return rt;
		}(0, -1, {0, 0});
		for (int i = 0; i < N; i++) {
			cout << all_ans[i] << '\n';
		}
	}

	return 0;
}
