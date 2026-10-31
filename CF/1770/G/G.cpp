#include <bits/stdc++.h>

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

/**
 * Author: Andrew He
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 */

namespace ecnerwala {
namespace fft {

using std::swap;
using std::vector;
using std::min;
using std::max;

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }
inline int nextPow2(int s) { return 1 << (s > 1 ? 32 - __builtin_clz(s-1) : 0); }

// Complex
template <typename dbl> struct cplx { /// start-hash
	dbl x, y;
	cplx(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
	friend cplx operator+(cplx a, cplx b) { return cplx(a.x + b.x, a.y + b.y); }
	friend cplx operator-(cplx a, cplx b) { return cplx(a.x - b.x, a.y - b.y); }
	friend cplx operator*(cplx a, cplx b) { return cplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	friend cplx conj(cplx a) { return cplx(a.x, -a.y); }
	friend cplx inv(cplx a) { dbl n = (a.x*a.x+a.y*a.y); return cplx(a.x/n,-a.y/n); }
};

// getRoot implementations
template <typename num> struct getRoot {
	static num f(int k) = delete;
};
template <typename dbl> struct getRoot<cplx<dbl>> {
	static cplx<dbl> f(int k) {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
		dbl a=2*M_PI/k;
		return cplx<dbl>(cos(a),sin(a));
	}
};
template <int MOD> struct primitive_root {
	static const int value;
};
template <> struct primitive_root<998244353> {
	static const int value = 3;
};
template <int MOD> struct getRoot<modnum<MOD>> {
	static modnum<MOD> f(int k) {
		assert((MOD-1)%k == 0);
		return pow(modnum<MOD>(primitive_root<MOD>::value), (MOD-1)/k);
	}
};

template <typename num> class fft {
	static vector<int> rev;
	static vector<num> rt;

public:
	static void init(int n);
	template <typename Iterator> static void go(Iterator begin, int n);

	static vector<num> scratch_a;
	static vector<num> scratch_b;
};

template <typename num> vector<int> fft<num>::rev({0,1});
template <typename num> vector<num> fft<num>::rt(2, num(1));
template <typename num> vector<num> fft<num>::scratch_a;
template <typename num> vector<num> fft<num>::scratch_b;

template <typename num> void fft<num>::init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	}
	rt.reserve(n);
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
		num z = getRoot<num>::f(2*k);
		for (int i = k/2; i < k; i++) {
			rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
		}
	}
}

template <typename num> template <typename Iterator> void fft<num>::go(Iterator begin, int n) {
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	for (int i = 0; i < n; i++) {
		if (i < (rev[i]>>s)) {
			swap(*(begin+i), *(begin+(rev[i]>>s)));
		}
	}
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i < n; i += 2 * k) {
			Iterator it1 = begin + i, it2 = it1 + k;
			for (int j = 0; j < k; j++, ++it1, ++it2) {
				num t = rt[j+k] * *it2;
				*it2 = *it1 - t;
				*it1 = *it1 + t;
			}
		}
	}
}

template <typename num> struct fft_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<num>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);
		copy(fa.begin(), fa.begin()+s, io);
	}
};

template <typename num>
struct fft_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0) return;
		int s = nextPow2(sza) * 2;
		fft<num>::init(s);
		if (sz(fa) < s) fa.resize(s);
		if (sz(fb) < s) fb.resize(s);
		fb[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			fill(fb.begin() + n, fb.begin() + 4 * n, num(0));
			n *= 2;
			copy(ia, ia+min(n,sza), fa.begin());
			fill(fa.begin()+min(n,sza), fa.begin()+2*n, 0);
			fft<num>::go(fb.begin(), 2*n);
			fft<num>::go(fa.begin(), 2*n);
			num d = inv(num(2*n));
			for (int i = 0; i < 2*n; i++) fb[i] = fb[i] * (2 - fa[i] * fb[i]) * d;
			reverse(fb.begin()+1, fb.begin()+2*n);
			fft<num>::go(fb.begin(), 2*n);
		}
		copy(fb.begin(), fb.begin()+sza, io);
	}
};

