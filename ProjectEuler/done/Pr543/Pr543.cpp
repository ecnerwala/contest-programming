#include<bits/stdc++.h>
using namespace std;

// We have the necessary condition that n >= 2k
// k = 3 and n >= 2k - all by goldbach(if n odd, n >= 7, then goldbach's weak conjecture, if n even, then - 2 and goldback's )
// k = 2 and n even - all by goldbach
// k = 2 and n odd - primes + 2
// k = 1 - primes

typedef long long ll;
const int MAXN = 8e8;
bool is_prime[MAXN];

void precomp() {
	memset(is_prime, 1, sizeof(is_prime));
	is_prime[0] = is_prime[1] = false;
	for(int i = 2; i * i < MAXN; i++) {
		if(!is_prime[i]) continue;
		for(int j = i * i; j < MAXN; j += i) {
			is_prime[j] = false;
		}
	}
}

ll S(int b) {
	ll res = 0;
	for(int k = b / 2; k > 2; k--) {
		// we need all numbers 2k <= n <= b
		res += (b - 2 * k) + 1;
	}
	// k = 2
	// evens b >= 2a >= 2k <=> b/2 >= a >= 2
	res += (b / 2) - 1;
	// k = 2
	// odds = p + 2, b - 2 >= p >= 3
	for(int p = b - 2; p > 2; p--) {
		res += is_prime[p];
	}
	// k = 1
	// primes b >= p >= 1
	for(int p = b; p > 0; p--) {
		res += is_prime[p];
	}
	return res;

}

int main() {
	precomp();

	assert(S(10) == 20);
	assert(S(100) == 2402);
	assert(S(1000) == 248838);

	ll res = 0;
	for(int a = 0, b = 1, c = 1; c <= 44; swap(a, b), b += a, c++) {
		if(c < 3) continue;
		res += S(b);
	}

	cout << res << '\n';
	return 0;
}
