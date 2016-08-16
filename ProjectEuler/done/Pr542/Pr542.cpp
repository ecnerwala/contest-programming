#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll N = 1e17;

int main() {

	priority_queue<pair<ll, pair<ll, ll>>> pq;

	// optimization = b > n^1/3 is unnecessary, as b^2 + b(b-1) + (b-1)^2 is pretty measly
	for(ll b = 2; b * b * b <= N; b++) {
		for(ll v = b, u = b + (b - 1), e = 2; v <= N / b; e ++) {
			v *= b;
			u *= b-1, u += v;
			// u = b^e + (b-1)b^e-1 + ... + (b-1)^e
			assert(u / v < e + 1);
			pq.emplace(-v, make_pair(v, u));
			//cerr << v << ' ' << u << ' ' << ld(u) / v << '\n';
		}
	}

	ld cull_point = 0;

	ll prv = 0;
	ll prvi = 0;
	ll res = 0;
	while(!pq.empty()) {
		ll a = -pq.top().first;
		ll v = pq.top().second.first;
		ll u = pq.top().second.second;
		pq.pop();

		ld rat = ld(u) / v;
		if(rat < cull_point - 1e-9) continue;

		ll n = a / v;

		if(n * u > prv) {
			if((n * v - prvi) & 1) {
				res += ((prvi & 1) ? -prv : prv);
			}
			prv = n * u;
			prvi = n * v;

			cull_point = max(cull_point, rat * n / (n+1));
		}

		if((n + 1) * v <= N) pq.emplace(-(n + 1) * v, make_pair(v, u));
	}

	if(((N + 1) - prvi) & 1) {
		res += ((prvi & 1) ? -prv : prv);
	}

	cout << res << '\n';

	return 0;
}