template <typename dbl>
struct fft_double_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<cplx<dbl>>& fa = fft<cplx<dbl>>::scratch_a;
		vector<cplx<dbl>>& fb = fft<cplx<dbl>>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cplx<dbl>>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		fill(fa.begin(), fa.begin() + n, 0);
		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i].x = *it; }
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fa[i].y = *it; }
		fft<cplx<dbl>>::go(fa.begin(), n);
		for (auto& x : fa) x = x * x;
		for (int i = 0; i < n; ++i) fb[i] = fa[(n-i)&(n-1)] - conj(fa[i]);
		fft<cplx<dbl>>::go(fb.begin(), n);
		{ auto it = io; for (int i = 0; i < s; ++i, ++it) *it = fb[i].y / (4*n); }
	}
};

template <typename mnum>
struct fft_mod_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		using cnum = cplx<double>;
		vector<cnum>& fa = fft<cnum>::scratch_a;
		vector<cnum>& fb = fft<cnum>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cnum>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fa.begin()+sza, fa.begin() + n, 0);
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fb[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fb.begin()+szb, fb.begin() + n, 0);

		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		double r0 = 0.5 / n; // 1/2n
		for (int i = 0; i <= n/2; i++) {
			int j = (n-i)&(n-1);
			cnum g0 = (fb[i] + conj(fb[j])) * r0;
			cnum g1 = (fb[i] - conj(fb[j])) * r0;
			swap(g1.x, g1.y); g1.y *= -1;
			if (j != i) {
				swap(fa[j], fa[i]);
				fb[j] = fa[j] * g1;
				fa[j] = fa[j] * g0;
			}
			fb[i] = fa[i] * conj(g1);
			fa[i] = fa[i] * conj(g0);
		}
		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		using ll = long long;
		const ll m = mnum::MOD;
		auto it = io;
		for (int i = 0; i < s; ++i, ++it) {
			*it = mnum((ll(fa[i].x+0.5)
						+ (ll(fa[i].y+0.5) % m << 15)
						+ (ll(fb[i].x+0.5) % m << 15)
						+ (ll(fb[i].y+0.5) % m << 30)) % m);
		}
	}
};

template <class multiplier, typename num>
struct multiply_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		if (sza == 0) return;
		int s = nextPow2(sza);
		vector<num> b(s,num(0));
		vector<num> tmp(2*s);
		b[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			// TODO: could be square instead of multiply
			multiplier::multiply(b.begin(),n,b.begin(),n,tmp.begin());
			int nn = min(sza,2*n);
			multiplier::multiply(tmp.begin(),nn,ia,nn,tmp.begin());
			for (int i = n; i < nn; i++) b[i] = -tmp[i];
			n = nn;
		}
		copy(b.begin(), b.begin()+sza, io);
	}
};

template <class multiplier, typename T> vector<T> multiply(const vector<T>& a, const vector<T>& b) {
	if (sz(a) == 0 || sz(b) == 0) return {};
	vector<T> r(max(0, sz(a) + sz(b) - 1));
	multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
	return r;
}

template <typename T> vector<T> fft_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_double_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_double_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_mod_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_mod_multiplier<T>, T>(a, b);
}

template <class inverser, typename T> vector<T> inverse(const vector<T>& a) {
	vector<T> r(sz(a));
	inverser::inverse(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_inverse(const vector<T>& a) {
	return inverse<fft_inverser<T>, T>(a);
}
template <typename T> vector<T> fft_double_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_double_multiplier<T>, T>, T>(a);
}
template <typename T> vector<T> fft_mod_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_mod_multiplier<T>, T>, T>(a);
}

}} // namespace ecnerwala::fft

using num = modnum<998244353>;

