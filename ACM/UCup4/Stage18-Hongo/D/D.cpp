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
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
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
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		if constexpr (MOD == (int64_t(1) << 31) - 1) {
		}
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

struct mod_goldilocks {
	static constexpr uint64_t MOD = 0xffffffff00000001ull;
	static constexpr uint64_t EPS = -MOD;
	// We have 2^32 is a primitive 6th root of unity.
	// Note that omega_8 + omega_8^7 == 2^24 - 2^72 == sqrt(2)
	// We'll pick the root so that 2^24 - 2^72 is our primitive 384th root of unity.
	static constexpr uint64_t PRIMITIVE_ROOT = 2717;
	struct is_reduced_tag {};
	uint64_t v;
	mod_goldilocks() : v(0) {}
	mod_goldilocks(uint64_t v_, is_reduced_tag) : v(v_) { assert(v < MOD); }
	mod_goldilocks(int64_t a) : v(a < 0 ? a+MOD : a) {}
	mod_goldilocks(int a) : mod_goldilocks(int64_t(a)) {}
	mod_goldilocks(uint64_t a) : v(a >= MOD ? a-MOD : a) {}
	mod_goldilocks(unsigned a) : mod_goldilocks(uint64_t(a)) {}
	mod_goldilocks(__int128_t a) : v(a % MOD < 0 ? uint64_t(MOD - a % MOD) : uint64_t(a % MOD)) {}
	mod_goldilocks(__uint128_t a) : v(uint64_t(a % MOD)) {}

	static mod_goldilocks from_reduced(uint64_t v) {
		return mod_goldilocks(v, is_reduced_tag{});
	}

	explicit operator uint64_t () const { return v; }
	int64_t as_signed() const { return MOD-v > v ? v : int64_t(v - MOD); }
	friend std::ostream& operator << (std::ostream& out, mod_goldilocks n) { return out << uint64_t(n); }

	friend bool operator == (mod_goldilocks a, mod_goldilocks b) { return a.v == b.v; }
	friend bool operator != (mod_goldilocks a, mod_goldilocks b) { return a.v != b.v; }

	mod_goldilocks operator+ () const { return *this; }

	// returns a-b, assuming -MOD <= a-b, e.g. b <= MOD
	static uint64_t sub_mod_raw(uint64_t a, uint64_t b) {
#if defined(__x86_64__)
		// TODO: We could try to write this using intrinsics, but GCC sometimes produces the wrong code.
		uint64_t res_wrapped = a;
		uint64_t adjustment = b;
		asm (
			// AT&T syntax: SRC DST
			"sub %[y], %[x]\n\t"
			// Trick from plonky2 implementation:
			// After sub, flag CF is set iff we underflowed. We want to correct by EPS == 2^32 - 1 iff C is set.
			// sbb (subtract with borrow) computes DST <- DST - SRC - CF
			// Thus, we can use the 32-bit form of sbb on a dummy register to load CF ? EPS : 0.
			// Here, we'll just reuse the original register holding b.
			"sbb %k[y], %k[y]\n\t"
			: [x] "+r"(res_wrapped),
			[y] "+r"(adjustment)
			:
			: "cc"
		);
#else
		uint64_t res_wrapped = a - b;
		uint64_t adjustment = (res_wrapped > a) ? EPS : 0;
#endif
		return res_wrapped - adjustment;
	}

	// Reduce lo + 2^64 * mi + 2^96 * hi, where hi <= MOD
	static uint64_t reduce_u160_raw(uint64_t lo, uint32_t mi, uint64_t hi) {
		// result = lo - hi + EPS * mi
		// 0 <= lo <= 2^64 - 1 = MOD + EPS - 1
		// 0 <= EPS * mi <= (2^32 - 1) * EPS = MOD - 1 - EPS
		// 0 <= hi <= MOD
		// -MOD <= lo - hi + EPS * mi <= 2*MOD-2
		// so we do have some leeway
		return sub_mod_raw(sub_mod_raw(lo, hi), MOD-(uint64_t(mi)<<32)+mi);
	}

	static uint64_t reduce_u128_raw(__uint128_t v) {
		uint64_t hi = uint64_t(v >> 64);
		uint64_t lo = uint64_t(v);
		uint32_t hi_hi = uint32_t(hi >> 32);
		uint32_t hi_lo = uint32_t(hi);
		return reduce_u160_raw(lo, hi_lo, hi_hi);
	}

