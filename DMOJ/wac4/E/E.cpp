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

using i64 = int64_t;
struct heap_node {
	heap_node* c[2] = {nullptr, nullptr};
	i64 val = 0; // relative to parent
};

mt19937 mt(48);

heap_node* merge(heap_node* a, i64 da, heap_node* b, i64 db) {
	if (!a && !b) {
		return nullptr;
	} else if (!a) {
		if (db) {
			b = new heap_node(*b);
			b->val += db;
		}
		return b;
	} else if (!b) {
		if (da) {
			a = new heap_node(*a);
			a->val += da;
		}
		return a;
	} else {
		i64 va = a->val + da;
		i64 vb = b->val + db;
		if (va < vb) {
			swap(a,b);
			swap(da,db);
			swap(va,vb);
		}
		assert(va >= vb);

		a = new heap_node(*a);
		a->val += da;

		int z = mt() & 1;
		a->c[z] = merge(a->c[z], 0, b, db - va);

		assert(a->c[z]->val <= 0);

		return a;
	}
}

heap_node* merge(heap_node* a, heap_node* b) {
	return merge(a, 0, b, 0);
}

heap_node* incr(heap_node* a, i64 c) {
	assert(a);
	a = new heap_node(*a);
	a->val += c;
	return a;
}

pair<heap_node*, i64> pop_ins(heap_node* a) {
	assert(a);
	i64 v = a->val;

	heap_node* n = new heap_node();
	n->val = 0;

	return {merge(merge(n, 0, a->c[0], v), 0, a->c[1], v), v};
}

struct seg_node {
	seg_node* c[2] = {nullptr, nullptr};
	int par;
	heap_node* hp;
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

	int N, Q; cin >> N >> Q;
	vector<int> Z(N);
	for (int i = 0; i < N; i++) cin >> Z[i];

	vector<seg_node*> state(Q+1);
	state[0] = std::y_combinator([&](auto self, int l, int r) -> seg_node* {
		assert(l < r);
		seg_node* res = new seg_node();
		if (r - l == 1) {
			res->hp = new heap_node();
			res->hp->val = Z[l];
			res->par = -1;
		} else {
			int m = (l+r) / 2;
			res->c[0] = self(l, m);
			res->c[1] = self(m, r);
		}
		return res;
	})(0, N);

	auto query_node = [&](seg_node* rt, int ind) -> seg_node* {
		return std::y_combinator([&](auto self, seg_node* cur, int l, int r) -> seg_node* {
			assert(l <= ind && ind < r);
			if (r - l == 1) {
				return cur;
			} else {
				int m = (l+r) / 2;
				if (ind < m) return self(cur->c[0], l, m);
				else return self(cur->c[1], m, r);
			}
		})(rt, 0, N);
	};

	auto update_node = [&](seg_node* rt, int ind, seg_node* newLeaf) -> seg_node* {
		return std::y_combinator([&](auto self, seg_node* cur, int l, int r) -> seg_node* {
			assert(l <= ind && ind < r);
			if (r - l == 1) {
				return newLeaf;
			} else {
				seg_node* nxt = new seg_node(*cur);
				int m = (l+r) / 2;
				if (ind < m) nxt->c[0] = self(cur->c[0], l, m);
				else nxt->c[1] = self(cur->c[1], m, r);
				return nxt;
			}
		})(rt, 0, N);
	};

	auto getPar = [&](seg_node* rt, int a) -> pair<int, seg_node*> {
		while (true) {
			seg_node* cur = query_node(rt, a);
			if (cur->par >= 0) {
				a = cur->par;
			} else {
				return {a, cur};
			}
		}
	};

	i64 lastAns = 0;
	auto readInt = [&]() -> int {
		i64 v; cin >> v;
		v ^= lastAns;
		return int(v);
	};
	for (int q = 1; q <= Q; q++) {
		int op; cin >> op;
		int j = readInt();
		seg_node* rt = state[j];

		if (op == 1) {
			int a = readInt(); a--;
			int b = readInt(); b--;
			seg_node* la; tie(a, la) = getPar(rt, a);
			seg_node* lb; tie(b, lb) = getPar(rt, b);
			if (a != b) {
				if (lb->par < la->par) {
					swap(a, b);
					swap(la, lb);
				}
				la = new seg_node(*la);
				lb = new seg_node(*lb);
				la->par += lb->par;
				lb->par = a;
				la->hp = merge(la->hp, lb->hp);
				lb->hp = nullptr;
				rt = update_node(rt, a, la);
				rt = update_node(rt, b, lb);
			}
		} else if (op == 2) {
			int a = readInt(); a--;

			seg_node* la; tie(a, la) = getPar(rt, a);

			la = new seg_node(*la);
			i64 v;
			tie(la->hp, v) = pop_ins(la->hp);
			cout << v << '\n';
			lastAns = v;

			rt = update_node(rt, a, la);
		} else if (op == 3) {
			int a = readInt(); a--;
			int c = readInt();

			seg_node* la; tie(a, la) = getPar(rt, a);

			la = new seg_node(*la);
			la->hp = incr(la->hp, c);
			rt = update_node(rt, a, la);
		}

		state[q] = rt;
	}

	return 0;
}
