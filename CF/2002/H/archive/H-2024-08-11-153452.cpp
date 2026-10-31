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

	using num = modnum<int(1e9)+7>;
	//const int N = 130;
	const int N = 10;
	const int M = 30;

	// the main dp is:
	// M, length, cost, max surplus, min surplus without paying
	tensor<num, 3> ans({M+1, N+1, N+1});
	ans[{0, 0, 0}] = num(1);
	for (int m = 1; m <= M; m++) {
		cerr << "M = " << m << '\n';
		tensor<num, 3> cur_dp({N+1, N+1, N+1});
		tensor<num, 3> fail_dp({N+1, N+1, N+1});
		for (int l1 = 0; l1 <= N; l1++) {
			for (int c1 = l1 & 1; c1 <= l1; c1 += 2) {
				assert(0 <= l1 && l1 < N+1);
				assert(0 <= c1 && c1 < N+1);
				cur_dp[{l1, c1, l1}] += ans[{m-1, l1, c1}];
			}
		}
		for (int c0 = 0; c0 <= N; c0++) {
			for (int l0 = c0; l0 <= N; l0++) {
				for (int a1 = (l0 ^ c0) & 1; a1 + c0 <= l0; a1 += 2) {
					cur_dp[{l0, a1 & 1, a1}] += fail_dp[{c0, l0, a1}];
				}
			}

			// update ans
			for (int l0 = c0; l0 <= N; l0++) {
				for (int a0 = (l0 ^ c0) & 1; a0 + c0 <= l0; a0 += 2) {
					for (int a1 = a0; a1 + c0 <= l0; a1 += 2) {
						num w = cur_dp[{l0, a0, a1}];
						if (a0 == 0) {
							ans[{m, l0, c0}] += w;
						} else if (a0 & 1) {
							ans[{m, l0, c0 + 1}] += w;
						} else {
							ans[{m, l0, c0 + 2}] += w;
						}
					}
				}
			}
			for (int l0 = c0; l0 <= N; l0++) {
				// could optimize further w/ prefix/suffix sums, but don't really need to, this is already M * N^4
				for (int a0 = l0 - c0; a0 >= 0; a0 -= 2) {
					num w = 0;
					for (int a1 = a0; a1 + c0 <= l0; a1 += 2) {
						w += cur_dp[{l0, a0, a1}];
					}
					num o = 1;
					for (int l1 = 0; l1 < a0 && l0 + 1 + l1 <= N; l1++, o *= num(m-1)) {
						int nl = l0 + l1 + 1;
						int nc = c0 + 1;
						int na1 = l1 - ((a0 - 1) & 1);
						nc++;
						if (na1 < 0) {
							assert(l1 == 0);
							assert(!(a0 & 1));
							na1 = l1;
							nc++;
						}
						//int na0 = na1 & 1;
						fail_dp[{nc, nl, na1}] += w * o;
					}
				}
			}

			// This is the big thing to optimize: make this into a convolution
			tensor<num, 3> nxt_dp({N+1, N+1, N+1});
			/*
			for (int l0 = c0; l0 <= N; l0++) {
				for (int a0 = (l0 ^ c0) & 1; a0 + c0 <= l0; a0 += 2) {
					for (int a1 = a0; a1 + c0 <= l0; a1 += 2) {
						num w = cur_dp[{l0, a0, a1}];
						for (int l1 = 0; l0 + 1 + l1 <= N; l1++) {
							for (int c1 = l1 & 1; c1 <= l1; c1 += 2) {
								num o = ans[{m-1, l1, c1}];
								if (o == 0) continue;

								int l0a0 = l0 + a0;
								int la = l0 - c0;
								int ga = (a1 - a0) / 2;
								int lb = l1;
								int gb = (l1 - c1) / 2;

								int lab = la + lb;
								int gab = ga + gb;

								int nl = lab + 1 + c0;
								int na1 = nl - l0a0 - 1;
								int na0 = na1 - 2 * gab;
								if (na1 < 0) continue;
								if (na0 < 0) na0 = na1 & 1;
								nxt_dp[{nl, na0, na1}] += w * o;
							}
						}
					}
				}
			}
			*/
			// first, let's fix l0 + a0 so that we can get nl and na1 the same way
			// l0 + a0 has to be the same parity as c0
			//
			for (int l0a0 = c0; l0a0 <= 2 * N - c0; l0a0 += 2) {
				// we now convolve pairs in coords of (len, gap), where len <= N - c0 - 1 and gap <= (N - c0)/2
				tensor<num, 2> lhs({N - c0, (N-c0)/2 + 1});
				for (int l0 = c0; l0 + 1 <= N; l0++) {
					int a0 = l0a0 - l0;
					if (0 <= a0 && a0 + c0 <= l0) {
						for (int a1 = a0; a1 + c0 <= l0; a1 += 2) {
							num w = cur_dp[{l0, a0, a1}];
							lhs[{l0 - c0, (a1 - a0) / 2}] += w;
						}
					}
				}
				tensor<num, 2> rhs({N - c0, (N-c0)/2 + 1});
				for (int l1 = 0; c0 + 1 + l1 <= N; l1++) {
					for (int c1 = l1 & 1; c1 <= l1; c1 += 2) {
						num o = ans[{m-1, l1, c1}];
						rhs[{l1, (l1 - c1) / 2}] += o;
					}
				}
				tensor<num, 2> sum({N - c0, (N-c0)/2 + 1});
				// TODO: CONVOLVE FAST!
				for (int la = 0; la <= N - c0 - 1; la++) {
					for (int ga = 0; ga <= la / 2 && ga <= (N - c0) / 2; ga++) {
						for (int lb = 0; la + lb <= N - c0 - 1; lb++) {
							for (int gb = 0; gb <= lb / 2 && ga + gb <= (N - c0) / 2; gb++) {
								int lab = (la + lb);
								int gab = (ga + gb);
								sum[{lab, gab}] += lhs[{la, ga}] * rhs[{lb, gb}];
							}
						}
					}
				}
				// add 1 + c0 to the length at the end
				for (int lab = 0; lab <= N - c0 - 1; lab++) {
					for (int gab = 0; gab <= (N - c0) / 2; gab++) {
						int nl = lab + (1 + c0);
						int na1 = nl - l0a0 - 1;
						if (na1 < 0) continue;
						int na0 = na1 - 2 * gab;
						if (na0 < 0) na0 = na1 & 1;
						nxt_dp[{nl, na0, na1}] += sum[{lab, gab}];
					}
				}
			}
			cur_dp = std::move(nxt_dp);
		}
		for (int i = 0; i <= N; i++) {
			for (int c = 0; c <= i; c++) {
				cerr << ans[{m, i, c}] << ' ';
			}
			cerr << '\n';
		}
	}

	int T; cin >> T;
	while (T--) {
		int n, m; cin >> n >> m;
		for (int k = 0; 2 * k < n; k++) {
			cout << ans[{m, n, n-2*k}] << " \n"[2*k+2 >= n];
		}
	}

	return 0;
}

// The main DP is:
