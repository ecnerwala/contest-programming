#include <bits/stdc++.h>

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
		len = 1;
		for (int i = NDIMS-1; i >= 0; i--) {
			strides[i] = len;
			len *= shape[i];
		}
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

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
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
	
	using num = modnum<998244353>;

	int N, M; cin >> N >> M;
	std::vector<std::vector<std::tuple<int, int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v, d; cin >> u >> v >> d; u--, v--;
		assert(u != v);
		adj[u].push_back({v, d, e});
		adj[v].push_back({u, d, e});
	}

	// Each vertex must have an odd number of out_edges
	// Also we need to 3-color the cactus
	// We can count these separately
	struct color_ways {
		num same_t;
		num diff_t;
	};
	auto merge_ways = [&](color_ways a, color_ways b) -> color_ways {
		return color_ways{
			a.same_t * b.same_t + a.diff_t * 2 * b.diff_t,
			a.same_t * b.diff_t + a.diff_t * b.same_t + a.diff_t * b.diff_t
		};
	};
	auto edge_color_ways = [&](int num_e) -> color_ways {
		color_ways res{1, 0};
		color_ways base{0, 1};
		while (num_e) {
			if (num_e & 1) res = merge_ways(res, base);
			base = merge_ways(base, base);
			num_e >>= 1;
		}
		return res;
	};

	struct dp_t {
		// child edge parity, upedge parity
		std::array<std::array<num, 2>, 2> edge_ways;

		// shallow value relative to current root
		color_ways cways;

		int edge_root = -1;
	};
	std::vector<int> depth(N, -1);
	depth[0] = 0;
	auto root_dp = std::y_combinator([&](auto self, int cur, int prv_e) -> dp_t {
		dp_t res;
		res.edge_ways[0][0] = 1;
		res.cways = {1, 0};
		res.edge_root = -1;
		for (auto [nxt, d, e] : adj[cur]) {
			if (e == prv_e) continue;
			dp_t ch;
			if (depth[nxt] == -1) {
				depth[nxt] = depth[cur] + 1;
				ch = self(nxt, e);
				ch.cways = merge_ways(edge_color_ways(d+1), ch.cways);
			} else if (depth[nxt] < depth[cur]) {
				assert(res.edge_root == -1);
				ch.edge_ways[0][1] = 1;
				ch.edge_ways[1][0] = 1;
				ch.cways = edge_color_ways(d+1);
				ch.edge_root = nxt;
			} else {
				// downedge, skip it
				assert(depth[nxt] > depth[cur]);
				continue;
			}

			std::array<std::array<num, 2>, 2> n_edge_ways;
			if (ch.edge_root != -1) {
				assert(res.edge_root == -1);
				if (ch.edge_root == cur) {
					res.cways.same_t *= ch.cways.same_t;
					res.cways.diff_t *= ch.cways.same_t;
					for (int i = 0; i < 2; i++) {
						for (int j = 0; j < 2; j++) {
							for (int k = 0; k < 2; k++) {
								n_edge_ways[i^j^k][0] += res.edge_ways[i][0] * ch.edge_ways[j][k];
							}
						}
					}
				} else {
					res.cways = merge_ways(res.cways, ch.cways);
					res.edge_root = ch.edge_root;
					for (int i = 0; i < 2; i++) {
						for (int j = 0; j < 2; j++) {
							for (int k = 0; k < 2; k++) {
								n_edge_ways[i^j][k] += res.edge_ways[i][0] * ch.edge_ways[j][k];
							}
						}
					}
				}
			} else {
				//assert(ch.cways.diff_t == 0);
				num all_ch_ways = ch.cways.same_t + 2 * ch.cways.diff_t;
				res.cways.same_t *= all_ch_ways;
				res.cways.diff_t *= all_ch_ways;
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						for (int k = 0; k < 2; k++) {
							n_edge_ways[i^j][k] += res.edge_ways[i][k] * ch.edge_ways[j][0];
						}
					}
				}
			}
			res.edge_ways = n_edge_ways;
		}
		//cerr << "vertex " << cur << ' ' << res.edge_root << ' ' << res.cways.same_t << ' ' << res.cways.diff_t << '\n';
		return res;
	})(0, -1);

	num a = root_dp.cways.same_t;
	num b = root_dp.edge_ways[1][0];
	cout << 3 * a * b << '\n';

	return 0;
}
