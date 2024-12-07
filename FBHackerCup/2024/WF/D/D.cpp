#include<bits/stdc++.h>

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
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; int64_t L; cin >> N >> M >> L;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int64_t> B(M);
		for (auto& b : B) cin >> b;

		for (auto a : A) assert(a <= 100);
		for (auto b : B) assert(b > 100);

		cerr << "Case #" << case_num << ": " << N << ' ' << M << '\n';

		using num = modnum<int(1e9)+7>;

		std::sort(A.begin(), A.end());
		std::sort(B.begin(), B.end());

		// our dp state is how many ends we have, and whether we've used the start/end
		tensor<std::vector<num>, 3> dp({1, 2, 2});
		dp[{0, 0, 0}] = std::vector<num>(1);
		for (int z = 0; z < N; z++) {
			tensor<std::vector<num>, 3> ndp({z+2, 2, 2});
			int dist = (z + 1 == N ? 100 : A[z+1]) - A[z];
			for (int c = 0; c <= z; c++) {
				for (int s = 0; s < 2; s++) {
					for (int t = 0; t < 2; t++) {
						if (c - s - t < 0) assert((dp[{c, s, t}].empty()));
						if (dp[{c, s, t}].empty()) continue;
						auto do_upd = [&](num w, int nc, int ns, int nt) -> void {
							assert(w != 0);
							const auto& src = dp[{c, s, t}];
							auto& dst = ndp[{nc, ns, nt}];
							cerr << c << ' ' << s << ' ' << t << " -> " << nc << ' ' << ns << ' ' << nt << '\n';
							assert(nc - ns - nt >= 0);
							int delta = dist * (2 * nc - ns - nt);
							assert(delta >= 0);
							int need_sz = int(src.size()) + delta;
							if (int(dst.size()) < need_sz) {
								dst.resize(need_sz);
							}
							for (int i = 0; i < int(src.size()); i++) {
								dst[i + delta] += src[i] * w;
							}
						};
						{
							// new piece connected to nothing
							num w = c + 1 - s - t;
							do_upd(w, c+1, s, t);
						}
						// connect on the left to src
						if (s == 0) {
							do_upd(1, c + 1, 1, t);
						}
						// connect on the right to sink
						if (t == 0) {
							do_upd(1, c + 1, s, 1);
						}
						// connect on the left to something
						if (c - t > 0) {
							do_upd(c - t, c, s, t);
						}
						// connect on the right to something
						if (c - s > 0) {
							do_upd(c - s, c, s, t);
						}
						// left sink and right something
						if (s == 0 && c - t > 0) {
							do_upd(1, c, 1, t);
						}
						// right sink and left something
						if (t == 0 && c - s > 0) {
							do_upd(1, c, s, 1);
						}
						// both something
						if (c >= 2 && c - s - t > 0) {
							do_upd(c-1, c - 1, s, t);
						}
					}
				}
			}
			dp = std::move(ndp);
		}
		for (int c = 0; c <= N; c++) {
			for (int s = 0; s < 2; s++) {
				for (int t = 0; t < 2; t++) {
					auto& a = dp[{c, s, t}];
					for (int i = 0; i+1 < int(a.size()); i++) {
						a[i+1] += a[i];
					}
				}
			}
		}

		num ans = 0;
		for (int msk = 0; msk < (1 << (M+1)); msk++) {
			int s = (msk >> 0) & 1;
			int t = (msk >> M) & 1;
			int c = __builtin_popcount(msk);
			if (msk == 0) continue;
			if (c > N) continue;
			const auto& a = dp[{c, s, t}];
			if (a.empty()) continue;
			do {
				int64_t weight = 0;
				for (int v = 0; v < M; v++) {
					int fac = __builtin_popcount((msk >> v) & 3);
					weight += fac * (B[v] - 100);
				}
				for (int v = 1; v < M; v++) {
					if (!((msk >> v) & 1)) {
						weight += std::abs(B[v] - B[v-1]);
					}
				}
				if (weight <= L) {
					ans += a[std::min<int64_t>(int(a.size()) - 1, L - weight)];
				}
			} while (std::next_permutation(B.begin(), B.end()));
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
