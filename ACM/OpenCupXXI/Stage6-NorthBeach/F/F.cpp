#include <bits/stdc++.h>

typedef unsigned long long ull;
struct FastMod {
	ull b, m;
	FastMod(ull b_) : b(b_), m(-1ULL / b) {}
	ull reduce(ull a) { // a % b + (0 or b)
		ull v = a - (ull)((__uint128_t(m) * a) >> 64) * b; return (v >= b ? v - b : v);
	}
};

template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");
	static inline FastMod fm = FastMod(MOD);
	static void setmod(int M) {
		assert(M > 0);
		MOD = M;
		fm = FastMod(MOD);
	}

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
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
		v = int(fm.reduce(ll(v) * ll(o.v)));
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

int MOD = 1;
using num = modnum<MOD>;

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	std::vector<int64_t> fib; fib.reserve(100);
	fib.push_back(0);
	fib.push_back(1);

	int64_t N; int Q; int M;
	while (std::cin >> N >> Q >> M) {
		num::setmod(M);

		using poly_t = std::array<num, 2>;
		auto mul = [&](poly_t a, poly_t b) -> poly_t {
			num lo = a[0] * b[0];
			num hi = a[1] * b[1];
			num all = (a[0] + a[1]) * (b[0] + b[1]);
			return {lo + hi, all - lo};
		};
		std::vector<poly_t> fibo_poly; fibo_poly.reserve(64);
		fibo_poly.push_back(poly_t{num(0), num(1)});

		// returns fib[k-1] and fib[k]
		auto get_fibs = [&](int64_t k) -> poly_t {
			assert(k >= 0);
			poly_t res({num(1), num(0)});
			for (; k; k &= k-1) {
				int v = __builtin_ctzll(k);
				while (v >= int(fibo_poly.size())) fibo_poly.push_back(mul(fibo_poly.back(), fibo_poly.back()));
				res = mul(res, fibo_poly[v]);
			}
			return res;
		};
		auto get_fib = [&](int64_t k) -> num {
			assert(k >= 0);
			return get_fibs(k)[1];
		};

		auto solve = [&](const int64_t P) -> num {
			// just compute a couple extra
			while (fib.end()[-2] <= P) fib.push_back(fib.end()[-2] + fib.end()[-1]);

			assert(P >= 1);
			if (N % 2 == 1) {
				// ends in A, so it's relatively clean

				int64_t cover_N = 1;
				while (cover_N < N && fib[cover_N] < P) cover_N += 2;

				num weight_b, weight_a;
				{ auto f = get_fibs(N - cover_N + 2); weight_b = f[0], weight_a = f[1]; }
				num resp = weight_a - 1;

				auto preimage = [&]() {
					std::tie(weight_a, weight_b) = std::make_pair(weight_a + weight_b, weight_a);
				};

				for (int64_t cur_P = P, cur_N = cover_N; cur_N > 1; cur_N -= 2) {
					//assert(1 <= cur_P && cur_P <= fib[cur_N+1]);
					if (cur_P <= fib[cur_N]) {
						// starts with A
						preimage();
						if (cur_P <= fib[cur_N-2]) {
							// starts with B->AA
							resp += weight_a;
						} else {
							// starts with A->AB
							cur_P -= fib[cur_N-2];
						}
						preimage();
					} else {
						// starts with B
						cur_P -= fib[cur_N];
						resp += weight_a;
						preimage();
						preimage();
					}
					assert(1 <= cur_P && cur_P <= fib[cur_N-1]);
				}

				return resp;
			} else if (N == 0) {
				assert(P == 1);
				return 0;
			} else {
				assert(N >= 2);

				if (P <= N / 2 - 1) {
					// Special case so that we can skip forwards
					// P iteration of f(f().cut)
					//  --> wb = F[2*P+1], wa = F[2*P+2], resp = F[2*P+2]-1
					// 1 iterations of f(f())
					//  --> wb = F[2*(P+1)+1], wa = F[2*(P+1)+2], resp = F[2*P+2]-1
					// N/2-1-P iterations of f(f()).cut
					//  --> resp = F[N] + F[N-2] + + ... + F[2*(P+1)+2] + F[2*P+2] - 1
					return get_fib(N+1) - get_fib(2*P+1) - 1;
				}

				int cover_N = 0;
				while (cover_N < N && (N-2-cover_N)/2 + fib[cover_N] < P) {
					cover_N += 2;
				}
				num weight_b, weight_a;
				{ auto f = get_fibs(N - cover_N + 2); weight_b = f[0], weight_a = f[1]; }
				num resp = weight_a - 1;
				auto preimage = [&]() {
					std::tie(weight_a, weight_b) = std::make_pair(weight_a + weight_b, weight_a);
				};

				for (int64_t cur_P = (cover_N == N ? P : P - (N-2-cover_N) / 2 + 1), cur_N = cover_N; cur_N > 0; cur_N -= 2) {
					//assert(1 <= cur_P && cur_P <= fib[cur_N+1]);
					if (cur_N == N) {
						if (cur_P <= fib[cur_N]) {
							// starts with A
							preimage();

							// by construction of cover_N
							assert(cur_P > fib[cur_N-2]);
							// starts with A->AB
							cur_P -= fib[cur_N-2];

							preimage();
						} else {
							// starts with B
							cur_P -= fib[cur_N];
							resp += weight_a;
							preimage();
							preimage();
						}
					} else {
						if (cur_P == 1 || cur_P > 1 + fib[cur_N]) {
							// starts with B
							if (cur_P > 1 + fib[cur_N]) cur_P -= fib[cur_N];
							resp += weight_a;
							preimage();
							preimage();
						} else {
							// starts with A
							cur_P --;
							preimage();
							if (cur_P == 1 || cur_P > 1 + fib[cur_N-2]) {
								// starts with A->AB
								if (cur_P > 1 + fib[cur_N-2]) cur_P -= fib[cur_N-2];
							} else {
								// starts with B->AA
								resp += weight_a;
							}
							preimage();
						}
					}
					assert(1 <= cur_P && cur_P <= fib[cur_N-1]);
				}
				return resp;
			}
		};

		for (int q = 0; q < Q; q++) {
			int64_t P; std::cin >> P;
			num ans = solve(P) + 1; // 1-index the suffixes
			std::cout << ans << " \n"[q+1==Q];
		}
	}
}

/*

In level N, the number of a's is fib[N], b's is fib[N-1]

first iter
* = a* + b*
a* = f(f(a*).cut) + f(f(*))
b* = f(f(*)).cut

second iter onwards:
* = b$ + a* + b?
b$ = f(f(b$)).cut
a* = f(f(b$)) + f(f(a*).cut) + f(f(a*)) + f(f(b?))
b? = f(f(a*)).cut + f(f(b?)).cut

*/
