#include <bits/stdc++.h>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int floor_log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int ceil_log_2(int a) {
	return a ? floor_log_2(2*a-1) : -1;
}

inline int next_pow_2(int a) {
	return 1 << ceil_log_2(a);
}

struct point {
	int a;
	point() : a(0) {}
	explicit point(int a_) : a(a_) { assert(a >= -1); }

	explicit operator bool () { return bool(a); }

	// This is useful so you can directly do array indices
	/* implicit */ operator int() const { return a; }

	point c(bool z) const {
		return point((a<<1)|z);
	}

	point operator [] (bool z) const {
		return c(z);
	}

	point p() const {
		return point(a>>1);
	}

	friend std::ostream& operator << (std::ostream& o, const point& p) { return o << int(p); }

	template <typename F> void for_each(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	template <typename F> void for_each_down(F f) const {
		// strictly greater than 0
		for (int L = floor_log_2(a); L >= 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_each_up(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = floor_log_2(a); L > 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		for (int v = a >> 1; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	point& operator ++ () { ++a; return *this; }
	point operator ++ (int) { return point(a++); }
	point& operator -- () { --a; return *this; }
	point operator -- (int) { return point(a--); }
};

struct range {
	int a, b;
	range() : a(1), b(1) {}
	range(int a_, int b_) : a(a_), b(b_) {
		assert(1 <= a && a <= b && b <= 2 * a);
	}
	explicit range(std::array<int, 2> r) : range(r[0], r[1]) {}

	explicit operator std::array<int, 2>() const {
		return {a,b};
	}

	const int& operator[] (bool z) const {
		return z ? b : a;
	}

	friend std::ostream& operator << (std::ostream& o, const range& r) { return o << "[" << r.a << ".." << r.b << ")"; }

	// Iterate over the range from outside-in.
	//   Calls f(point a)
	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++));
			if (y & 1) f(point(--y));
		}
	}

	// Iterate over the range from outside-in.
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each_with_side(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	// Iterate over the range from left to right.
	//    Calls f(point)
	template <typename F> void for_each_l_to_r(F f) const {
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = floor_log_2((x-1) ^ y);
		for (int i = floor_log_2(x); i > dx; i--) {
			f(point(x >> i));
		}
		for (int i = anc_depth; i > dy; i--) {
			f(point(y >> i));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = floor_log_2((x-1) ^ y);
		for (int i = dx+1; i <= anc_depth; i++) {
			f(point(x >> i));
		}
		for (int v = y >> (dy+1); v; v >>= 1) {
			f(point(v));
		}
	}
};

struct in_order_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N, S;
	in_order_layout() : N(0), S(0) {}
	in_order_layout(int N_) : N(N_), S(N ? next_pow_2(N) : 0) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		a += S;
		return point(a >= 2 * N ? a - N : a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		if (N == 0) return range();
		a += S, b += S;
		return range((a >= 2 * N ? 2*(a-N) : a), (b >= 2 * N ? 2*(b-N) : b));
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return (a < S ? a + N : a) - S;
	}

	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? (x>>1) + N : x) - S, (y >= 2 * N ? (y>>1) + N : y) - S};
	}

	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		int l = __builtin_clz(2*a+1) - __builtin_clz(2*N-1);
		int x = (2*a+1) << l;
		assert(S <= x && x < 2*S);
		return (x >= 2 * N ? (x>>1) + N : x) - S;
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		return bounds[1] - bounds[0];
	}
};

struct circular_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N;
	circular_layout() : N(0) {}
	circular_layout(int N_) : N(N_) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		return point(N + a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		if (N == 0) return range();
		return range(N + a, N + b);
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return a - N;
	}

	// Returns {x,y} so that 0 <= x < N and 1 <= y <= N
	// If the point is non-wrapping, then 0 <= x < y <= N
	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int S = next_pow_2(N);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? x >> 1 : x) - N, (y > 2 * N ? y >> 1 : y) - N};
	}

	// Returns the split point of the node, such that 1 <= s <= N.
	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		return get_node_bounds(pt.c(0))[1];
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		int r = bounds[1] - bounds[0];
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

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

struct splay_node {
	splay_node* p = nullptr;
	std::array<splay_node*, 2> c{nullptr, nullptr};
	// sorted increasing by time
	int64_t own_time;
	int64_t own_b;

