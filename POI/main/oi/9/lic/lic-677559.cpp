#include<bits/stdc++.h>

using namespace std;

int N, M, B;

const int MAXB = 2e6;
bool isprime[MAXB];

const int S = 7e6;
int val[S + 200];

// [l, r)
int solve(int l, int r) {
	int d = r - l;
	for(int i = 0; i < d; i++) {
		val[i] = l + i;
	}
	//for(int z = 0; z < P; z++) { int p = primes[z];
	for(int p = 2; p <= B; p++) { if(!isprime[p]) continue;
		for(long long q = p, t = (r - 1) / p; t * q >= l; q *= p, t /= p) {
			for(int i = t * q - l; i >= 0; i -= q) {
				val[i] /= p;
			}
		}
	}

	int res = 0;
	for(int i = 0; i < d; i++) {
		if(val[i] == 1) res++;
	}
	return res;
}

int main() {
	scanf("%d %d %d", &N, &M, &B);

	for(int i = 2; i <= B; i++) {
		isprime[i] = true;
	}
	for(int i = 2; i * i <= B; i++) {
		if(isprime[i]) {
			for(int j = i * i; j <= B; j += i) {
				isprime[j] = false;
			}
		}
	}

	/*
	for(int i = 2; i <= B; i++) {
		if(isprime[i]) primes[P++] = i;
	}
	*/

	M += N;
	M++;
	// [N, M)
	int res = 0;
	while(N < M) {
		int r = min(M, N + S);
		res += solve(N, r);
		N = r;
	}

	printf("%d\n", res);
	return 0;
}
