#include <bits/stdc++.h>

std::mt19937_64 mt(48);

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

struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937_64::result_type pri = mt();

	int lo, hi;
	int color;
	bool is_good = false;

	~treap_node() {
		delete c[0];
		delete c[1];
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	return r;
}

template <typename F>
std::pair<treap_node*, treap_node*> split_at_point(treap_node* r, int x, F&& coord) {
	if (!r) return {nullptr, nullptr};
	if (r->color == -1 && r->lo < x && x <= r->hi) {
		// split r into two if it's rainbow, otherwise leave it intact on the rhs
		auto r2 = new treap_node;
		r2->lo = x;
		r2->hi = r->hi;
		r2->color = -1;
		r2->is_good = r->is_good;
		r->hi = x-1;
		auto b = merge(r2, std::exchange(r->c[1], nullptr));
		return {r, b};
	}
	treap_node *a, *b;
	if (coord(r) >= x) {
		b = r;
		std::tie(a, b->c[0]) = split_at_point(r->c[0], x, coord);
	} else {
		a = r;
		std::tie(a->c[1], b) = split_at_point(r->c[1], x, coord);
	}
	return {a, b};
}

void dump_treap(treap_node* n) {
	if (!n) return;
	dump_treap(n->c[0]);
	std::cerr << n->lo << '-' << n->hi << ' ' << n->color << ' ' << n->is_good << '\n';
	dump_treap(n->c[1]);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q, K; cin >> N >> Q >> K;
	std::vector<std::array<int, 2>> ops(Q);
	for (auto& [l, r] : ops) {
		cin >> l >> r; l--; r--;
	}
	std::vector<int64_t> ans(Q, -1);
	for (int split = K; split - K < Q; split += K+1) {
		//cerr << "reset split " << K << '\n';
		std::vector<std::vector<std::pair<int, int>>> deletions(K+1);

		{
			// First, simulate the RHS
			treap_node* root = new treap_node;
			root->lo = 0;
			root->hi = N-1;
			root->color = -1;

			for (int i = 0; i < K; i++) {
				auto [l, r] = ops[(split + i) % Q];
				//cerr << "doing suffix op " << ' ' << l << '-' << r << '\n';
				//cerr << "root =\n"; dump_treap(root);
				auto [x, yz] = split_at_point(root, l+1, [](treap_node* n) -> int { return n->lo; });
				auto [y, z] = split_at_point(yz, r+1, [](treap_node* n) -> int { return n->hi; });
				//cerr << "x =\n"; dump_treap(x);
				//cerr << "y =\n"; dump_treap(y);
				//cerr << "z =\n"; dump_treap(z);
				assert(x);
				auto [x_body, x_last] = split_at_point(x, l, [](treap_node* n) -> int { return n->hi; });
				assert(x_last && !x_last->c[0] && !x_last->c[1]);
				assert(x_last->lo <= l && l <= x_last->hi);
				if (x_last->color == -1) {
					assert(x_last->lo == l && l == x_last->hi);
					x_last->color = l;
				}
				if (x_last->hi >= r) {
					assert(!y);
				} else {
					std::y_combinator([&](auto self, treap_node* n) -> void {
						if (!n) return;
						self(n->c[0]);
						deletions[i].push_back(n->color == -1 ? std::pair<int, int>(n->lo, n->hi) : std::pair<int, int>(n->color, n->color));
						self(n->c[1]);
					})(y);
					delete y;
					if (z) {
						auto z_first = z;
						while (z_first->c[0]) z_first = z_first->c[0];
						if (z_first->lo <= r) {
							assert(z_first->color != -1);
							z_first->lo = r+1;
						}
					}
					x_last->hi = r;
				}
				root = merge(merge(x_body, x_last), z);
				//cerr << "root =\n"; dump_treap(root);
			}
			std::y_combinator([&](auto self, treap_node* n) -> void {
				if (!n) return;
				self(n->c[0]);
				deletions[K].push_back(n->color == -1 ? std::pair<int, int>(n->lo, n->hi) : std::pair<int, int>(n->color, n->color));
				self(n->c[1]);
			})(root);
			delete root;
		}

		{
			// Now, simulate adding things to the prefix
			treap_node* root = new treap_node;
			root->lo = 0;
			root->hi = N-1;
			root->color = -1;

			int num_good = 0;

			for (int i = 0; i <= K; i++) {
				for (auto [l, r] : deletions[K-i]) {
					//cerr << "doing insert" << ' ' << l << '-' << r << '\n';
					//cerr << "root =\n"; dump_treap(root);
					// mark l-r as good
					auto [x, yz] = split_at_point(root, l, [](treap_node* n) -> int { return n->hi; });
					auto [y, z] = split_at_point(yz, r+1, [](treap_node* n) -> int { return n->lo; });
					//cerr << "x =\n"; dump_treap(x);
					//cerr << "y =\n"; dump_treap(y);
					//cerr << "z =\n"; dump_treap(z);
					std::y_combinator([&](auto self, treap_node* n) -> void {
						if (!n) return;
						if (!n->is_good) {
							n->is_good = true;
							if (n->color == -1) {
								num_good += n->hi - n->lo + 1;
							} else {
								num_good++;
							}
						}
						self(n->c[0]);
						self(n->c[1]);
					})(y);
					if (z) {
						auto z_first = z;
						while (z_first->c[0]) z_first = z_first->c[0];
						if (z_first->lo <= r) {
							assert(false);
							assert(z_first->color != -1);
							if (!z_first->is_good) {
								z_first->is_good = true;
								num_good++;
							}
						}
					}
					root = merge(x, merge(y, z));
					//cerr << "root =\n"; dump_treap(root);
				}

				{
					int q = split - i;
					//cerr << "ans " << q << ' ' << num_good << '\n';
					if (q < Q) {
						ans[q] = num_good;
					}
				}

				if (i < K) {
					auto [l, r] = ops[(split - 1 - i) % Q];
					//cerr << "doing prefix op " << l << ' ' << r << '\n';
					// now, merge everything colored l..r
					//cerr << "root =\n"; dump_treap(root);
					// mark l-r as good
					auto [x, yz] = split_at_point(root, l, [](treap_node* n) { return n->color == -1 ? n->hi : n->color; });
					auto [y, z] = split_at_point(yz, r+1, [](treap_node* n) { return n->color == -1 ? n->hi : n->color; });
					//cerr << "x =\n"; dump_treap(x);
					//cerr << "y =\n"; dump_treap(y);
					//cerr << "z =\n"; dump_treap(z);

					if (y) {
						// merge all of y together
						bool n_good = false;
						int n_lo = N;
						int n_hi = 0;
						int n_color = l;
						std::y_combinator([&](auto self, treap_node* n) -> void {
							if (!n) return;
							if (n->is_good) {
								n_good = true;
								if (n->color == -1) {
									num_good -= n->hi - n->lo + 1;
								} else {
									num_good --;
								}
							}
							n_lo = std::min(n_lo, n->lo);
							n_hi = std::max(n_hi, n->hi);
							self(n->c[0]);
							self(n->c[1]);
						})(y);
						delete y;
						y = new treap_node;
						y->lo = n_lo;
						y->hi = n_hi;
						y->color = n_color;
						y->is_good = n_good;
						if (n_good) {
							num_good ++;
						}
					}

					root = merge(x, merge(y, z));
					//cerr << "root =\n"; dump_treap(root);
				}
			}
			delete root;
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << " \n"[q+1==Q];
	}

	return 0;
}
