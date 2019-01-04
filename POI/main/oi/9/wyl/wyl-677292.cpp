#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MAXN = 30;
int N;
int A[MAXN];
bool gone[MAXN];

ll gcd(ll a, ll b) { return a ? gcd(b % a, a) : b; }

ll lcm(ll a, ll b) {
	return a / gcd(a, b) * b;
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		int v;
		scanf("%d", &v);
		v--;
		A[v] = i;
	}
	ll val = 0;
	ll mod = 1;
	ll cur = N;
	gone[N] = true;
	for(int i = N; i > 0; i--) {
		int nxt = A[N - i];
		ll dist = 1;
		while(cur != nxt) {
			if(!gone[cur]) dist++;
			cur ++;
			cur %= N + 1;
		}
		assert(!gone[nxt]);
		gone[nxt] = true;
		assert(cur == nxt);
		dist %= i;
		// val % mod and dist % i
		for(int j = 0; val % i != dist && j < i; j++) {
			val += mod;
		}
		if(val % i != dist) {
			printf("NIE\n");
			return 0;
		}
		assert(val % i == dist);
		mod = lcm(mod, i);
	}
	if(!val) val += mod;
	printf("%lld\n", val);
}
