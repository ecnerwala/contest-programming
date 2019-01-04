#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;
const ll MAXN = 2e5;
ll N;
pii V[2][MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	ll S = 0;
	for(ll i = 0; i < N; i++) {
		ll x, y, t;
		cin >> x >> y >> t;
		V[0][i] = pii(x + y, t);
		V[1][i] = pii(x - y, t);
		S += t;
	}
	ll cost[2][2];
	ll best[2][2];
	// coords are v 
	for(ll t = 0; t < 2; t++) {
		sort(V[t], V[t] + N);
		ll L = 0;
		for(ll i = 0; i < N; i++) {
			assert(i >= L);
			if(i == L) continue;
			assert(i > L);
			if(V[t][i].first == V[t][L].first) {
				V[t][L].second += V[t][i].second;
			} else {
				V[t][++L] = V[t][i];
			}
		}
		L++;
		ll s = V[t][0].second;
		ll i = 0;
		while(s * 2 < S) {
			s += V[t][++i].second;
		}
		assert(i < L);
		ll x = V[t][i].first;

		cost[t][x % 2] = 0;
		best[t][x % 2] = x;

		assert(s * 2 >= S);
		cost[t][!(x % 2)] = s - (S - s);
		best[t][!(x % 2)] = x + 1;

		s -= V[t][i].second;
		assert(s * 2 < S);
		if((S - s) - s < cost[t][!(x % 2)]) {
			cost[t][!(x % 2)] = (S - s) - s;
			best[t][!(x % 2)] = x - 1;
		}

	}

	ll a, b;
	if(cost[0][0] + cost[1][0] < cost[0][1] + cost[1][1]) {
		a = best[0][0];
		b = best[1][0];
	} else {
		a = best[0][1];
		b = best[1][1];
	}
	cout << (a + b) / 2 << ' ' << (a - b) / 2 << '\n';
}
