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

node* merge(node* a, node* b) {
	if (!a) return b;
	if (!b) return a;
	node* r;
	if (a->pri < b->pri) {
		r = a;
		r->r = merge(a->r, b);
	} else {
		r = b;
		r->l = merge(a, b->l);
	}
	return r;
}

template <typename F> pair<node*, node*> split(node* a, const F& isRight) {
	if (!a) return {nullptr, nullptr};
	if (isRight(a)) {
		node* o;
		node* r = a;

		tie(o, r->l) = split(a->l, isRight);
		return {o, r};
	} else {
		node* r = a;
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
		int N, K; cin >> N >> K;
		vector<int> V(N);
		for (int i = 0; i < N; i++) {
			cin >> V[i];
		}

		vector<node> node_pool(N);

		vector<node*> v;

		auto insertNode = [&](int i, int c) {
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

			node* n = &node_pool[i];
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
		};

		auto deleteNode = [&](int i) {
			int k = 0;
			for (; true; k++) {
				assert(k < int(v.size()));
				node* cur = v[k];
				while (cur) {
					if (i == cur->i) {
						break;
					} else if (i < cur->i) {
						cur = cur->l;
					} else if (i > cur->i) {
						cur = cur->r;
					} else assert(false);
				}
				if (cur) {
					assert(cur->i == i);
					break;
				}
			}
			node* hi_chain; node* lo_chain;
			tie(hi_chain, lo_chain) = splitInd(v[k], i);
			node* drop;
			tie(hi_chain, drop) = splitInd(hi_chain, i-1);
			assert(drop == &node_pool[i]);

			for (; k+1 < int(v.size()); k++) {
				node* n_hi_chain = nullptr;
				node* mid_chain = v[k+1];
				node* n_lo_chain = nullptr;

				if (hi_chain) {
					tie(n_hi_chain, mid_chain) = splitVal(mid_chain, rightMost(hi_chain)->c);
				}
				if (lo_chain) {
					tie(mid_chain, n_lo_chain) = splitInd(mid_chain, leftMost(lo_chain)->i);
				}

				v[k] = merge(merge(hi_chain, mid_chain), lo_chain);
				hi_chain = n_hi_chain;
				lo_chain = n_lo_chain;
			}

			assert(k == int(v.size())-1);
			if (hi_chain || lo_chain) {
				v[k] = merge(hi_chain, lo_chain);
			} else {
				v.pop_back();
			}
		};

		vector<bool> isLazy(N, true);
		vector<int> lazyInsert(N); iota(lazyInsert.begin(), lazyInsert.end(), 0);

		auto fixNodes = [&]() {
			while (int(v.size()) < K && !lazyInsert.empty()) {
				int i = lazyInsert.back(); lazyInsert.pop_back();
				assert(isLazy[i]);
				isLazy[i] = false;
				insertNode(i, V[i]);
			}
		};
		fixNodes();

		int Q; cin >> Q;
		for (int q = 0; q < Q; q++) {
			int i, c; cin >> i >> c; i--;
			if (!isLazy[i]) {
				deleteNode(i);
				isLazy[i] = true;
				lazyInsert.push_back(i);
			}
			V[i] = c;
			fixNodes();
			cout << v.size() << '\n';
		}
	}

	return 0;
}