	int64_t min_a_needed;
	int64_t tot_b;

	bool d() { return this == p->c[1]; }
	void update() {
		tot_b = own_b;
		min_a_needed = own_time - own_b;
		if (c[0]) {
			min_a_needed = std::min(min_a_needed - c[0]->tot_b, c[0]->min_a_needed);
			tot_b += c[0]->tot_b;
		}
		if (c[1]) {
			min_a_needed = std::min(min_a_needed, c[1]->min_a_needed - tot_b);
			tot_b += c[1]->tot_b;
		}
	}

	void rot() {
		assert(p);
		splay_node* pa = p;
		int x = d();

		if (pa->p) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		if (this->c[!x]) this->c[!x]->p = pa;
		pa->c[x] = this->c[!x];

		pa->p = this;
		this->c[!x] = p;

		pa->update();
	}

	void splay() {
		while (p) {
			if (p->p) {
				if (d() == p->d()) p->rot();
				else rot();
			}
			rot();
		}
		update();
	}
};

splay_node* concat(splay_node* a, splay_node* b) {
	if (!a) return b;
	if (!b) return a;
	assert(!a->p);
	assert(!b->p);
	while (b->c[0]) b = b->c[0];
	b->splay();
	assert(!b->c[0]);
	b->c[0] = a;
	a->p = b;
	b->update();
	return b;
}

splay_node* erase(splay_node* n) {
	n->splay();
	auto [a, b] = n->c;
	n->c = {nullptr, nullptr};
	if (a) a->p = nullptr;
	if (b) b->p = nullptr;
	return concat(a, b);
}

void insert_and_splay(splay_node* n, splay_node* t) {
	assert(n);
	assert(!n->p);
	assert(!n->c[0]);
	assert(!n->c[1]);

	if (!t) return;
	assert(!t->p);
	while (true) {
		assert(n->own_time != t->own_time);
		int d = n->own_time > t->own_time;
		if (!t->c[d]) {
			t->c[d] = n;
			n->p = t;
			n->splay();
			return;
		} else {
			t = t->c[d];
		}
	}
}

void main_slow() {
	using namespace std;

	int N, Q; cin >> N >> Q;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) {
		cin >> P[i]; P[i]--;
	}
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	auto sim = [&]() -> std::vector<int> {
		std::vector<int64_t> votes(N);
		for (int i = 0; i < N; i++) {
			votes[i] += A[i];
			if (i) votes[P[i]] += B[i];
		}
		std::vector<int> times(N, -1);
		for (int t = 0; t < N; t++) {
			int64_t most_votes = -1;
			int best_person = -1;
			for (int i = N-1; i >= 0; i--) {
				if (times[i] != -1) continue;
				if (best_person == -1 || votes[i] > most_votes) {
					best_person = i;
					most_votes = votes[i];
				}
			}
			times[best_person] = t;
			if (best_person) votes[P[best_person]] -= B[best_person];
		}
		return times;
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int p; int x, y; cin >> p >> x >> y; p--;
			A[p] = x;
			B[p] = y;
		} else if (op == 2) {
			int c, d; cin >> c >> d; c--, d--;
			auto times = sim();
			cout << int(times[d] < times[c]) << '\n';
		}
	}
}

