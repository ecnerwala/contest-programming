#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	ll N, K, I, J; scanf("%lld %lld %lld %lld", &N, &K, &I, &J);
	I += (N - K);
	J += (N - K);
	ll res = (I + J) * (J - I + 1) / 2 - N * (max(N, J) - max(I-1, N));
	printf("%lld\n", res);
	return 0;
}
