#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

typedef long long ll;

// reference implementation of a counter
struct counter {
	ll v;

	counter() : v(0) {}
	counter(ll v_) : v(v_) {}
	static counter sum_all(ll x) {
		assert(x >= 0);
		counter res;
		if (x & 1) {
			res.v = x * ((x+1) / 2);
		} else {
			res.v = (x/2) * (x+1);
		}
		return res;
	}

	counter& operator += (const counter &other) {
		v += other.v;
		return *this;
	}
	counter& operator -= (const counter &other) {
		v -= other.v;
		return *this;
	}
	counter& operator *= (const counter &other) {
		v *= other.v;
		return *this;
	}

	counter operator + (const counter &other) const {
		counter res = *this;
		res += other;
		return res;
	}
	counter operator - (const counter &other) const {
		counter res = *this;
		res -= other;
		return res;
	}
	counter operator * (const counter &other) const {
		counter res = *this;
		res *= other;
		return res;
	}
};
inline std::ostream& operator << (std::ostream &out, const counter &cnt) {
	return out << cnt.v;
}

// Compute pi(x) = \sum_{prime p<=x} T(p) for all x = \floor(N/a)
// T must implement all ring methods
// As a constructor function, T must be fully multiplicative and support efficiently finding things like sum_{i=1}^n T(i)
// T(x) = 1, T(x) = x^c supports this, as do some more complicated functions like T(x) = pair(x%4==1, x%4==3)
template <typename T> struct prime_count {

	static ll cbrt(ll a) {
		assert(a >= 0);
		ll r = 0;
		while (r * r * r <= a) r ++;
		r--;
		return r;
	}

	ll N;
	ll A;
	ll B;

	static const ll MAXA = 3e3;
	static const ll MAXB = 3e6;

	ll min_prime[MAXB+1];
	T pi_small[MAXB+1]; // precompute the phis

	ll P;
	ll primes[MAXB+1];

	ll PA;

	ll to_inv(ll x) {
		ll n = N / x;
		assert(x == N / n);
		assert(n <= A);
		return n;
	}

	T phi[MAXA+1]; // phi[i] = phi(N/i, PA-1)
	// where phi(x, p) = sum of values <= x with all prime factors > primes[p]

	/* Equivalent, slow calculations of phi
	T phi_slow(ll x, ll i) {
		if (x <= 0) return T();
		if (i < 0) {
			return T::sum_all(x);
		}
		T res = phi_slow(x, i-1) - T(primes[i]) * phi_slow(x / primes[i], i-1);
		return res;
	}

	T phi_slow2(ll x, ll i) {
		ll p = (i >= 0) ? primes[i] : 0;
		T res;
		for (ll v = 1; v <= x; v++) {
			if (min_prime[v] == 0 || min_prime[v] > p) {
				res += T(v);
			}
		}
		return res;
	}
	*/

	void compute_phi() {
		// binary-indexed tree
		T (*dp)[MAXA+1] = new T[MAXA+1][MAXA+1]; // dp[j][p] = sum T(i) for i <= N/j and min_prime[i] > p

		T *bit = new T[MAXA+1];
		for (ll i = 0, j = B; j > A; j--) {
			assert(N/j <= B);
			assert(i <= N/j);
			while (i < N/j) {
				i++;
				// insert i into the bit for all values <= min_prime[i]
				T val(i);
				for (ll v = (min_prime[i] == 0) ? A : min(min_prime[i], A); v; v -= v & (-v)) {
					bit[v] += val;
				}
			}
			assert(i == N/j);

			// now we iterate over prime factors of j
			for (ll cur = j; cur > 1; ) {
				ll p = min_prime[cur];
				assert(j % p == 0);
				assert(j > A);
				if (p > A) break;
				if (j / p <= A) {
					// number of values with min_prime >= p
					T phi_cur; // phi(N/j, p)
					for (ll v = p; v <= A; v += v & (-v)) {
						phi_cur += bit[v];
					}
					dp[j/p][p] -= T(p) * phi_cur;
				}
				while (cur % p == 0) cur /= p;
			}
		}
		delete[] bit;

		cerr << "second sieve done\n";

		for (ll j = 1; j <= A; j++) {
			dp[j][0] = T::sum_all(N/j);
		}

		for (ll i = 0; i < PA; i++) {
			ll q = (i > 0) ? primes[i-1] : 0;
			ll p = primes[i];
			for (ll j = 1; j <= A; j++) {
				dp[j][p] += dp[j][q];
				if (j * p <= A) {
					dp[j][p] -= T(p) * dp[j*p][q];
				} else {
					// our first pass covered this
				}
			}
		}

		for (ll j = 1; j <= A; j++) {
			phi[j] = dp[j][primes[PA-1]];
		}

		delete[] dp;
	}

	unordered_map<ll, T> mp;
	T p2_A(ll x) {
		if (mp.count(x)) return mp[x];

		assert(x <= N);
		T res;
		for (ll i = PA; i < P && primes[i] * primes[i] <= x; i++) {
			ll p = primes[i];
			assert(x/p >= p);
			res += T(p) * (pi(x/p) - pi(p-1));
		}

		return mp[x] = res;
	}

	T pi(ll x) {
		if (x <= B) {
			return pi_small[x];
		}
		T res = pi(A);
		//res += phi_slow(x, PA-1);
		res += phi[to_inv(x)];
		res -= T(1);
		res -= p2_A(x);
		return res;
	}

	T count(ll x) {
		return pi(x);
	}

	prime_count() {
	}
	void init(ll N_) {
		mp.clear();
		N = N_;
		A = cbrt(N);
		B = N / A;
		for (ll i = 0; i <= B; i++) {
			if (i <= 1) min_prime[i] = 0;
			else if (i % 2 == 0) min_prime[i] = 2;
			else min_prime[i] = i;
		}

		for (ll p = 3; p * p <= B; p++) {
			if (min_prime[p] != p) continue;
			for (ll j = p * p; j <= B; j += p) {
				if (min_prime[j] == j) min_prime[j] = p;
			}
		}
		cerr << "sieve done\n";

		pi_small[0] = T();
		P = 0;
		for (ll p = 1; p <= B; p++) {
			pi_small[p] = pi_small[p-1];

			if (min_prime[p] != p) continue;
			primes[P++] = p;
			pi_small[p] += T(p);
		}

		PA = 0;
		while (PA < P && primes[PA] <= A) PA++;
		assert(PA <= P);

		cerr << "primes gathered\n";

		compute_phi();

		cerr << "precompute done\n";
	}
};

prime_count<counter> pc;

bool isPrime[int(1e5)];

class SimpleMathProblemDiv1 {

public:

	long long calculate(long long X) {
		if (X == 1) return 0;

		pc.init(X);
		ll R = llround(floor(sqrt(X)));
		assert(R * R <= X);
		assert((R+1) * (R+1) > X);
		ll pR = pc.count(R).v;
		ll res = 0;
		for (ll i = 1; i <= R; i++) {
			// add in the large primes, greater than R
			// add them in once per multiple
			ll tot = pc.count(X / i).v;
			res += tot - pR;
		}
		for (ll i = 1; i <= R; i++) {
			isPrime[i] = true;
		}
		isPrime[1] = false;
		for (ll p = 2; p <= R; p++) {
			if (!isPrime[p]) continue;
			for (ll j = p + p; j <= R; j += p) {
				isPrime[j] = false;
			}
			ll prv = 0;
			for (ll q = p; q <= X; q *= p) {
				ll numRep = X / p - (q / p - 1);
				res += numRep * (q - prv);
				prv = q;
			}
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::SimpleMathProblemDiv1;

// vim:ft=cpp
