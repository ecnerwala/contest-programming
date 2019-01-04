#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

typedef long long ll;

template <class T> T gcd(T a, T b) { return a ? gcd(b % a, a) : b; }

const int MAXK = 500000;

ll N;
int K;
ll M[MAXK];

set<ll> factors;
vector<ll> primes;

vector<ll> getPrimes(ll v) {
	vector<ll> ans;
	for(ll i = 2; i * i <= v; i++) {
		if(v % i == 0) {
			ans.push_back(i);
			while(v % i == 0) v /= i;
		}
	}
	if(v > 1) ans.push_back(v);
	return ans;
}

set<ll> getFactors(ll v) {
	set<ll> ans;
	for(ll i = 1; i * i <= v; i++) {
		if(v % i == 0) {
			ans.insert(i);
			ans.insert(v / i);
		}
	}
	return ans;
}

void clear(ll i) {
	if(!factors.count(i)) return;

	factors.erase(i);
	
	for(int j = 0; j < primes.size(); j++) {
		ll p = primes[j];
		if(i % p == 0) clear(i / p);
	}
}

ll go() {
	K--;

	ll V = M[K] = gcd(M[K], N);

	for(int i = 0; i < K; i++) {
		M[i] = gcd(M[i], V);
	}

	primes = getPrimes(V);
	factors = getFactors(V);

	for(int i = 0; i < K; i++) {
		clear(M[i]);
	}

	return N / (*factors.begin());
}

int main() {
	ios_base::sync_with_stdio(0);
	
	cin >> N >> K;
	for(int i = 0; i < K; i++) {
		cin >> M[i];
	}

	cout << go() << '\n';
}
