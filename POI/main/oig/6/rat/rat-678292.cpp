#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
inline ll sq(ll a) { return a * a; }
int main() {
	ll N, K, X, Y;
	scanf("%lld %lld %lld %lld", &N, &K, &X, &Y);
	int res = 0;
	for(int i = 0; i < N; i++) {
		ll a, b; scanf("%lld %lld", &a, &b);
		if(sq(X - a) + sq(Y - b) > sq(K)) res++;
	}
	printf("%d\n", res);
	return 0;
}
