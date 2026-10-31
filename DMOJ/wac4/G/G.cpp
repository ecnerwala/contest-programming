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

template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

struct lct_node {
	lct_node* p = nullptr;
	lct_node* c[2] = {nullptr, nullptr};
	bool flip = false;
	int val = 0;
	int min_val = 0;

	bool r() { return !(p && (this == p->c[0] || this == p->c[1])); }
	int d() { assert(p); return this == p->c[1]; }

	void do_flip() {
		flip ^= 1;
	}

	void downdate() {
		if (flip) {
			if (c[0]) c[0]->do_flip();
			if (c[1]) c[1]->do_flip();
			swap(c[0], c[1]);
			flip = false;
		}
	}

	void downdate_all() {
		if (!r()) p->downdate_all();
		downdate();
	}

	void update() {
		min_val = val;
		if (c[0]) setmin(min_val, c[0]->min_val);
		if (c[1]) setmin(min_val, c[1]->min_val);
	}

	void rot() {
		assert(!flip);

		assert(p);
		assert(!r());
		lct_node* pa = p;
		int z = d();
		lct_node* ch = c[!z];

		if (!pa->r()) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		pa->c[z] = ch;
		if (ch) ch->p = pa;

		pa->p = this;
		this->c[!z] = pa;

		pa->update();
	}

	void splay() {
		downdate_all();
		while (!r()) {
			if (!p->r()) {
				if (p->d() == d()) {
					p->rot();
				} else {
					rot();
				}
			}
			rot();
		}
		update();
		assert(!flip);
	}

	void expose() {
		splay();
		while (p) {
			p->splay();
			assert(!p->flip);
			assert(p->r());
			p->c[1] = this;
			rot(); // updates p
		}
		assert(!flip);
		c[1] = nullptr;
		update();
	}

	void make_root() {
		expose();
		do_flip();
		downdate();
	}

	lct_node* find_min() {
		if (val == min_val) return this;
		if (c[0] && c[0]->min_val == min_val) return c[0]->find_min();
		if (c[1] && c[1]->min_val == min_val) return c[1]->find_min();
		assert(false);
	}
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, M, Q; cin >> N >> M >> Q;
	vector<pair<pair<int, int>, pair<int, int>>> edges(M);
	for (int e = 0; e < M; e++) {
		int a, b, d, t; cin >> a >> b >> d >> t; a--, b--;
		edges[e] = {{t, d}, {a,b}};
	}
	vector<pair<pair<int, int>, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		int d, t; cin >> d >> t;
		queries[q] = {{t, d}, q};
	}

	sort(edges.begin(), edges.end());
	sort(queries.begin(), queries.end());

	vector<bool> ans(Q);

	multiset<int> durabilities;
	vector<lct_node> node_pool(N+(N-1)+M);
	const int INF = int(1e9)+200;
	for (int i = 0; i < N; i++) {
		node_pool[i].val = INF+1;
		node_pool[i].update();
	}

	for (int i = 0; i < N-1; i++) {
		node_pool[N+i].val = -1;
		node_pool[N+i].update();

		durabilities.insert(node_pool[N+i].val);

		node_pool[i].p = &node_pool[N+i];
		node_pool[N+i].p = &node_pool[i+1];
	}

	auto print_vert = [&](lct_node* a) {
		if (!a) {
			cerr << "nullptr";
			return;
		} else {
			int i = int(a - node_pool.data());
			if (0 <= i && i < N) {
				cerr << "v" << i;
			} else if (N <= i && i < N+(N-1)) {
				cerr << "e-init-" << (i-N) << '-' << (i-N+1);
			} else {
				cerr << "e" << i-(N+(N-1));
			}
		}
	};
	auto dump_tree = std::y_combinator([&](auto self, lct_node* l) -> void {
		if (!l) return;
		self(l->c[l->flip ? 1 : 0]);
		print_vert(l); cerr << '\n';
		self(l->c[l->flip ? 0 : 1]);
	});
	int curE = 0;
	for (int z = 0; z < Q; z++) {
		while (curE < M && edges[curE].first.first <= queries[z].first.first) {
			// insert this edge
			int d = edges[curE].first.second;
			int a, b; tie(a,b) = edges[curE].second;
			//cerr << "verts " << a << ' ' << b << ' ' << d << '\n';

			lct_node* n = &node_pool[N+(N-1)+curE];
			n->val = d; n->update();

			lct_node* na = &node_pool[a];
			lct_node* nb = &node_pool[b];

			na->make_root();
			assert(!na->p);
			//cerr << "na" << '\n'; dump_tree(na);

			nb->make_root();
			//cerr << "nb" << '\n'; dump_tree(nb);

			if (n->val > nb->min_val) {
				lct_node* o = nb->find_min();
				o->splay();
				//cerr << "o" << '\n'; dump_tree(o);
				assert(!o->p);
				assert(o->c[0] && o->c[1]);
				o->c[0]->p = o->c[1]->p = nullptr;
				o->c[0] = o->c[1] = nullptr;

				durabilities.erase(durabilities.find(o->val));

				na->make_root();
				assert(!na->p);
				nb->make_root();
				assert(!nb->p);
				assert(!na->p);

				na->p = nb->p = n;
				durabilities.insert(n->val);
			}

			//cerr << curE << ": " << '\n'; for (int d : durabilities) { cerr << d << " "; } cerr << '\n';

			curE++;
		}

		bool res = *durabilities.begin() >= queries[z].first.second;
		ans[queries[z].second] = res;
	}

	for (int q = 0; q < Q; q++) {
		cout << (ans[q] ? "YES" : "NO") << '\n';
	}

	return 0;
}
