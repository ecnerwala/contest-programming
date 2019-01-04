#include<bits/stdc++.h>

using namespace std;

int N, M, B;

int R;

const int MAXR = 5e4;
bool isprime[MAXR];

const int S = 7.7e6;
int val[S + 200];

int res;

// (l, r]
void solve(int l, int r) {
	int d = r - l;
	for(int i = d; i; i--) {
		val[i] = 1;
	}
	for(int p = 2; p <= R; p++) { if(!isprime[p]) continue;
		for(int q = p, t = r / p, s = l / p; t ^ s; q *= p, t /= p, s /= p) {
			int *v = val + (t * q - l);
			for(int cnt = t - s; cnt; cnt--) {
				*v *= p;
				v -= q;
			}
		}
	}

	for(int i = d; i; i--) {
		if((l + i) / val[i] <= B) res++;
	}
}

int main() {
	scanf("%d %d %d", &N, &M, &B);

	M += N;
	N--;

	while(R < B && R * R < M) R ++;

	for(int i = 2; i <= R; i++) {
		isprime[i] = true;
	}
	for(int i = 2; i * i <= R; i++) {
		if(isprime[i]) {
			for(int j = i * i; j <= R; j += i) {
				isprime[j] = false;
			}
		}
	}

	// (N, M]
	while(N ^ M) {
		int r = min(M, N + S);
		solve(N, r);
		N = r;
	}

	printf("%d\n", res);
	return 0;
}
