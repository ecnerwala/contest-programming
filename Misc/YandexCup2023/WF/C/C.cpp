#include <bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

public:
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

public:
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

using num = modnum<998244353>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// We need EV of 1/2^{# of local mins}
	// For each column, one of the two is smaller (uniformly at random), the other doesn't matter
	// If two columns are the same direction, then they can influence each other
	// Otherwise, it's uniform
	int N; cin >> N;
	std::string R0, R1; cin >> R0 >> R1;

	num num_parities = 0;
	for (int tgt_z = 0; tgt_z < 2; tgt_z++) {
		bool bad = false;
		int num_empty = 0;
		for (int i = 0; i < N; i++) {
			if (R0[i] == '.' && R1[i] == '.') {
				num_empty++;
				continue;
			}
			for (int j = 0; j < 2; j++) {
				char c = (j ? R0 : R1)[i];
				if (c == '.') continue;
				if ((((c == 'B') ^ i ^ j) ^ tgt_z) & 1) {
					bad = true;
				}
			}
		}
		if (bad) continue;
		num_parities++;
	}
	num tot_ans = 0;
	tensor<num, 4> dp({1, 1, 2, 2}); // 0 means maybe local min, 1 means def not local min
	int cur_empty = 0;
	int cur_v0_has = false;
	int cur_v1_has = false;
	dp[{0, 0, 1, 1}] = num(1);
	num inv_2 = inv(num(2));
	std::vector<num> buf; buf.reserve(5 * N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 2; j++) {
			char c = (j ? R0 : R1)[i];
			//cerr << "step " << i << ' ' << j << ' ' << c << '\n';
			//cerr << " cur_empty " << cur_empty << ' ' << cur_v0_has << ' ' << cur_v1_has << '\n';
			int nxt_empty = cur_empty + (c == '.');
			int nxt_v0_has = (j == 0) ? (c == '.') : cur_v0_has;
			int nxt_v1_has = (j == 1) ? (c == '.') : cur_v1_has;
			tensor<num, 4> ndp({nxt_v0_has ? nxt_empty : 1, nxt_v1_has ? nxt_empty : 1, 2, 2});
			if (c == '.') {
				num denom = inv(num(nxt_empty));
				if (cur_v0_has && cur_v1_has) {
					if (j == 0) {
						for (int a0 = 0; a0 < 2; a0++) {
							for (int a1 = 0; a1 < 2; a1++) {
								for (int v1 = 0; v1 < cur_empty; v1++) {
										//cerr << "  " << v0 << ' ' << v1 << ' ' << a0 << ' ' << a1 << '\n';
									buf.clear();
									buf.push_back(0);
									for (int v0 = 0; v0 < cur_empty; v0++) {
										buf.push_back(buf.back() + dp[{v0, v1, a0, a1}] * denom);
									}

									for (int v_new = 0; v_new <= cur_empty; v_new++) {
										int nv1 = v1 + (v1 >= v_new);
										int na1 = a1;
										// v0 small
										{
											num nways = buf[v_new] - buf[0];
											int na0 = a0;
											int a_new = 0;
											a_new = 1;

											int nv0 = v_new;
											if (na0 == 0) nways *= inv_2;
											na0 = a_new;
											ndp[{nv0, nv1, na0, na1}] += nways;
										}
										// v0 big
										{
											num nways = buf.back() - buf[v_new];
											int na0 = a0;
											int a_new = 0;
											na0 = 1;

											int nv0 = v_new;
											if (na0 == 0) nways *= inv_2;
											na0 = a_new;
											ndp[{nv0, nv1, na0, na1}] += nways;
										}
									}
								}
							}
						}
					} else {
						for (int a0 = 0; a0 < 2; a0++) {
							for (int a1 = 0; a1 < 2; a1++) {
								for (int v0 = 0; v0 < cur_empty; v0++) {
									buf.clear();
									buf.push_back(0);
									for (int v1 = 0; v1 < cur_empty; v1++) {
										buf.push_back(buf.back() + dp[{v0, v1, a0, a1}] * denom);
									}
									for (int v_new = 0; v_new <= cur_empty; v_new++) {
										// v1 small
										{
											num nways = buf[v_new] - buf[0];
											int nv0 = v0 + (v0 >= v_new);
											int na0 = a0;
											int na1 = a1;
											int a_new = 0;

											if (v_new <= v0) {
												na0 = 1;
											} else {
												a_new = 1;
											}

											a_new = 1;
											int nv1 = v_new;
											if (na1 == 0) nways *= inv_2;
											na1 = a_new;
											ndp[{nv0, nv1, na0, na1}] += nways;
										}
										// v1 big
										{
											num nways = buf.back() - buf[v_new];
											int nv0 = v0 + (v0 >= v_new);
											int na0 = a0;
											int na1 = a1;
											int a_new = 0;

											if (v_new <= v0) {
												na0 = 1;
											} else {
												a_new = 1;
											}

											na1 = 1;
											int nv1 = v_new;
											if (na1 == 0) nways *= inv_2;
											na1 = a_new;
											ndp[{nv0, nv1, na0, na1}] += nways;
										}
									}
								}
							}
						}
					}
				} else {
					for (int v0 = 0; v0 < (cur_v0_has ? cur_empty : 1); v0++) {
						for (int v1 = 0; v1 < (cur_v1_has ? cur_empty : 1); v1++) {
							for (int a0 = cur_v0_has ? 0 : 1; a0 < 2; a0++) {
								for (int a1 = cur_v1_has ? 0 : 1; a1 < 2; a1++) {
									//cerr << "  " << v0 << ' ' << v1 << ' ' << a0 << ' ' << a1 << '\n';
									num ways = dp[{v0, v1, a0, a1}];
									//cerr << "  " << v0 << ' ' << v1 << ' ' << a0 << ' ' << a1 << ' ' << ways << '\n';
									if (ways == 0) continue;
									ways *= denom;
									for (int v_new = 0; v_new <= cur_empty; v_new++) {
										num nways = ways;
										int nv0 = cur_v0_has ? v0 + (v0 >= v_new) : 0;
										int nv1 = cur_v1_has ? v1 + (v1 >= v_new) : 0;
										int na0 = a0;
										int na1 = a1;
										int a_new = 0;
										if (j == 0) {
											if (cur_v0_has) {
												if (v_new < nv0) {
													na0 = 1;
												} else {
													a_new = 1;
												}
											} else if (i > 0) {
												a_new = 1;
											}

											nv0 = v_new;
											if (na0 == 0) nways *= inv_2;
											na0 = a_new;
										} else {
											if (cur_v0_has) {
												if (v_new < nv0) {
													na0 = 1;
												} else {
													a_new = 1;
												}
											} else {
												// above always exists
												a_new = 1;
											}
											if (cur_v1_has) {
												if (v_new < nv1) {
													na1 = 1;
												} else {
													a_new = 1;
												}
											} else if (i > 0) {
												a_new = 1;
											}

											nv1 = v_new;
											if (na1 == 0) nways *= inv_2;
											na1 = a_new;
										}
										ndp[{nv0, nv1, na0, na1}] += nways;
									}
								}
							}
						}
					}
				}
			} else {
				for (int v0 = 0; v0 < (cur_v0_has ? cur_empty : 1); v0++) {
					for (int v1 = 0; v1 < (cur_v1_has ? cur_empty : 1); v1++) {
						for (int a0 = 0; a0 < 2; a0++) {
							for (int a1 = 0; a1 < 2; a1++) {
								num ways = dp[{v0, v1, a0, a1}];
								if (ways == 0) continue;
								int nv0 = v0;
								int nv1 = v1;
								int na0 = a0;
								int na1 = a1;
								if (j == 0) {
									nv0 = 0;
									na0 = 1;
								} else {
									// we set na0 to 1 for free
									na0 = 1;
									nv1 = 0;
									na1 = 1;
								}
								ndp[{nv0, nv1, na0, na1}] += ways;
							}
						}
					}
				}
			}
			cur_empty = nxt_empty;
			dp = std::move(ndp);
			cur_v0_has = nxt_v0_has;
			cur_v1_has = nxt_v1_has;
		}
	}
	for (int v0 = 0; v0 < (cur_v0_has ? cur_empty : 1); v0++) {
		for (int v1 = 0; v1 < (cur_v1_has ? cur_empty : 1); v1++) {
			for (int a0 = 0; a0 < 2; a0++) {
				for (int a1 = 0; a1 < 2; a1++) {
					tot_ans += dp[{v0, v1, a0, a1}] * (a0 == 0 ? inv_2 : 1) * (a1 == 0 ? inv_2 : 1);
				}
			}
		}
	}
	tot_ans *= num_parities;

	cout << tot_ans << '\n';

	return 0;
}
