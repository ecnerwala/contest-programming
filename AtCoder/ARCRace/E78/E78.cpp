#include<bits/stdc++.h>
using namespace std;

#define TEST 0

using ll = long long;

#if TEST
static const ll N = ll(1e9) - 1;
#endif

bool cmp(ll a, ll b) {
	return (a <= b) == (to_string(a) <= to_string(b));
}

bool query(ll v) {
	assert(1 <= v && v <= ll(1e18));
	cout << "?" << ' ' << v << '\n' << flush;
	char c;
#if TEST
	c = "NY"[cmp(v, N)];
	cerr << c << '\n' << flush;
#else
	cin >> c;
#endif
	assert(c == 'Y' || c == 'N');
	return c == 'Y';
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	ll mi = ll(1e8) - 1;
	ll ma = ll(1e9) - 1;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (query(md * 10)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	// it's a prefix of ma
	if (ma == ll(1e8)) {
		ma *= 10;
	}
	string s = to_string(ma);
	if (s[0] == '1') {
		s[0]++;
		mi = 0;
		ma = int(s.size());
		while (ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if (query(stoll(s.substr(0, md)))) {
				ma = md;
			} else {
				mi = md;
			}
		}
		s[0]--;
		s = s.substr(0, ma);
	} else {
		s[0]--;
		mi = 1;
		ma = int(s.size())+1;
		while (ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if (query(stoll(s.substr(0, md)))) {
				mi = md;
			} else {
				ma = md;
			}
		}
		s[0]++;
		s = s.substr(0, mi);
	}
	ll ans = stoll(s);
	cout << "!" << ' ' << ans << '\n';
#if TEST
	assert(ans == N);
#endif

	return 0;
}
