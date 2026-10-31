#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 19;
pair<ll, ll> seg[2*S]; // cnt, sum of positions

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	ll N, Q; cin >> N >> Q;
	ll offset = 0;
	for (ll i = 0; i < N; i++) {
		ll c; cin >> c;
		if (i == 0) {
			offset = c - (S/2);
		}
		c -= offset;
		assert(0 <= c && c < S);
		if (i == 0) assert(c == S/2);
		for (ll a = c+S; a; a /= 2) {
			seg[a].first += 1;
			seg[a].second += c;
		}
	}

	for (ll q = 0; q < Q; q++) {
		ll t; cin >> t;
		if (t == 1) {
			ll c; cin >> c;
			c -= offset;
			assert(0 <= c && c < S);
			for (ll a = c+S; a; a /= 2) {
				seg[a].first += 1;
				seg[a].second += c;
			}
		} else if (t == 2) {
			ll a; cin >> a;
			offset += a;
		} else if (t == 3) {
			ll k; cin >> k;
			ll ans = 0;
			auto queryRange = [&](ll l, ll r) -> pair<ll, ll> {
				l = max(l, ll(0));
				r = min(r, ll(S));
				pair<ll, ll> res(0,0);
				if (l >= r) return res;
				assert(0 <= l && l <= r && r <= S);
				for (ll a = l+S, b = r+S; a < b; a /= 2, b /= 2) {
					if (a & 1) {
						res.first += seg[a].first;
						res.second += seg[a].second;
						a++;
					}
					if (b & 1) {
						--b;
						res.first += seg[b].first;
						res.second += seg[b].second;
					}
				}
				return res;
			};
			auto check = [&](ll fac) {
				ll lo = fac * k - offset;
				ll md = fac * k + (k / 2) + 1 - offset;
				ll hi = fac * k + k - offset;
				auto it = queryRange(lo, md);
				ans += it.second - it.first * lo;
				auto jt = queryRange(md, hi);
				ans += jt.first * hi - jt.second;
			};

			for (ll v = offset / k; v * k - offset <= S; ) {
				check(v++);
			}
			for (ll v = offset / k; v * k - offset >= 0; ) {
				check(--v);
			}
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
