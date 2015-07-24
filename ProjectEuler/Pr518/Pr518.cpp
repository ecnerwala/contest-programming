#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e8;

bool isprime[MAXN];

ll gcd(ll a, ll b) {
	return a ? gcd(b % a, a) : b;
}

ll S(ll N) {
	for(ll i = 2; i <= N; i++) {
		isprime[i] = true;
	}
	for(ll i = 2; i * i <= N; i++) {
		if(!isprime[i]) continue;
		for(ll j = i * i; j <= N; j += i) {
			isprime[j] = false;
		}
	}

	ll res = 0;
	for(ll a = 1; a * a <= N; a++) {
		for(ll b = a + 1; b * b <= N; b++) {
			if(gcd(a, b) != 1) continue;
			for(ll c = 1; c <= N / (b * b); c++) {
				if(isprime[a * a * c - 1] && isprime[a * b * c - 1] && isprime[b * b * c - 1]) {
					res += (a * a + a * b + b * b) * c - 3;
				}
			}
		}
	}
	return res;
}

int main() {
	assert(S(100) == 1035);
	cout << S(100000000) << '\n';
	return 0;
}