void main_fast() {
	using namespace std;

	int N, Q; cin >> N >> Q;
	std::vector<int> orig_par(N);
	orig_par[0] = -1;
	for (int i = 1; i < N; i++) { cin >> orig_par[i]; orig_par[i] --; }
	std::vector<std::vector<int>> orig_ch(N);
	for (int i = 1; i < N; i++) orig_ch[orig_par[i]].push_back(i);

	{
		std::vector<int> sz(N, 1);
		for (int i = N-1; i > 0; i--) sz[orig_par[i]] += sz[i];
		for (int i = 0; i < N; i++) {
			if (!orig_ch[i].empty()) {
				auto it = std::max_element(orig_ch[i].begin(), orig_ch[i].end(), [&](int a, int b) { return sz[a] < sz[b]; });
				std::swap(*it, *orig_ch[i].begin());
			}
		}
	}
	std::vector<int> preorder; preorder.reserve(N);
	std::vector<int> preorder_idx(N);
	std::vector<int> par(N);
	std::vector<int> heavy_root(N);
	std::vector<int> heavy_cnt(N);
	{
		std::y_combinator([&](auto self, int cur, int prv_idx) -> void {
			int cur_idx = int(preorder.size());
			preorder.push_back(cur);
			preorder_idx[cur] = cur_idx;

			par[cur_idx] = prv_idx;
			if (prv_idx != -1 && cur_idx == prv_idx+1) heavy_root[cur_idx] = heavy_root[prv_idx];
			else heavy_root[cur_idx] = cur_idx;
			heavy_cnt[heavy_root[cur_idx]]++;
			for (int nxt : orig_ch[cur]) {
				self(nxt, cur_idx);
			}
		})(0, -1);
		assert(int(preorder.size()) == N);
	}

	std::vector<int64_t> A(N);
	for (int i = 0; i < N; i++) {
		auto& a = A[preorder_idx[i]];
		cin >> a; a *= N; a += i;
	}
	std::vector<int64_t> B(N);
	for (int i = 0; i < N; i++) {
		auto& b = B[preorder_idx[i]];
		cin >> b; b *= N;
	}

	std::vector<splay_node> nodes(N);
	std::vector<splay_node*> light_trees(N, nullptr);

	struct piecewise_fn {
		int64_t thresh;
		int64_t v_smaller;
		int64_t v_bigger;
	};
	auto query_tree = [&](int i, int64_t a) -> int64_t {
		splay_node* t = light_trees[i];
		assert(!t || !t->p);
		if (!t || a < t->min_a_needed) {
			return a;
		}
		while (true) {
			assert(a >= t->min_a_needed);
			int64_t b_left = (t->c[0] ? t->c[0]->tot_b : 0) + t->tot_b;
			if (t->c[1] && a + b_left >= t->c[1]->min_a_needed) {
				a += b_left;
				t = t->c[1];
			} else if (a + b_left >= t->own_time) {
				a += b_left;
				break;
			} else {
				assert(t->c[0] && a >= t->c[0]->min_a_needed);
				t = t->c[0];
			}
		}
		t->splay();
		light_trees[i] = t;
		return a;
	};

	auto query_lights = [&](int i) -> piecewise_fn {
		if (i+1 == N || heavy_root[i+1] != heavy_root[i]) {
			// we're a leaf
			assert(light_trees[i] == nullptr);
			return piecewise_fn{-1, A[i], A[i]};
		} else {
			//cerr << "query " << i << ' ' << A[i] << ' ' << B[i+1] << '\n';
			int64_t v_smaller = query_tree(i, A[i] + B[i+1]);
			int64_t v_bigger = query_tree(i, A[i]);
			assert(v_smaller >= v_bigger);
			// If the next guy is smaller than v_smaller, we prefer it
			return piecewise_fn{v_smaller, v_smaller, v_bigger};
		}
	};

	std::vector<std::vector<piecewise_fn>> heavy_segs(N);
	auto eval = [](piecewise_fn a, int64_t v) -> int64_t {
		return (v < a.thresh) ? a.v_smaller : a.v_bigger;
	};
	auto compose = [eval](piecewise_fn a, piecewise_fn b) -> piecewise_fn {
		return {b.thresh, eval(a, b.v_smaller), eval(a, b.v_bigger) };
	};

	cerr << "par "; for (int i = 0; i < N; i++) { cerr << par[i] << ' '; } cerr << '\n';
	cerr << "hrt "; for (int i = 0; i < N; i++) { cerr << heavy_root[i] << ' '; } cerr << '\n';
	cerr << "A "; for (int i = 0; i < N; i++) { cerr << A[i] << ' '; } cerr << '\n';
	cerr << "B "; for (int i = 0; i < N; i++) { cerr << B[i] << ' '; } cerr << '\n';

	for (int i = N-1; i >= 0; i--) {
		if (heavy_root[i] == i) {
			int sz = heavy_cnt[i];
			seg_tree::in_order_layout layout(sz);
			heavy_segs[i].resize(2*sz);
			for (int j = i; j < i + sz; j++) {
				auto& fn = heavy_segs[i][layout.get_point(j - i)] = query_lights(j);
				cerr << "lights " << j << ' ' << fn.thresh << "?" << fn.v_smaller << ":" << fn.v_bigger << '\n';
			}
			for (seg_tree::point a(sz-1); a > 0; a--) {
				heavy_segs[i][a] = compose(heavy_segs[i][a.c(0)], heavy_segs[i][a.c(1)]);
			}
			assert(heavy_segs[i][1].thresh == -1);
			assert(heavy_segs[i][1].v_smaller == heavy_segs[i][1].v_bigger);
			if (i != 0) {
				int p = par[i];
				assert(0 <= p && p < i-1);
				nodes[i].own_time = heavy_segs[i][1].v_smaller;
				nodes[i].own_b = B[i];
				nodes[i].update();
				cerr << "own_time " << i << ' ' << nodes[i].own_time << ' ' << nodes[i].own_b << '\n';
				insert_and_splay(&nodes[i], light_trees[p]);
				light_trees[p] = &nodes[i];
			}
		}
	}
	auto update_heavy_chain = [&](int v) -> void {
		int i = heavy_root[v];
		int sz = heavy_cnt[i];
		seg_tree::in_order_layout layout(sz);
		auto x = layout.get_point(v - i);
		auto& fn = heavy_segs[i][x] = query_lights(v);
		cerr << "lights " << v << ' ' << fn.thresh << "?" << fn.v_smaller << ":" << fn.v_bigger << '\n';
		x.for_parents_up([&](auto a) -> void {
			heavy_segs[i][a] = compose(heavy_segs[i][a.c(0)], heavy_segs[i][a.c(1)]);
		});
	};

	auto update = [&](int v) -> void {
		update_heavy_chain(v);
		if (v != heavy_root[v]) {
			// B[i] changed, so update v-1
			update_heavy_chain(v-1);
		} else {
			// otherwise, we'll just update it as part of the nodes[v] update
		}

		// now just update up the chain
		while (true) {
			v = heavy_root[v];
			assert(v == heavy_root[v]);
			int p = par[v];
			if (p == -1) {
				assert(v == 0);
				break;
			}
			auto t = erase(&nodes[v]);
			nodes[v].own_time = heavy_segs[v][1].v_smaller;
			nodes[v].own_b = B[v];
			nodes[v].update();
			cerr << "own_time " << v << ' ' << nodes[v].own_time << ' ' << nodes[v].own_b << '\n';
			insert_and_splay(&nodes[v], t);
			light_trees[p] = &nodes[v];
			v = p;
			update_heavy_chain(v);
		}
	};

	auto sim = [&]() -> std::vector<int64_t> {
		std::vector<int64_t> votes(N);
		for (int i = 0; i < N; i++) {
			votes[i] += A[i];
			if (i) votes[par[i]] += B[i];
		}
		std::vector<int64_t> times(N, -1);
		for (int t = 0; t < N; t++) {
			int64_t most_votes = -1;
			int best_person = -1;
			for (int i = N-1; i >= 0; i--) {
				if (times[i] != -1) continue;
				if (best_person == -1 || votes[i] > most_votes) {
					best_person = i;
					most_votes = votes[i];
				}
			}
			times[best_person] = most_votes;
			if (best_person) votes[par[best_person]] -= B[best_person];
		}
		return times;
	};


	auto query = [&](int v) -> int64_t {
		int i = heavy_root[v];
		int sz = heavy_cnt[i];
		seg_tree::in_order_layout layout(sz);
		auto x = layout.get_range(v - i, sz);
		int64_t res = -1;
		x.for_each_r_to_l([&](auto a) -> void {
			res = eval(heavy_segs[i][a], res);
		});
		auto sim_res = sim()[v];
		cerr << "query " << v << ' ' << res << ' ' << sim()[v] << '\n';
		assert(res == sim_res);
		return res;
	};

	for (int q = 0; q < Q; q++) {
		cerr << '\n';
		int op; cin >> op;
		if (op == 1) {
			int p; int64_t x, y; cin >> p >> x >> y; p--;
			x *= N; x += p;
			y *= N;
			p = preorder_idx[p];
			A[p] = x;
			B[p] = y;
			cerr << "update " << p << ' ' << A[p] << ' ' << B[p] << '\n';
			update(p);
		} else if (op == 2) {
			int c, d; cin >> c >> d; c--, d--;
			c = preorder_idx[c];
			d = preorder_idx[d];
			cout << int(query(c) < query(d)) << '\n';
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	main_fast();
	return 0;
}