	mod_goldilocks neg() const { return from_reduced(v ? MOD-v : 0); }
	friend mod_goldilocks neg(const mod_goldilocks& m) { return m.neg(); }
	mod_goldilocks operator- () const { return neg(); }

	mod_goldilocks& operator ++ () {
		++ v;
		if (v == MOD) v = 0;
		return *this;
	}
	mod_goldilocks& operator -- () {
		if (v == 0) v = MOD;
		-- v;
		return *this;
	}
	mod_goldilocks& operator += (mod_goldilocks o) {
		v = sub_mod_raw(v, MOD-o.v);
		return *this;
	}
	mod_goldilocks& operator -= (mod_goldilocks o) {
		v = sub_mod_raw(v, o.v);
		return *this;
	}
	mod_goldilocks& operator *= (mod_goldilocks o) {
		v = reduce_u128_raw(__uint128_t(v) * __uint128_t(o.v));
		return *this;
	}

	friend mod_goldilocks operator ++ (mod_goldilocks& a, int) { mod_goldilocks r = a; ++a; return r; }
	friend mod_goldilocks operator -- (mod_goldilocks& a, int) { mod_goldilocks r = a; --a; return r; }
	friend mod_goldilocks operator + (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) += b; }
	friend mod_goldilocks operator - (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) -= b; }
	friend mod_goldilocks operator * (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) *= b; }

	mod_goldilocks inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend mod_goldilocks inv(mod_goldilocks m) { return m.inv(); }
	mod_goldilocks& operator /= (mod_goldilocks o) {
		return *this *= o.inv();
	}
	friend mod_goldilocks operator / (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) /= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);


	const int MOD = int((int64_t(1) << 31) - 1);
	using num = modnum<MOD>;
	const int L = 2370291;
	const num RT = 7;
	const num G = power(RT, (MOD-1) / L);
	const num invG = inv(G);
	// 2370291
	assert(L % 11 == 0);

	auto fact = [&](int i) -> num {
		num r = 1;
		for (int z = 1; z <= i; z++) r *= num(z);
		return r;
	};
	auto solve = [&](std::array<int, 10> freq) -> num {
		int tot = std::accumulate(freq.begin(), freq.end(), 0);
		assert(tot <= L);
		tensor<num, 2> vals({L, 11}, num(1));
		num w0 = fact(tot / 2) * fact((tot + 1) / 2);
		num w1 = 0;
		if (freq[0]) {
			w1 = fact((tot - 1) / 2) * fact(tot / 2);
		}
		for (int d = 0; d < 10; d++) {
			w0 /= fact(freq[d]);
			w1 /= fact((d == 0 && freq[0]) ? freq[0] - 1 : freq[d]);
		}
		// 1/L * 1/11
		// sum over 
		// product of (1/(omega^dk g^l) + omega^dk g^l)^freq[d]
		num Gi = 1;
		num invGi = 1;
		for (int i = 0; i < L; i++, Gi *= G, invGi *= invG) {
			for (int d = 0; d < 10; d++) {
				num a;
				if (d == 0) {
					if (freq[0]) {
						a = power(Gi + invGi, freq[0] - 1);
						a *= w0 * (Gi + invGi) * ((tot & 1) ? invGi : 1) - w1 * ((tot & 1) ? 1 : invGi);
					} else {
						a = power(Gi + invGi, freq[0]) * w0 * ((tot & 1) ? invGi : 1);
					}
				} else {
					a = power(Gi + invGi, freq[d]);
				}
				for (int k = 0; k < 11; k++) {
					int j = i - (d * k) % 11 * (L / 11);
					if (j < 0) j += L;
					if (d == 0) assert(j == i);
					vals[{j, k}] *= a;
				}
			}
		}
		num ans = 0;
		for (int i = 0; i < L; i++) {
			for (int k = 0; k < 11; k++) {
				ans += vals[{i, k}];
			}
		}
		ans /= num(11) * num(L);
		return ans;
	};

	std::string S; std::cin >> S;
	std::array<int, 10> freq; freq.fill(0);
	for (char c : S) {
		assert('0' <= c && c <= '9');
		freq[c-'0']++;
	}
	num ans = solve(freq);
	std::cout << ans << '\n';

	return 0;
}
