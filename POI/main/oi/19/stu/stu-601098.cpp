#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e6;

const ll MAXH = 1e9 + 20;

const ll INF = 1.5e18;

inline ll c2(ll a) { return a * (a - 1) / 2; }

int N;
int X[MAXN];
ll M;

ll H[MAXN];
ll pref[MAXN];
// s = slope
// returns pair of work, location
pair<ll, int> getwork(ll s) {
	for(int i = 0; i < N; i++) H[i] = X[i];

	ll res = 0;
	for(int z = 0; z < 2; z++) {
		ll m = H[0];
		for(ll i = 1; i < N; i++) {
			if(H[i] > m + i * s) {
				res += H[i] - (m + i * s);
				H[i] = m + i * s;
			} else {
				m = min(m, H[i] - i * s);
			}
		}
		reverse(H, H + N);
	}
	//for(int i = 0; i < N; i++) cerr << H[i] << '\n';

	pref[0] = 0;
	for(int i = 0; i < N; i++) pref[i + 1] = pref[i] + H[i];

	ll work = INF;
	ll pos = -1;

	for(int i = 0, l = 0, r = 0; i < N; i++) {
		while(H[l] < s * (i - l)) l++;
		assert(l <= i);
		while(r < N && H[r] > s * (r - i)) r++;
		ll v = pref[r] - pref[l] - s * (c2(r - i) + c2(i - l + 1));
		if(v < work) {
			work = v;
			pos = i;
		}
	}

	return make_pair(res + work, pos);
}

int main() {
	scanf("%d%lld", &N, &M);
	for(int i = 0; i < N; i++) scanf("%d", &X[i]);

	ll mi = -1;
	ll ma = MAXH;
	while(ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if(getwork(md).first <= M) ma = md;
		else mi = md;
	}

	//cerr << getwork(ma).first << '\n';

	printf("%d %lld\n", getwork(ma).second + 1, ma);

	return 0;
}
