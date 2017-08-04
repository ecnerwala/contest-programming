#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = int(1e5) + 200;
int N;
ll K;

pair<ll, ll> val[MAXN];

int main() {
	scanf("%d %lld", &N, &K);
	for (int i = 0; i < N; i++) {
		int M; scanf("%d %lld", &M, &val[i].second);
		val[i].first = 0;
		for (ll j = 0; j < M; j++) {
			ll x, y; scanf("%lld %lld", &x, &y);
			val[i].first = max(val[i].first, x * x + y * y);
		}
		val[i].first = ll(ceil(sqrt((long double) val[i].first)));
	}
	sort(val, val + N);
	ll res = 0;
	ll revenue = 0;
	for (ll i = 0; i < N; i++) {
		revenue += val[i].second;
		ll r = (val[i].first + K - 1) / K;
		res = max(res, revenue - r * (r - 1) / 2);
	}
	printf("%lld\n", res);

	return 0;
}
