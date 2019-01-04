#include<bits/stdc++.h>

using namespace std;

int N, M, B;

int R;

const int MAXR = 2e6;
bool isprime[MAXR];

const int S = 7e6;
int val[S + 200];

int res;

// [l, r)
void solve(int l, int r) {
	int d = r - l;
	for(int i = 0; i < d; i++) {
		val[i] = 1;
	}
	for(int p = 2; p <= R; p++) { if(!isprime[p]) continue;
		for(int q = p, t = (r - 1) / p, s = (l - 1) / p; t > s; q *= p, t /= p, s /= p) {
			int *v = val + (t * q - l);
			for(int cnt = t - s; cnt > 0; cnt--) {
				*v *= p;
				v -= q;
			}
		}
	}

	for(int i = 0; i < d; i++) {
		if((l + i) / val[i] <= B) res++;
	}
}

int main() {
	scanf("%d %d %d", &N, &M, &B);

	M += N;
	M++;

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

	// [N, M)
	while(N < M) {
		int r = min(M, N + S);
		solve(N, r);
		N = r;
	}

	printf("%d\n", res);
	return 0;
}
