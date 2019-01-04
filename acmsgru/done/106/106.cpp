#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll gcd(ll a, ll b) {
	return a ? gcd(b % a, a) : b;
}

ll inv(ll a, ll m) {
	a %= m;
	return (a == 1) ? 1 : m - inv(m, a) * m / a;
}

ll go() {
	ll a, b, c, x1, x2, y1, y2;
	cin >> a >> b >> c >> x1 >> x2 >> y1 >> y2;

	// ax + by = c
	c *= -1;

	if (a < 0) {
		a *= -1;
		x1 *= -1;
		x2 *= -1;
		swap(x1, x2);
	}

	if (b < 0) {
		b *= -1;
		y1 *= -1;
		y2 *= -1;
		swap(y1, y2);
	}

	assert(a >= 0);
	assert(b >= 0);

	c -= a * x1;
	x2 -= x1;
	x1 -= x1;

	c -= b * y1;
	y2 -= y1;
	y1 -= y1;

	if (x2 < 0 || y2 < 0) return 0;
	assert(x2 >= 0);
	assert(y2 >= 0);

	if (!(0 <= c && c <= a * x2 + b * y2)) {
		return 0;
	}

	if (a == 0 && b == 0) {
		assert(c == 0);
		return (x2+1) * (y2+1);
	}

	ll g = gcd(a,b);
	if (c % g != 0) return 0;
	a /= g;
	b /= g;
	c /= g;
	assert(gcd(a,b) == 1);

	if (a == 0) return x2+1;
	if (b == 0) return y2+1;

	assert(a > 0 && b > 0);
	if (b > 1) {
		ll x0 = c % b * inv(a, b) % b;
		assert(0 <= x0 && x0 < b);
		x2 -= x0;
		c -= x0 * a;
		if (x2 < 0) return 0;
		if (c < 0) return 0;
		assert(c % b == 0);

		c /= b;
		x2 /= b;
		b = 1;
	}

	x2 = min(x2, c / a);
	x1 = 0;
	if (c - y2 > 0) {
		x1 = (c - y2 + a-1) / a;
	}
	return x2 - x1 + 1;
}

int main() {
	cout << go() << '\n';

	return 0;
}
