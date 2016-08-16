#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll go(ll s, ll x) {
	// s = a + b = (a ^ b) + ((a & b) << 1)
	if(s < x) return 0;
	s -= x;
	if(s & 1) return 0;
	s >>= 1;
	// s = a & b, x = a ^ b
	if(s & x) return 0; // if there's overlap, then we're screwed

	ll res = 1;
	for(; x; x &= x - 1) res <<= 1;

	if(s == 0) res -= 2;
	if(res < 0) res = 0;
	return res;
}

int main() {
	ll s, x; cin >> s >> x;
	cout << go(s, x) << '\n';
}
