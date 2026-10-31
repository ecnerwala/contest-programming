#include <bits/stdc++.h>

template <typename T> int signum(T a) {
	return (a > 0) - (a < 0);
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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;

	std::mt19937_64 mt(48);
	struct treap_node {
		std::array<treap_node*, 2> c{nullptr, nullptr};
		std::mt19937_64::result_type pri;
		int own_val;
		int tot_val;
		int max_pref;

		int64_t t0;

		int64_t t_lo;
		int64_t t_hi;

		int64_t nxt_x;
	};

	const int64_t INF = 1e18;

	auto update_node = [](treap_node* n) -> void {
		assert(n);
		n->tot_val = n->own_val;
		n->max_pref = std::max(0, n->own_val);

		assert(n->own_val != 0);

		n->nxt_x = INF;
		n->t_lo = n->own_val > 0 ? n->t0 : INF;
		n->t_hi = n->own_val < 0 ? n->t0 : -INF;

		if (n->c[0]) {
			n->max_pref = std::max(n->c[0]->max_pref, n->c[0]->tot_val + n->max_pref);
			n->tot_val += n->c[0]->tot_val;

			n->nxt_x = std::min(n->nxt_x, n->c[0]->nxt_x);
			if (n->c[0]->t_hi != -INF && n->t_lo != INF) {
				n->nxt_x = std::min(n->nxt_x, (n->t_lo - n->c[0]->t_hi) / 2);
			}
			n->t_lo = n->c[0]->t_lo;
		}
		if (n->c[1]) {
			n->max_pref = std::max(n->max_pref, n->tot_val + n->c[1]->max_pref);
			n->tot_val += n->c[1]->tot_val;

			n->nxt_x = std::min(n->nxt_x, n->c[1]->nxt_x);
			if (n->t_hi != -INF && n->c[1]->t_lo != INF) {
				n->nxt_x = std::min(n->nxt_x, (n->c[1]->t_lo - n->t_hi) / 2);
			}
			n->t_hi = n->c[1]->t_hi;
		}
	};

	auto do_merge = std::y_combinator([&](auto self, treap_node* a, treap_node* b) -> treap_node* {
		if (!b) return a;
		if (!a) return b;
		treap_node* r;
		if (a->pri < b->pri) {
			r = a;
			r->c[1] = self(a->c[1], b);
		} else {
			r = b;
			r->c[0] = self(a, b->c[0]);
		}
		update_node(r);
		return r;
	});

	auto prune_side = std::y_combinator([&](auto self, treap_node *a, bool dir, int cnt) -> treap_node* {
		assert(a);
		if (a->c[dir]) {
			a->c[dir] = self(a->c[dir], dir, cnt);
			update_node(a);
			return a;
		} else {
			assert((dir ? -a->own_val : a->own_val) >= cnt);
			a->own_val -= dir ? -cnt : cnt;
			if (a->own_val == 0) {
				treap_node* r = a->c[!dir];
				a->c[!dir] = nullptr;
				return r;
			} else {
				update_node(a);
				return a;
			}
		}
	});

	auto dump_node = std::y_combinator([&](auto self, treap_node* r, int64_t x, std::string prefix = "") -> void {
		if (!r) return;
		self(r->c[0], x, prefix + " ");
		cerr << prefix << "own=" << r->own_val << " tot=" << r->tot_val << " pref=" << r->max_pref << " t0=" << r->t0 << " t=" << (r->own_val > 0 ? r->t0 - x : r->t0 + x) << '\n';
		self(r->c[1], x, prefix + " ");
	});

	auto update_to = std::y_combinator([&](auto self, treap_node* a, int64_t x) -> treap_node* {
		if (!a) return nullptr;
		if (a->nxt_x >= x) return a;

		a->c[0] = self(a->c[0], x);
		a->c[1] = self(a->c[1], x);

		treap_node *lo, *hi;
		if (a->own_val > 0) {
			lo = a->c[0];
			a->c[0] = nullptr;
			hi = a;
		} else if (a->own_val < 0) {
			hi = a->c[1];
			a->c[1] = nullptr;
			lo = a;
		} else assert(false);
		update_node(a);

		// now, clear lo and hi as long as we can
		while (lo && hi) {
			assert(lo->nxt_x >= x);
			assert(hi->nxt_x >= x);
			if (lo->t_hi == -INF) break;
			if (hi->t_lo == INF) break;
			if (lo->t_hi + x <= hi->t_lo - x) break;
			// now, peel off the first and last
			int lo_cnt;
			{
				treap_node* cur = lo;
				while (cur->c[1]) cur = cur->c[1];
				lo_cnt = -cur->own_val;
			}
			int hi_cnt;
			{
				treap_node* cur = hi;
				while (cur->c[0]) cur = cur->c[0];
				hi_cnt = cur->own_val;
			}

			int rem_cnt = std::min(lo_cnt, hi_cnt);
			assert(rem_cnt > 0);
			//cerr << -lo_cnt << ' ' << hi_cnt << " -> " << -(lo_cnt - rem_cnt) << ' ' << hi_cnt - rem_cnt << '\n';

			//dump_node(lo, x);
			//cerr << '\n';
			//dump_node(hi, x);
			//cerr << '\n';

			lo = prune_side(lo, 1, rem_cnt);
			hi = prune_side(hi, 0, rem_cnt);

			//dump_node(lo, x);
			//cerr << '\n';
			//dump_node(hi, x);
			//cerr << '\n';
		}
		return do_merge(lo, hi);
	});

	auto split = std::y_combinator([&](auto self, treap_node* root, auto&& should_left) -> std::pair<treap_node*, treap_node*> {
		if (!root) return {nullptr, nullptr};
		treap_node *a, *b;
		if (should_left(root)) {
			a = root;
			std::tie(a->c[1], b) = self(root->c[1], should_left);
		} else {
			b = root;
			std::tie(a, b->c[0]) = self(root->c[0], should_left);
		}
		update_node(root);
		return {a, b};
	});

	auto insert_node = std::y_combinator([&](auto self, treap_node* root, treap_node* n, auto should_left) -> treap_node* {
		if (root && root->pri < n->pri) {
			if (should_left(root)) {
				root->c[1] = self(root->c[1], n, should_left);
			} else {
				root->c[0] = self(root->c[0], n, should_left);
			}
			update_node(root);
			return root;
		} else {
			std::tie(n->c[0], n->c[1]) = split(root, should_left);
			update_node(n);
			return n;
		}
	});

	int64_t tot_blue = 0;
	treap_node* root = nullptr;
	std::vector<treap_node> nodes(Q);
	for (int q = 0; q < Q; q++) {
		int64_t x, t; int c; cin >> x >> t >> c;
		assert(c != 0);
		if (c > 0) {
			tot_blue += c;
		}

		//cerr << "move to " << x << '\n';
		root = update_to(root, x);

		//dump_node(root, x);
		//cerr << '\n';

		//cerr << "insert " << x << ' ' << t << ' ' << c << '\n';

		treap_node* cur = &nodes[q];
		cur->pri = mt();
		cur->c = {nullptr, nullptr};
		cur->own_val = c;
		cur->t0 = (c > 0 ? t + x : t - x);
		update_node(cur);

		// insert cur
		root = insert_node(root, cur, [&](treap_node* r) -> bool {
			int64_t r_t = r->own_val > 0 ? r->t0 - x : r->t0 + x;
			return r_t < t || (r_t == t && r->own_val < 0);
		});
		//dump_node(root, x);
		//cerr << '\n';

		cout << tot_blue - (root ? root->max_pref : 0) << '\n';
	}

	return 0;
}