num solve(std::string S) {
	//using std::cerr;
	//cerr << "solve " << S << '\n';
	std::vector<int> heights; heights.reserve(S.size()+1);
	{
		int pref = 0;
		int cur_h = 0;
		for (int i = 0; i < int(S.size()); i++) {
			if (S[i] == '(') {
				pref++;
			} else {
				pref--;
				if (pref < 0) {
					pref = 0;
					cur_h--;
				} else {
					// add a rightwards edge
					heights.push_back(cur_h);
				}
			}
		}
		for (auto& h : heights) {
			h -= cur_h;
		}
		heights.push_back(0);
		heights.push_back(0);
	}
	//for (auto h : heights) { cerr << h << ' '; } cerr << '\n';

	int V = int(S.size())+10;
	std::vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto cross_ways = [&](std::vector<num> in, std::vector<num>& out) {
		if (in.empty()) return;
		std::reverse(in.begin(), in.end());
		for (int i = 0; i < int(in.size()); i++) {
			in[i] *= ifact[i];
		}
		std::reverse(in.begin(), in.end());
		std::vector<num> tmp(fact.begin(), fact.begin() + int(in.size()) + int(out.size()));
		auto v = ecnerwala::fft::fft_multiply(in, tmp);
		for (int i = 0; i < int(out.size()); i++) {
			out[i] += v[int(in.size())-1+i] * ifact[i];
		}
	};

	auto parallel_ways = [&](std::vector<num> in, std::vector<num>& out, int dist) {
		assert(dist >= 0);
		assert(in.size() == out.size());
		if (in.empty()) return;
		if (dist == 0) {
			for (int i = 0; i < int(in.size()); i++) {
				out[i] += in[i];
			}
			return;
		}
		//cerr << "parallel " << dist << '\n';
		//for (int i = 0; i < int(in.size()); i++) { cerr << in[i] << ' '; } cerr << '\n';
		std::vector<num> tmp(in.size());
		for (int i = 0; i < int(tmp.size()); i++) {
			tmp[i] = fact[dist - 1 + i] * ifact[i] * ifact[dist - 1];
		}
		auto v = ecnerwala::fft::fft_multiply(in, tmp);
		//for (int i = 0; i < int(in.size()); i++) { cerr << v[i] << ' '; } cerr << '\n';
		for (int i = 0; i < int(out.size()); i++) {
			out[i] += v[i];
		}
	};

	// lefts are listed top to bottom
	auto final_ways = std::y_combinator([&](auto self, int l, int r, std::vector<num> lefts) -> std::vector<num> {
		assert(l < r);
		assert(int(lefts.size()) == heights[l] - heights[r]);
		if (r - l == 1) {
			return std::vector<num>({std::accumulate(lefts.begin(), lefts.end(), num(0))});
		}
		int m = (l+r)/2;
		assert(l < m && m < r);
		auto tops = self(l, m, std::vector<num>(lefts.begin(), lefts.begin() + (heights[l] - heights[m])));
		//cerr << "solving " << l << '-' << r << '\n';
		//for (int i = l; i <= r; i++) cerr << heights[i] << '\n';
		lefts.erase(lefts.begin(), lefts.begin() + (heights[l] - heights[m]));

		//cerr << "lefts "; for (auto v : lefts) { cerr << v << ' '; } cerr << '\n';
		//cerr << "tops "; for (auto v : tops) { cerr << v << ' '; } cerr << '\n';

		std::vector<num> rights(heights[m] - heights[r]);
		std::vector<num> bots; bots.reserve(r-l);
		bots.resize(m-l);

		parallel_ways(lefts, rights, int(tops.size()));
		parallel_ways(tops, bots, int(lefts.size()));
		cross_ways(lefts, bots);
		cross_ways(tops, rights);

		//cerr << "rights "; for (auto v : rights) { cerr << v << ' '; } cerr << '\n';
		//cerr << "bots "; for (auto v : bots) { cerr << v << ' '; } cerr << '\n';

		auto r_bots = self(m, r, rights);
		bots.insert(bots.end(), r_bots.begin(), r_bots.end());
		return bots;
	})(0, int(heights.size())-1, std::vector<num>(heights[0], num(1)));
	return std::accumulate(final_ways.begin(), final_ways.end(), num(1));
}

int slow(std::string S) {
	int N = int(S.size());
	int min_cnt = N+1;
	int ways = 0;
	for (int m = 0; m < (1 << N); m++) {
		bool good = true;
		int cnt = 0;
		int pref = 0;
		for (int i = 0; i < N; i++) {
			if (m & (1 << i)) {
				pref += (S[i] == '(' ? +1 : -1);
				if (pref < 0) good = false;
			} else {
				cnt++;
			}
		}
		if (good) {
			if (cnt < min_cnt) {
				min_cnt = cnt, ways = 0;
			}
			if (cnt == min_cnt) ways++;
		}
	}
	return ways;
}

void test() {
	for (int N = 1; N <= 10; N++) {
		for (int m = 0; m < (1 << N); m++) {
			std::string S;
			for (int i = 0; i < N; i++) {
				S += (m & (1 << i)) ? '(' : ')';
			}
			int slo = slow(S);
			num res = solve(S);
			if (slo != res) {
				std::cerr << S << ' ' << slo << ' ' << res << '\n';
				exit(0);
			}
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	string S; cin >> S;

	num ans = solve(S);
	std::reverse(S.begin(), S.end());
	for (auto& c : S) c ^= '(' ^ ')';
	ans *= solve(S);
	cout << ans << '\n';

	return 0;
}
