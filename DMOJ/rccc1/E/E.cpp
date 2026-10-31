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

const int L = 18;
const int S = 1 << L;

struct node {
	std::array<node*, 2> c{nullptr, nullptr};
	int lazy_xor = 0;
	int tot_xor = 0;
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	auto apply_lazy = [&](node* n, int x) {
		if (!n) return;
		n->lazy_xor ^= x;
		if (n->tot_xor & S) n->tot_xor ^= x;
	};

	auto update = [&](node* n) {
		if (!n) return;
		n->tot_xor = 0;
		for (int z = 0; z < 2; z++) {
			if (n->c[z]) {
				n->tot_xor ^= n->c[z]->tot_xor;
			}
		}
		if (n->tot_xor & S) n->tot_xor ^= n->lazy_xor;
	};

	auto downdate = [&](node* n, int level) {
		if (!n) return;
		if (!n->lazy_xor) return;
		if (level > 0 && ((n->lazy_xor >> (level-1)) & 1)) swap(n->c[0], n->c[1]);
		apply_lazy(n->c[0], n->lazy_xor);
		apply_lazy(n->c[1], n->lazy_xor);
		n->lazy_xor = 0;
	};

	node* ROOT = nullptr;

	auto insert = [&](int x) -> void {
		node** cur_ptr = &ROOT;
		for (int level = L; level >= 0; level--) {
			node*& cur = *cur_ptr;
			if (!cur) {
				cur = new node;
			}
			downdate(cur, level);
			cur->tot_xor ^= S ^ x;
			if (!level) break;
			cur_ptr = &cur->c[(x >> (level-1)) & 1];
		}
	};
	auto do_xor = [&](int l, int r, int x) -> void {
		node* to_flip = nullptr;
		std::tie(ROOT, to_flip) = std::y_combinator([&](auto self, node* cur, int lo, int hi, int level) -> std::pair<node*, node*> {
			if (!cur) return {nullptr, nullptr};
			if (hi <= l || r <= lo) return {cur, nullptr};
			if (l <= lo && hi <= r) return {nullptr, cur};
			downdate(cur, level);
			level--;
			int md = (lo + hi) / 2;
			auto r0 = self(cur->c[0], lo, md, level);
			auto r1 = self(cur->c[1], md, hi, level);

			if (!r0.first && !r1.first) {
				return {nullptr, cur};
			}
			if (!r0.second && !r1.second) {
				return {cur, nullptr};
			}

			cur->c[0] = r0.first;
			cur->c[1] = r1.first;
			update(cur);

			node* oth = new node;
			oth->c[0] = r0.second;
			oth->c[1] = r1.second;
			update(oth);

			return {cur, oth};
		})(ROOT, 0, S, L);
		apply_lazy(to_flip, x);
		ROOT = std::y_combinator([&](auto self, node* a, node* b, int level) -> node* {
			if (!b) return a;
			if (!a) return b;
			downdate(a, level);
			downdate(b, level);

			if (level == 0) {
				a->tot_xor ^= b->tot_xor;
			} else {
				level--;
				a->c[0] = self(a->c[0], b->c[0], level);
				a->c[1] = self(a->c[1], b->c[1], level);
				update(a);
			}

			delete b;
			return a;
		})(ROOT, to_flip, L);
	};

	auto do_query = [&](int l, int r) -> int {
		int res = 0;
		std::y_combinator([&](auto self, node* cur, int lo, int hi, int level) -> void {
			if (!cur) return;
			if (hi <= l || r <= lo) return;
			if (l <= lo && hi <= r) {
				res ^= cur->tot_xor;
				return;
			}
			downdate(cur, level);
			level--;
			int md = (lo + hi) / 2;
			self(cur->c[0], lo, md, level);
			self(cur->c[1], md, hi, level);
		})(ROOT, 0, S, L);
		return res & (S-1);
	};
	while (N--) {
		int x; cin >> x;
		insert(x);
	}
	while (Q--) {
		int t; cin >> t;
		if (t == 1) {
			int x; cin >> x;
			insert(x);
		} else if (t == 2) {
			int l, r; cin >> l >> r; r++;
			int x; cin >> x;
			do_xor(l, r, x);
		} else if (t == 3) {
			int l, r; cin >> l >> r; r++;
			cout << do_query(l, r) << '\n';
		}
	}

	ROOT = nullptr;
	std::y_combinator([&](auto self, node* cur) -> void {
		if (!cur) return;
		self(cur->c[0]);
		self(cur->c[1]);
		delete cur;
	})(ROOT);

	return 0;
}
