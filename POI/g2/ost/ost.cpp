#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll inv(ll a, ll m) { return a == 1 ? 1 : m - inv(m % a, a) * m / a; }

ll crt(ll a, ll p, ll b, ll q) {
	a %= p;
	b %= q;
	if (b < a) {
		swap(a,b);
		swap(p,q);
	}
	assert(a <= b);
	return a + (b - a) * inv(p, q) % q * p;
}

ll pow(ll a, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) r *= a;
		a *= a;
		e /= 2;
	}
	return r;
}

int main() {
	ll K, A; scanf("%lld %lld", &K, &A);
	ll p2 = pow(2, K);
	ll p5 = pow(5, K);
	vector<ll> res;
	if (A == 2) {
		// 0 or 1 mod 2^n, 0 or 1 mod 5^n
		res.push_back(crt(0, p2, 0, p5));
		res.push_back(crt(0, p2, 1, p5));
		res.push_back(crt(1, p2, 0, p5));
		res.push_back(crt(1, p2, 1, p5));
	} else if (A == 3) {
		// 0, (0|2^(n-1)) +-1 mod 2^n, 0 or 1 mod 5^n
		res.push_back(crt(0, p2, 0, p5));
		res.push_back(crt(0, p2, 1, p5));
		res.push_back(crt(0, p2, p5-1, p5));
		res.push_back(crt(1, p2, 0, p5));
		res.push_back(crt(1, p2, 1, p5));
		res.push_back(crt(1, p2, p5-1, p5));
		res.push_back(crt(p2/2-1, p2, 0, p5));
		res.push_back(crt(p2/2-1, p2, 1, p5));
		res.push_back(crt(p2/2-1, p2, p5-1, p5));
		res.push_back(crt(p2/2+1, p2, 0, p5));
		res.push_back(crt(p2/2+1, p2, 1, p5));
		res.push_back(crt(p2/2+1, p2, p5-1, p5));
		res.push_back(crt(p2-1, p2, 0, p5));
		res.push_back(crt(p2-1, p2, 1, p5));
		res.push_back(crt(p2-1, p2, p5-1, p5));
	} else assert(false);

	sort(res.begin(), res.end());
	res.resize(unique(res.begin(), res.end()) - res.begin());
	for (int i = 0; i < int(res.size()); i++) {
		if(i) cout << ' ';
		cout << res[i];
	}
	cout << '\n';

	return 0;
}
