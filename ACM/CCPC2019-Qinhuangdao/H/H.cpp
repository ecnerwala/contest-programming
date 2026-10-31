#include<bits/stdc++.h>
using namespace std;

using ll = long long;

namespace kactl {
typedef unsigned long long ull;
typedef long double ld;
ull mod_mul(ull a, ull b, ull M) {
	ll ret = a * b - M * ull(ld(a) * ld(b) / ld(M));
	return ret + M * (ret < 0) - M * (ret >= (ll)M);
}
ull mod_pow(ull b, ull e, ull mod) {
	ull ans = 1;
	for (; e; b = mod_mul(b, b, mod), e /= 2)
		if (e & 1) ans = mod_mul(ans, b, mod);
	return ans;
}
bool isPrime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return n - 2 < 2;
	ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n >> s;
	for(auto &a : A) {   // ^ count trailing zeroes
		ull p = mod_pow(a, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = mod_mul(p, p, n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
ull pollard(ull n) {
	auto f = [n](ull x) { return (mod_mul(x, x, n) + 1) % n; };
	if (!(n & 1)) return 2;
	for (ull i = 2;; i++) {
		ull x = i, y = f(x), p;
		while ((p = __gcd(n + y - x, n)) == 1)
			x = f(x), y = f(f(y));
		if (p != n) return p;
	}
}
vector<ull> factor(ull n) {
	if (n == 1) return {};
	if (isPrime(n)) return {n};
	ull x = pollard(n);
	auto l = factor(x), r = factor(n / x);
	l.insert(l.end(), r.begin(), r.end());
	return l;
}
} // namespace kactl

const int MAXN = 1.1e5;
int N;
ll MOD;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> MOD;
	map<ll, int> pfacs;
	for (ll p : kactl::factor(MOD-1)) {
		pfacs[p]++;
	}

	vector<ll> weights;
	weights.push_back(1);
	{
		int s = 1;
		for (auto it : pfacs) {
			ll p = it.first;
			int e = it.second;

			ll p2 = kactl::mod_mul(p, p, MOD);
			ll invp2 = kactl::mod_pow(p2, MOD-2, MOD);
			assert(kactl::mod_mul(p2, invp2, MOD) == 1);

			ll w = invp2 - 1;
			for (int i = 0; i < e; i++, w = kactl::mod_mul(w, invp2, MOD)) {
				for (int a = 0; a < s; a++) {
					weights.push_back(kactl::mod_mul(weights[a], w, MOD));
				}
			}
			s *= e+1;
		}
	}

	vector<ll> sums(weights.size());

	for (int q = 0; q < N; q++) {
		ll a; cin >> a;
		ll pows[61];
		pows[0] = a;
		for (int i = 0; i < 60; i++) {
			pows[i+1] = kactl::mod_mul(pows[i], pows[i], MOD);
		}
		auto getPow = [&](ll e) {
			ll ans = 1;
			for (int i = 0; i <= 60; i++) {
				if (e & (1ll << i)) {
					ans = kactl::mod_mul(ans, pows[i], MOD);
				}
			}
			return ans;
		};
		ll ord = MOD-1;
		//assert(getPow(ord) == 1);
		int pos = 0;
		int s = 1;
		for (auto it : pfacs) {
			ll p = it.first;
			int e = it.second;

			int v = e;
			while (v && getPow(ord / p) == 1) {
				ord /= p;
				v--;
			}
			pos += v * s;
			s *= e+1;
		}
		sums[pos] += ord;
		if (sums[pos] >= MOD) sums[pos] -= MOD;
	}

	{
		int s = 1;
		for (auto it : pfacs) {
			int e = it.second;
			for (int i = int(sums.size()) - 1; i >= s; i--) {
				if (i / s % (e+1)) {
					sums[i-s] += sums[i];
					if (sums[i-s] >= MOD) sums[i-s] -= MOD;
				}
			}
			s *= e+1;
		}
	}

	ll ans = 0;
	for (int i = 0; i < int(weights.size()); i++) {
		ans += kactl::mod_mul(kactl::mod_mul(sums[i], sums[i], MOD), weights[i], MOD);
		if (ans >= MOD) ans -= MOD;
	}
	cout << ans << '\n';

	return 0;
}

// w convolve 1 = 1/x^2
// w = 1/x^2 convolve mu
// w(q = p^e) = 1/q^2 - 1/(q/p)^2 = (1 - p^2) / q^2
