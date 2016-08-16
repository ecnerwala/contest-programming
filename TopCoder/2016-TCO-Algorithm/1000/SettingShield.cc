#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
#define A first
#define B second

class SettingShield {

public:
	static const int MAXN = 2e5;
	static const int MAXH = 2e5;
	static const int MAXT = 2e5;

	int N, H;
	ll T;
	ll P[MAXN];
	pii S[MAXH];
	int maxR[MAXN];

	static const int L = 1 << 17;
	ll seg[2 * L];

	// [a,b]
	void inc(int a, int b, ll v) {
		for(a += L, b += L; a <= b; a /= 2, b /= 2) {
			if(a & 1) {
				seg[a] += v;
				a++;
			}
			if(!(b & 1)) {
				seg[b] += v;
				b--;
			}
		}
	}

	ll query(int a) {
		ll res = 0;
		for(a += L; a; a /= 2) {
			res += seg[a];
		}
		return res;
	}

	ll solve(ll v) {
		// v uses of the shield
		ll res = v * T;
		memset(seg, 0, sizeof(seg));
		for(int i = 0; i < N; i++) {
			ll cur = P[i] - v - query(i);
			if(cur > 0) {
				assert(maxR[i] >= i);
				inc(i, maxR[i], cur);
				res += cur;
			}
		}
		return res;
	}

	ll getOptimalCost(int n, int h, int t, vector <int> val0, vector <int> a, vector <int> b, vector <int> m) {
		N = n, H = h, T = t;
		P[0] = val0[0];
		for(int i = 1; i < N; i++) {
			P[i] = (a[0] * P[i - 1] + b[0]) % m[0];
		}
		S[0] = pii(val0[1], val0[2]);
		for(int i = 1; i < H; i++) {
			ll l = min(ll(N - 1), (a[1] * S[i-1].A + b[1]) % m[1]);
			ll dist = S[i-1].B - S[i-1].A;
			ll r = min(ll(N - 1), l + (a[2] * dist + b[2]) % m[2]);
			S[i] = pii(l, r);
		}
		sort(S, S + H);
		maxR[0] = -1;
		for(int i = 0, j = 0; i < N; i++) {
			if(i) maxR[i] = maxR[i - 1];
			while(j < H && S[j].A == i) {
				maxR[i] = max(maxR[i], int(S[j].B));
				j++;
			}
		}
		ll mi = 0, ma = 1e7;
		for(int i = 0; i < N; i++) {
			if(maxR[i] < i) {
				mi = max(mi, P[i]);
			}
		}
		assert(mi <= ma);
		while(ma - mi > 1) {
			ll md = (mi + ma) / 2;
			assert(mi <= md && md + 1 <= ma);
			if(solve(md) < solve(md + 1)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		//cerr << mi << '\n';
		return min(solve(mi), solve(ma));
		/*
		ll res = solve(ma);
		ll prv0 = 0;
		ll prv1 = 0;
		for(ll v = mi; v <= ma; v++) {
			ll cur = solve(v);
			if(v >= mi + 2) {
				assert(prv0 + cur >= 2 * prv1);
			}
			prv0 = prv1, prv1 = cur;
			res = min(res, cur);
		}
		return res;
		*/
	}
};

// if we fix the special shield at k
// if you can get gains the second time, you can get them the first
// vim:ft=cpp
