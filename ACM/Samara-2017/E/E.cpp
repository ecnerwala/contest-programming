#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 1e18;

size_t NT, NB;
vector<ll> T;
vector<ll> B;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> NT >> NB;
	T.reserve(NT + 2);
	T.push_back(-INF);
	for (size_t i = 0; i < NT; i++) {
		int t; cin >> t;
		T.push_back(t);
	}
	T.push_back(INF);
	NT += 2;

	B.reserve(NB);
	for (size_t i = 0; i < NB; i++) {
		int b; cin >> b;
		B.push_back(b);
	}

	ll res = 0;
	for (size_t t = 0, b = 0; t + 1 < NT && b < NB; t++) {
		ll l = T[t], r = T[t+1];
		if (B[b] >= r) {
			continue;
		}
		assert(B[b] >= l);

		ll v = r - l;

		ll x = B[b];
		assert(l <= x && x <= r);
		v = min(v, 2 * (r - x));

		while (b < NB && B[b] <= r) {
			v = min(v, 2 * (b + 1 < NB ? max(r - B[b+1], 0ll) : 0ll) + 2 * (B[b] - l));
			b++;
		}
		res += v;
	}
	cout << res << '\n';

	return 0;
}
