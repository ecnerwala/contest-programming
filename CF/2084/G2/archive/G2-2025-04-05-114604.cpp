#include <bits/stdc++.h>

struct treap_node {
	std::array<treap_node*, 2> c = {nullptr, nullptr};
	std::mt19937_64::result_type pri;

	int sz;
	int64_t own_slope;
	std::pair<int64_t, int64_t> lazy{0, 0};

	void apply_lazy(std::pair<int64_t, int64_t> f) {
		own_slope += f.first;
		own_slope += (c[0] ? c[0]->sz : 0) * f.second;
		lazy.first += f.first;
		lazy.second += f.second;
	}
	void downdate() {
		if (c[0]) {
			c[0]->apply_lazy(lazy);
		}
		if (c[1]) {
			c[1]->apply_lazy({lazy.first + lazy.second * (1 + (c[0] ? c[0]->sz : 0)), lazy.second});
		}
		lazy = {0, 0};
	}
	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//slow(20);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		// we only care about the evens and the odds
		std::vector<int> invA(N, -1);
		for (int i = 0; i < N; i++) if (A[i] != -1) invA[A[i]] = i;

		std::mt19937_64 mt(48);
		std::vector<treap_node> nodes; nodes.reserve(N);
		treap_node* root = nullptr;
		int c0 = 0;
		int64_t v0 = 0;
		int F = N / 2;
		for (int i = 0; i < N; i++) {
			{
				// first, add the function
				int64_t a = int64_t(2 * c0 - i) * int64_t(2 * (F - c0) - (N - i));
				int64_t b = int64_t(2 * (c0 + 1) - i) * int64_t(2 * (F - (c0 + 1)) - (N - i));
				v0 += a;
				if (root) {
					root->apply_lazy({
						b - a, -8
					});
				}
			}
			if (invA[i] == -1) {
				// insert a 0 row
				treap_node* n = &nodes.emplace_back();
				n->own_slope = 0;
				n->update();
				[&](this auto&& self, treap_node* &cur) -> void {
					if (!cur || n->pri < cur->pri) {
						// run split
						[&](this auto&& split, treap_node* a, treap_node* &l, treap_node* &r) -> void {
							if (!a) { l = r = nullptr; return; }
							a->downdate();
							if (n->own_slope >= a->own_slope) {
								r = a;
								split(a->c[0], l, r->c[0]);
							} else {
								l = a;
								split(a->c[1], l->c[1], r);
							}
							a->update();
						}(cur, n->c[0], n->c[1]);
						n->update();
						cur = n;
					} else {
						cur->downdate();
						if (n->own_slope >= cur->own_slope) {
							self(cur->c[0]);
						} else {
							self(cur->c[1]);
						}
						cur->update();
					}
				}(root);
			} else {
				c0 += invA[i] & 1;
			}
		}
		int64_t ans = v0;
		[&](this auto&& self, treap_node* cur) -> void {
			if (!cur) return;

			cur->downdate();
			if (cur->c[0]) self(cur->c[0]);
			if (c0 < F) {
				ans += cur->own_slope;
				c0++;
			}
			if (cur->c[1]) self(cur->c[1]);
		}(root);
		ans += int64_t(N+1) * int64_t(N) * int64_t(N+1) / 2;
		assert(!(ans & 1));
		ans /= 2;
		cout << ans << '\n';
	}

	return 0;
}
