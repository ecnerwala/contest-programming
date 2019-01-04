#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// first fast factor
// then it's sum of mu(d) mu(d2) * cnt(m / d / d2)
// cnt(m / d) = 2^(# divisors of m/d) - 1
// mu convolve mu = 1 if e = 0, -2 if e = 1, 1 if e = 2
//
mt19937_64 mt(30);

using ull = unsigned long long;
const int bits = 64-60;
const ull po = 1 << bits;
ull mod_mul(ull a, ull b, const ull &c) {
	ull x = a * (b & (po-1)) % c;
	while ((b >>= bits) > 0) {
		a = (a << bits) % c;
		x += (a * (b & (po - 1))) % c;
	}
	return x % c;
}
ull mod_pow(ull a, ull b, ull mod) {
	if (b == 0) return 1;
	ull res = mod_pow(a, b / 2, mod);
	res = mod_mul(res, res, mod);
	if (b & 1) return mod_mul(res, a, mod);
	return res;
}
bool MR(ull p) {
	if (p == 2) return true;
	if (p == 1 || p % 2 == 0) return false;
	ull s = p-1;
	while (s % 2 == 0) s /= 2;
	for(int i = 0; i < 30; i++) {
		ull a = mt() % (p-1) + 1, tmp = s;
		ull mod = mod_pow(a, tmp, p);
		while (tmp != p-1 && mod != 1 && mod != p-1) {
			mod = mod_mul(mod, mod, p);
			tmp *= 2;
		}
		if (mod != p-1 && tmp % 2 == 0) return false;
	}
	return true;
}

ll M;
vector<ll> exps;
const ll MOD = 998244353;
ll pow2(ll a) {
	a %= MOD-1;
	if (a < 0) a += MOD-1;
	ll r = 1;
	ll base = 2;
	while (a) {
		if (a & 1) r *= base, r %= MOD;
		base *= base, base %= MOD;
		a /= 2;
	}
	return r;
}
ll ans = 0;
void dfs(size_t ind = 0, ll cur_facs = 1, ll sign = 1) {
	cur_facs %= MOD-1;
	sign %= MOD;
	if (ind == exps.size()) {
		ans += sign * (pow2(cur_facs) - 1);
		ans %= MOD;
		return;
	}
	assert(exps[ind] >= 1);
	dfs(ind+1, cur_facs * (exps[ind] + 1) % (MOD-1), sign);
	dfs(ind+1, cur_facs * (exps[ind]) % (MOD-1), -2 * sign);
	if (exps[ind] >= 2) dfs(ind+1, cur_facs * (exps[ind] - 1), sign);
}
int main() {
	ios_base::sync_with_stdio(0);
	cin >> M;
	for (ll p = 2; p * p * p <= M; p++) {
		if (M % p == 0) {
			ll e = 0;
			while (M % p == 0) {
				e++;
				M /= p;
			}
			exps.push_back(e);
		}
	}
	if (M > 1) {
		// check square
		ll rt = ll(sqrt((long double)(M)));
		while (rt * rt < M) rt++;
		while (rt * rt > M) rt--;
		if (rt * rt == M) {
			exps.push_back(2);
			M = 1;
		}
	}
	if (M > 1) {
		// check semiprime
		if (MR(ull(M))) {
			// prime
			exps.push_back(1);
		} else {
			// semiprime
			exps.push_back(1);
			exps.push_back(1);
		}
	}
	//for (auto e : exps) {cerr << e << ' ';} cerr << '\n';
	dfs();
	ans %= MOD;
	if (ans < 0) ans += MOD;
	cout << ans << '\n';

	return 0;
}
