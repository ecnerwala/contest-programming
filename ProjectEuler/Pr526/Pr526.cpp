#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;
#define A first
#define B second

const int V = 1e8 + 200;
const int MAXV = V + 200;
bool prime[MAXV];

// Best 47500041620774139
// Searched up to 1e16 - 5e8
const ll N = 1e16 - 5e8 + 8;

ll fs[9];

int main() {
	priority_queue<pii> pq;

	for(ll i = 2; i <= V; i++) prime[i] = true;
	for(ll i = 2; i <= V; i++) {
		if(!prime[i]) continue;
		pq.push(pii(N / i * i, -i));
		for(ll j = i * i; j <= V; j += i) {
			prime[j] = false;
		}
	}

	ll g = 0;
	ll h = 0;

	for(ll k = N; k; k--) {

		ll v = k;

		ll f = 0;

		while(pq.top().A == k) {
			f = -pq.top().B;
			pq.pop(), pq.push(pii(k - f, -f));
			while(v % f == 0) v /= f;
		}

		if(v > 1) assert(v > f), f = v;

		g -= fs[k % 9];
		g += (fs[k % 9] = f);
		if(k > N - 8) continue;

		if(g > h) {
			h = g;
			cerr << "New best: " << h << '\n';
		}
		if(k % 100000000 == 0) cerr << "Finished " << k << '\n';
	}

	return 0;
}
