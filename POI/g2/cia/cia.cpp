#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll A[int(3e6)];

ll go(ll N) {
	assert(N >= 1);
	A[0] = 0;
	A[1] = 1;
	A[2] = 2;
	ll a = 1;
	ll k = 0;
	ll l = 0;
	ll m = 0;
	ll n = 0;
	for (ll a = 1; true; a++) {
		for (ll z = 0; z < A[a]; z++) {
			assert(n < N);
			k++;
			A[k] = a;
			// A[k] = a
			// A[l+1..l+a] = [k, ... k] (a of them)
			// A[m+1..m+ak] = [l+1, ... l+a] (k each)
			// A[n+1..n+(2l + a + 1)a/2*k] = [m+1, ..., m + ak] ((l+1) * k, (l+2) * k, ....)
			ll nn = n + ((l+1) + (l+a)) * a / 2 * k;
			if (nn >= N) {
				ll mi = m;
				ll ma = m + a * k;
				while (ma - mi > 1) {
					ll md = (mi + ma) / 2;
					ll v = (md - m);
					ll na = (v / k);
					ll nnn = n + ((l + 1) + (l + na)) * na / 2 * k;
					nnn += (l+na+1) * (v % k);
					if (nnn >= N) {
						ma = md;
					} else {
						mi = md;
					}
				}
				return ma;
			}
			n = nn;
			m += a * k;
			l += a;
		}
	}

}

int main() {
	ll N;
	scanf("%lld", &N);
	ll res = go(N);
	printf("%lld\n", res);
	return 0;
}
