#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second

ll N, M, B;

const int MAXB = 2e6;
bool isprime[MAXB];

int main() {
	scanf("%lld %lld %lld", &N, &M, &B);
	// pll's store value, base
	priority_queue<pll, vector<pll>, greater<pll> > pq;

	for(ll i = 2; i <= B; i++) {
		isprime[i] = true;
	}
	for(ll i = 2; i <= B; i++) {
		if(isprime[i]) {
			for(ll j = i * i; j <= B; j += i) {
				isprime[j] = false;
			}
			// ceiling N / i;
			ll v = (N + i - 1) / i;
			v *= i;
			if(v <= N + M) pq.push(pll(v, i));
		}
	}

	ll res = 0;
	for(ll i = N; i <= N + M; i++) {
		ll val = i;
		while(!pq.empty() && pq.top().A <= i) {
			assert(pq.top().A == i);
			ll p = pq.top().B;
			pq.pop();

			assert(isprime[p]);
			ll nxt = i + p;
			assert(nxt % p == 0);
			if(nxt <= N + M) pq.push(pll(nxt, p));

			assert(val % p == 0);
			while(val % p == 0) {
				val /= p;
			}
		}
		if(val == 1) res++;
	}
	assert(pq.empty());
	printf("%lld\n", res);
	return 0;
}
