#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXD = 2e7;
ll tmp[MAXD];

ll powmod(ll a, ll e, ll m) {
	assert(e >= 0);
	ll r = 1;
	while (e) {
		if (e & 1) r = r * a % m;
		a = a * a % m;
		e /= 2;
	}
	return r;
}

ll modinv(ll a, ll m) {
	assert(0 < a && a < m);
	return a == 1 ? 1 : (m - modinv(m % a, a) * m / a);
}

pair<ll, ll> facMod(ll n, ll p, ll e, ll q) {
	assert(n >= 0);
	assert(e >= 1);
	tmp[0] = 1;
	for (int i = 1; i <= q; i++) {
		if (i % p == 0) tmp[i] = tmp[i-1];
		else tmp[i] = tmp[i-1] * i % q;
	}
	ll dmod = 1;
	ll numP = 0;
	while (n) {
		dmod *= powmod(tmp[q-1], n / q, q);
		dmod %= q;
		dmod *= tmp[n % q];
		dmod %= q;
		numP += n / p;
		n /= p;
	}
	return {dmod, numP};
}

pair<ll, ll> chooseDMod(ll n, ll r, ll p, ll e, ll q) {
	auto num = facMod(n, p, e, q);
	auto den = facMod(r, p, e, q);
	{
		auto den_ = facMod(n-r, p, e, q);
		den.first *= den_.first;
		den.first %= q;
		den.second += den_.second;
	}
	num.first *= modinv(den.first, q);
	num.first %= q;
	num.second -= den.second;
	assert(num.second >= 0);
	while (num.second % e) {
		num.first *= p;
		num.first %= q;
		num.second--;
	}
	assert(num.second % e == 0);
	num.second /= e;
	return num;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll N, R, D; cin >> N >> R >> D;
	ll curV = 0;
	ll curM = 1;
	ll curCnt = 8e18;
	for (ll p = 2; D > 1; p++) {
		ll e = 0;
		ll q = 1;
		while (D % p == 0) {
			D /= p;
			q *= p;
			e++;
		}
		if (!e) continue;
		//cerr << p << '^' << e << ' ' << q << '\n';
		auto res = chooseDMod(N, R, p, e, q);
		//cerr << res.first << ' ' << res.second << '\n';
		if (curCnt > res.second) {
			curV = 0;
			curCnt = res.second;
		} else if (res.second > curCnt) {
			res.first = 0;
			res.second = curCnt;
		}
		assert(curCnt == res.second);
		if (curV) {
			curV *= powmod(modinv(q % curM, curM), curCnt, q);
			curV %= curM;
		}
		res.first *= powmod(modinv(curM % q, q), curCnt, q);
		res.first %= q;
		curV = curV + ((res.first - curV) % q + q) % q * modinv(curM % q, q) % q * curM;
		curM *= q;
		//cerr << curV << ' ' << curM << ' ' << curCnt << '\n';
	}

	cout << curV << '\n';
	return 0;
}
