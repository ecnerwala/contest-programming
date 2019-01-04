#include<iostream>
#include<vector>

using namespace std;

typedef long long ll;

#define ITER(i, v) for(__typeof((v).begin()) i = v.begin(); i != v.end(); i++) 

const ll MAXN = 1e5;
const ll MAXM = 2e6;
ll N, M;
vector<ll> adj[MAXN];

ll type[MAXN];

inline ll c2(ll a) {
	return a * (a - 1) / 2;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(ll i = 0; i < M; i++) {
		ll a, b; cin >> a >> b; a--, b--;

		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	ITER(it, adj[0]) {
		ll v = *it;
		type[v] = 1;
		ITER(jt, adj[v]) {
			ll w = *jt;
			if(type[w] == 0) type[w] = 2;
		}
	}
	ITER(it, adj[1]) {
		ll v = *it;
		type[v] = -1;
		ITER(jt, adj[v]) {
			ll w = *jt;
			if(type[w] == 0) type[w] = -2;
		}
	}

	ll a1 = 0, b1 = 0, a2 = 0, b2 = 0, n = 0;
	for(ll i = 2; i < N; i++) {
		switch(type[i]) {
			case  0: n++;
					 break;
			case +1: a1++;
					break;
			case +2: b1++;
					break;
			case -1: a2++;
					 break;
			case -2: b2++;
					 break;
		}
	}

	if(a1 > a2) {
		b1 += n;
	} else {
		b2 += n;
	}

	//cerr << a1 << ' ' << b1 << ' ' << a2 << ' ' << b2 << '\n';

	ll num = a1 + a2 + b1 * b2 + b1 * a1 + b2 * a2 + c2(a1) + c2(a2) + c2(b1) + c2(b2);
	num -= M;
	cout << num << '\n';
	return 0;
}
