#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll sum(ll a) {
	ll res = 0;
	for(a /= 2; a; a /= 2) {
		res += a;
	}
	return res;
}

ll Q(ll m , ll n) {
	return m * sum((n+1) / 2) + sum(n / 2);
}

ll Q(ll n) {
	return Q(904961, n);
}

int main() {
	assert(Q(8) == 2714886);
	cout << Q(ll(1e12)) << '\n';
	return 0;
}

/*
((n+2)*(n+1)/2)^m - (n+1)^m

taking the exponent does almost nothing?

if n+1 is even, E(m, n) = m*(v2(n+1) - 1) = m v2((n+1)/2)

if n+1 is odd, E(m, n) = v2((n+2)/2 - 1) = v2(n/2)

Overall, we have
Q(m,n) = sum_i=1^n E(m,n)
= m * sum_i=1^(n+1)/2 v_2(i) + sum_i=1^(n/2) v_2(i)
*/
