#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) {
	if (!b) {
		cout << "GARBAGE" << '\n';
		exit(0);
	}
}

mt19937 mt(48);

struct node {
	node *l = nullptr, *r = nullptr;
	mt19937::result_type pri;
	int i, c;
};
int NODE_POOL_SZ = 0;
const int NODE_POOL_TOT = int(15e6);
node node_pool[NODE_POOL_TOT];
node* make_node() {
	wssert(NODE_POOL_SZ < NODE_POOL_TOT);
	return &node_pool[NODE_POOL_SZ++];
}
node* clone_node(node* a) {
	wssert(a);
	node* r = make_node();
	*r = *a;
	return r;
}

node* merge(node* a, node* b) {
	if (!a) return b;
	if (!b) return a;
	node* r;
	if (a->pri < b->pri) {
		r = clone_node(a);
		r->r = merge(a->r, b);
	} else {
		r = clone_node(b);
		r->l = merge(a, b->l);
	}
	return r;
}

template <typename F> pair<node*, node*> split(node* a, const F& isRight) {
	if (!a) return {nullptr, nullptr};
	if (isRight(a)) {
		node* o;
		node* r = clone_node(a);

		tie(o, r->l) = split(a->l, isRight);
		return {o, r};
	} else {
		node* r = clone_node(a);
		node* o;
		tie(r->r, o) = split(a->r, isRight);
		return {r, o};
	}
}

pair<node*, node*> splitInd(node* a, int idx) {
	// indexes should all be distinct
	return split(a, [&](node* x) -> bool { return x->i > idx; });
}

pair<node*, node*> splitVal(node* a, int c) {
	return split(a, [&](node* x) -> bool { return x->c <= c; });
}

node* leftMost(node* a) {
	wssert(a);
	while (a->l) a = a->l;
	return a;
}
node* rightMost(node* a) {
	wssert(a);
	while (a->r) a = a->r;
	return a;
}

pair<node*, node*> splice(node* chain, node* toInsert) {
	wssert(toInsert);
	wssert(chain);
	node* hi_chain; node* lo_chain;
	tie(hi_chain, lo_chain) = splitInd(chain, leftMost(toInsert)->i);
	wssert(hi_chain == nullptr || rightMost(hi_chain)->c >= leftMost(toInsert)->c);
	node* mid_chain;
	tie(mid_chain, lo_chain) = splitVal(lo_chain, rightMost(toInsert)->c);
	wssert(lo_chain == nullptr || leftMost(lo_chain)->i > rightMost(toInsert)->i);

	return {merge(merge(hi_chain, toInsert), lo_chain), mid_chain};
}

ostream& operator << (ostream& o, node* n) {
	if (!n) return o;
	return o << n->l << "(" << n->i << "," << n->c << ")" << ' ' << n->r;
}

const int MAXN = int(1e5) + 20;
const int MAXQ = int(1e5) + 20;
int V[MAXN];
int Vstart[MAXN];
vector<pair<int, int>> ops[1<<18];

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
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		NODE_POOL_SZ = 0;
		int N, K; cin >> N >> K;
		for (int i = 0; i < N; i++) {
			cin >> V[i];
			Vstart[i] = 0;
		}
		int Q; cin >> Q;
		int S = 1;
		while (S <= Q) S *= 2;
		for (int q = 0; q < Q; q++) {
			int i, c; cin >> i >> c; i--;
			for (int a = S+Vstart[i], b = S+q; a < b; a /= 2, b /= 2) {
				if (a & 1) ops[a++].emplace_back(i, V[i]);
				if (b & 1) ops[--b].emplace_back(i, V[i]);
			}
			V[i] = c;
			Vstart[i] = q;
		}

		for (int i = 0; i < N; i++) {
			for (int a = S+Vstart[i], b = S+Q; a < b; a /= 2, b /= 2) {
				if (a & 1) ops[a++].emplace_back(i, V[i]);
				if (b & 1) ops[--b].emplace_back(i, V[i]);
			}
		}

		std::y_combinator([&](auto self, int s, vector<node*> v) -> void {
			int OLD_NODE_POOL_SZ = NODE_POOL_SZ;
			//cerr << s << '\n';
			//for (node* n : v) { cerr << n << '\n'; }
			for (auto it : ops[s]) {
				if (int(v.size()) >= K) {
					break;
				}
				int i = it.first, c = it.second;
				//cerr << "insert " << i << ' ' << c << '\n';

				int k = 0;
				for (; k < int(v.size()); k++) {
					node* best = nullptr;
					node* cur = v[k];
					while (cur) {
						if (i < cur->i) {
							cur = cur->l;
						} else {
							best = cur;
							cur = cur->r;
						}
					}
					if (best == nullptr || best->c >= c) {
						break;
					}
				}

				node* n = make_node();
				n->l = n->r = nullptr;
				n->pri = mt();
				n->i = i, n->c = c;

				for (; n; k++) {
					if (k == int(v.size())) {
						v.push_back(n);
						n = nullptr;
					} else {
						tie(v[k], n) = splice(v[k], n);
					}
				}
			}
			//for (node* n : v) { cerr << n << '\n'; }
			if (s >= S) {
				if (0 <= s-S && s-S < Q) {
					cout << v.size() << '\n';
				} else {
					wssert(v.empty());
				}
			} else {
				self(2*s, v);
				self(2*s+1, v);
			}
			NODE_POOL_SZ = OLD_NODE_POOL_SZ;
		})(1, vector<node*>());

		for (int i = 0; i < 2*S; i++) ops[i] = {};
	}

	return 0;
}
