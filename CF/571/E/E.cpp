#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

const int MAXN = 200;
int N;
ll VA[MAXN], VB[MAXN];

const int MAXP = 1000;
int P;
vector<ll> primes;

ll powmod(ll a, ll b) {
	a %= MOD;
	b %= MOD - 1;
	ll res = 1;
	while(b) {
		if(b & 1) res *= a, res %= MOD;
		a *= a, a %= MOD;
		b /= 2;
	}
	return res;
}

ll A[MAXN][MAXP];
ll B[MAXN][MAXP];

void factor(ll a) {
	for(ll i = 2; i * i <= a; i++) {
		if(a % i == 0) {
			primes.push_back(i);
			while(a % i == 0) a /= i;
		}
	}
	if(a > 1) primes.push_back(a);
}

ll E[MAXP];
ll C[MAXN];

ll go() {
	memset(E, -1, sizeof(E));
	memset(C, -1, sizeof(C));
	for(int j = 0; j < P; j++) {
		ll val = -1;
		for(int i = 0; i < N; i++) {
			if(B[i][j] == 0) {
				if(val == -1) {
					val = A[i][j];
				} else if(val != A[i][j]) {
					return -1;
				}
			}
		}
		if(val == -1) continue;
		E[j] = val;
		for(int i = 0; i < N; i++) {
			if(B[i][j] == 0) {
				assert(val == A[i][j]);
			} else {
				if(val < A[i][j]) return -1;
				if((val - A[i][j]) % B[i][j] != 0) return -1;
				C[i] = (val - A[i][j]) / B[i][j];
				ll res = 1;
				for(int k = 0; k < P; k++) {
					E[k] = A[i][k] + B[i][k] * C[i];
					res *= powmod(primes[k], E[k]);
					res %= MOD;
				}
				for(int l = 0; l < N; l++) {
					C[l] = -1;
					for(int k = 0; k < P; k++) {
						if(E[k] < A[l][k]) return -1;
						if(B[l][k] == 0) {
							if(E[k] != A[l][k]) return -1;
							continue;
						}
						if((E[k] - A[l][k]) % B[l][k] != 0) return -1;
						ll cnd = (E[k] - A[l][k]) / B[l][k];
						if(C[l] == -1) C[l] = cnd;
						else if(C[l] != cnd) return -1;
					}
				}
				return res;
			}
		}
	}

	cerr << "hi\n";
	for(int j = 0; j < P; j++) {
		for(int i = 0; i < N; i++) {
			assert((E[j] == -1) ^ (B[i][j] == 0));
		}
	}

	ll res = 1;
	int s = 0;
	for(int j = 0; j < P; j++) {
		if(E[j] == -1) {
			assert(s <= j);
			if(s < j) {
				E[s] = E[j];
				primes[s] = primes[j];
				for(int i = 0; i < N; i++) {
					A[i][s] = A[i][j];
					B[i][s] = B[i][j];
				}
			}
			s++;
		} else {
			res *= powmod(primes[j], E[j]);
			res %= MOD;
		}
	}
	P = s;

	assert(N >= 2);

	for(int j = 0; j < P; j++) {
		assert(E[j] != -1);
		res *= powmod(primes[j], E[j]);
		res %= MOD;
	}
	return res;
}

int main() {
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> VA[i] >> VB[i];
		factor(VA[i]);
		factor(VB[i]);
	}
	if(N == 1) {
		cout << VA[i] << '\n';
		return 0;
	}
	sort(primes.begin(), primes.end());
	primes.resize(unique(primes.begin(), primes.end()) - primes.begin());
	P = int(primes.size());
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < P; j++) {
			ll p = primes[j];
			while(VA[i] % p == 0) {
				VA[i] /= p;
				A[i][j] ++;
			}
			while(VB[i] % p == 0) {
				VB[i] /= p;
				B[i][j] ++;
			}
		}
	}

	cout << go() << '\n';

	return 0;
}
