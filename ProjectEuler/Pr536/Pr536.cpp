#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXV = 2e6;
const ll V = 1e6 + 10;
bool isprime[MAXV];

vector<ll> primes;

const ll N = 1e12;

ll gcd(ll a, ll b) {
	return a ? gcd(b % a, a) : b;
}

ll lcm(ll a, ll b) {
	return a / gcd(a, b) * b;
}

ll inv(ll a, ll m) {
	return (a == 1) ? 1 : (m - inv(m % a, a) * m / a);
}

int main() {
	for(ll i = 2; i <= V; i++) isprime[i] = true;
	for(ll i = 2; i * i <= V; i++) {
		if(isprime[i]) {
			for(ll j = i * i; j <= V; j += i) isprime[j] = false;
		}
	}
	for(ll i = 2; i <= V; i++) {
		if(isprime[i]) {
			primes.push_back(i);
		}
	}

	return 0;
}

// min solution for prime p: 0 mod p, -3 mod p-1 is p * (p - 4)
