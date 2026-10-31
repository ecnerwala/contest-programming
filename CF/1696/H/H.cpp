#include <bits/stdc++.h>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
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

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = log_2(a); L > 0; L--) {
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
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = log_2((x-1) ^ y);
		for (int i = log_2(x); i > dx; i--) {
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
		int anc_depth = log_2((x-1) ^ y);
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

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	T* data;

	tensor_view(std::array<int, NDIMS> shape_, std::array<int, NDIMS> strides_, T* data_) : shape(shape_), strides(strides_), data(data_) {}

public:
	tensor_view() : shape{0}, strides{0}, data(nullptr) {}

protected:
	int flatten_index(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) { res += idx[i] * strides[i]; }
		return res;
	}
	int flatten_index_checked(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) {
			assert(0 <= idx[i] && idx[i] < shape[i]);
			res += idx[i] * strides[i];
		}
		return res;
	}

public:
	T& operator[] (std::array<int, NDIMS> idx) const {
		return data[flatten_index(idx)];
	}
	T& at(std::array<int, NDIMS> idx) const {
		return data[flatten_index_checked(idx)];
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	int len;
	T* data;

public:
	tensor() : shape{0}, strides{0}, len(0), data(nullptr) {}

	explicit tensor(std::array<int, NDIMS> shape_, const T& t = T()) {
		shape = shape_;
		strides[NDIMS-1] = 1;
		for (int i = NDIMS-1; i > 0; i--) {
			strides[i-1] = strides[i] * shape[i];
		}
		len = strides[0] * shape[0];
		data = new T[len];
		std::fill(data, data + len, t);
	}

	tensor(const tensor& o) : shape(o.shape), strides(o.strides), len(o.len), data(new T[len]) {
		for (int i = 0; i < len; i++) {
			data[i] = o.data[i];
		}
	}

	tensor& operator=(tensor&& o) noexcept {
		using std::swap;
		swap(shape, o.shape);
		swap(strides, o.strides);
		swap(len, o.len);
		swap(data, o.data);
		return *this;
	}
	tensor(tensor&& o) : tensor() {
		*this = std::move(o);
	}
	tensor& operator=(const tensor& o) {
		return *this = tensor(o);
	}
	~tensor() { delete[] data; }

	using view_t = tensor_view<T, NDIMS>;
	view_t view() {
		return tensor_view<T, NDIMS>(shape, strides, data);
	}
	operator view_t() {
		return view();
	}

	using const_view_t = tensor_view<const T, NDIMS>;
	const_view_t view() const {
		return tensor_view<const T, NDIMS>(shape, strides, data);
	}
	operator const_view_t() const {
		return view();
	}

	T& operator[] (std::array<int, NDIMS> idx) { return view()[idx]; }
	T& at(std::array<int, NDIMS> idx) { return view().at(idx); }
	const T& operator[] (std::array<int, NDIMS> idx) const { return view()[idx]; }
	const T& at(std::array<int, NDIMS> idx) const { return view().at(idx); }

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () {
		return *view();
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 == D), const T&>::type operator * () const {
		return *view();
	}
};

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//using num = modnum<int(1e9)+7>;
	using num = int64_t;

	int N_TOT, K; cin >> N_TOT >> K;

	std::vector<num> p2(N_TOT+1);
	p2[0] = 1;
	for (int i = 1; i <= N_TOT; i++) p2[i] = p2[i-1] + p2[i-1];

	if (K == 1) {
		std::vector<int> A(N_TOT);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		std::reverse(A.begin(), A.end());
		num ans = 0;
		for (int v : A) {
			ans += ans;
			ans += num(v);
		}
		cout << ans << '\n';
		exit(0);
	}

	// both store absolute values
	std::vector<int64_t> pos; pos.reserve(N_TOT);
	std::vector<int64_t> neg; neg.reserve(N_TOT);
	for (int i = 0; i < N_TOT; i++) {
		int a; cin >> a;
		if (a >= 0) pos.push_back(a);
		else neg.push_back(-a);
	}

	// sort in increasing absolute value
	std::sort(pos.begin(), pos.end());
	std::sort(neg.begin(), neg.end());

	int N_pos = int(pos.size());
	int N_neg = int(neg.size());

	tensor<num, 2> dp_pos({N_pos+1, K+1});
	dp_pos[{N_pos, 0}] = 1;
	for (int i = N_pos-1; i >= 0; i--) {
		for (int k = 0; k <= K; k++) {
			dp_pos[{i, k}] = dp_pos[{i+1, k}];
			if (k > 0) dp_pos[{i, k}] += dp_pos[{i+1, k-1}] * num(pos[i]);
		}
	}

	tensor<num, 2> dp_neg({N_neg+1, K+1});
	dp_neg[{N_neg, 0}] = 1;
	for (int i = N_neg-1; i >= 0; i--) {
		for (int k = 0; k <= K; k++) {
			dp_neg[{i, k}] = dp_neg[{i+1, k}];
			if (k > 0) dp_neg[{i, k}] += dp_neg[{i+1, k-1}] * num(neg[i]);
		}
	}

	std::vector<std::tuple<int64_t, bool, std::array<int, 2>>> evts; evts.reserve(N_TOT * N_TOT);
	for (int i = 0; i < N_pos; i++) {
		for (int j = i+1; j < N_pos; j++) {
			evts.push_back({pos[i] * pos[j], 1, {i, j}});
		}
		evts.push_back({-1, 1, {0, i}});
	}
	for (int i = 0; i < N_neg; i++) {
		for (int j = i+1; j < N_neg; j++) {
			evts.push_back({neg[i] * neg[j], 0, {i, j}});
		}
		evts.push_back({-1, 0, {0, i}});
	}
	std::sort(evts.begin(), evts.end());
	std::reverse(evts.begin(), evts.end());

	seg_tree::in_order_layout layout_pos(N_pos);
	tensor<num, 2> seg_big_pos({2 * N_pos, K+1});
	tensor<num, 2> seg_small_pos({2 * N_pos, K+1});
	seg_tree::in_order_layout layout_neg(N_neg);
	tensor<num, 2> seg_big_neg({2 * N_neg, K+1});
	tensor<num, 2> seg_small_neg({2 * N_neg, K+1});

	num ans = 0;
	for (auto [cur_prod_, is_pos_, inds_] : evts) {
		auto cur_prod = cur_prod_;
		auto is_pos = is_pos_;
		auto inds = inds_;
		if (cur_prod != -1) {
			num weight = p2[inds[0]] * num(cur_prod);
			// First, set seg_big
			if (is_pos) {
				auto rng = layout_pos.get_range(inds[0], N_pos);
				rng.for_each([&](auto a) -> void {
					for (int k = 2; k <= K; k++) {
						seg_big_pos[{a, k}] += weight * dp_pos[{inds[1]+1, k-2}];
					}
				});
			} else {
				auto rng = layout_neg.get_range(inds[0], N_neg);
				rng.for_each([&](auto a) -> void {
					for (int k = 2; k <= K; k++) {
						seg_big_neg[{a, k}] += weight * dp_neg[{inds[1]+1, k-2}];
					}
				});
			}
		}

		if (cur_prod != -1) {
			num weight = p2[inds[0]] * num(cur_prod);

			// assume this is the last paired one
			int mi, ma;
			if (is_pos) {
				mi = -1;
				ma = N_neg;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (neg[md] * neg[md] <= cur_prod) {
						mi = md;
					} else {
						ma = md;
					}
				}

				// it does cross it
				for (int k = 2; k <= K; k++) {
					if ((K-k) % 2 == 0) ans += weight * dp_pos[{inds[1]+1, k-2}] * p2[ma] * dp_neg[{ma, K-k}];
				}

				// big side covers mi
				if (mi >= 0) {
					auto p = layout_neg.get_point(mi);
					p.for_each([&](auto a) -> void {
						for (int k = 2; k <= K; k++) {
							if ((K-k) % 2 == 0) ans += weight * dp_pos[{inds[1]+1, k-2}] * seg_big_neg[{a, K-k}];
						}
					});
				}

				// prepare for small side covers ma
				if (ma < N_neg) {
					auto rng = layout_neg.get_point(ma);
					rng.for_each([&](auto a) -> void {
						for (int k = 2; k <= K; k++) {
							seg_small_neg[{a, k}] += weight * dp_pos[{inds[1]+1, k-2}];
						}
					});
				}
			} else {
				mi = -1;
				ma = N_pos;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (pos[md] * pos[md] < cur_prod) {
						mi = md;
					} else {
						ma = md;
					}
				}

				// it does cross it
				for (int k = 2; k <= K; k++) {
					if (k % 2 == 0) ans += weight * dp_neg[{inds[1]+1, k-2}] * p2[ma] * dp_pos[{ma, K-k}];
				}

				// big side covers mi
				if (mi >= 0) {
					auto p = layout_pos.get_point(mi);
					p.for_each([&](auto a) -> void {
						for (int k = 2; k <= K; k++) {
							if (k % 2 == 0) ans += weight * dp_neg[{inds[1]+1, k-2}] * seg_big_pos[{a, K-k}];
						}
					});
				}

				// prepare for small side covers ma
				if (ma < N_pos) {
					auto rng = layout_pos.get_point(ma);
					rng.for_each([&](auto a) -> void {
						for (int k = 2; k <= K; k++) {
							seg_small_pos[{a, k}] += weight * dp_neg[{inds[1]+1, k-2}];
						}
					});
				}
			}
		}

		if (is_pos) {
			auto rng = layout_pos.get_range(0, inds[1]+1);
			rng.for_each([&](auto a) -> void {
				for (int k = 0; k <= K; k++) {
					if ((K-k) % 2 == 0) ans += p2[inds[0]] * dp_pos[{inds[1]+1, k}] * seg_small_pos[{a, K-k}];
				}
			});
		} else {
			auto rng = layout_neg.get_range(0, inds[1]+1);
			rng.for_each([&](auto a) -> void {
				for (int k = 0; k <= K; k++) {
					if (k % 2 == 0) ans += p2[inds[0]] * dp_neg[{inds[1]+1, k}] * seg_small_neg[{a, K-k}];
				}
			});
		}
	}
	for (int k = 1; k <= K; k++) {
		ans += num(-1) * dp_neg[{0, k}] * dp_pos[{0, K-k}];
	}
	if (K & 1) {
		num ways = 0;
		std::vector<num> dp(K+1);
		dp[0] = 1;
		for (int i = 0; i < N_neg; i++) {
			ways += ways;
			ways += dp[K];
			for (int k = K; k > 0; k--) {
				dp[k] += dp[k-1] * num(neg[i]);
			}
		}
		ans += num(-1) * ways;
	}

	cout << ans << '\n';

	return 0;
}
