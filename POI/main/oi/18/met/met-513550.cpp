#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>
#include<cassert>

using namespace std;

#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;

const int MAXN = 600000;
const int MAXM = 600000;
const int MAXK = 600000;

int N, M, K;

int O[MAXM];
pii own[MAXM];
int S[MAXM];
int B[MAXN];
int E[MAXN];
ll P[MAXN];

int L[MAXK], R[MAXK];
ll V[MAXK];
bool revMet[MAXK];

int T;
int W[MAXN];

ll D[MAXM];

void go() {

	for(int i = 0; i < M; i++) {
		own[i] = pii(O[i], i);
	}

	sort(own, own + M);

	for(int i = 0; i < M; i++) {
		S[i] = own[i].B;
		E[own[i].A] = i + 1;
	}

	for(int i = M - 1; i >= 0; i--) {
		B[own[i].A] = i;
	}

	//for(int i = 0; i < M; i++) { cerr << S[i] << ' '; } cerr << '\n';
	//for(int i = 0; i < N; i++) { cerr << B[i] << '-' << E[i] << ' ';} cerr << '\n';

	int numLeft = N;

	T = 2000; // emperical

	memset(W, -1, sizeof(W));
	memset(D, 0, sizeof(D));
	
	for(int a = 0; a * T < K; a++) {
		//cerr << a << '\n';

		ll num = 0;
		for(int b = 0; b < min(T, K - a * T); b++) {
			//cerr << b << '\n';
			int ind = a * T + b;
			int l = L[ind];
			int r = R[ind];
			ll v = V[ind];

			D[l] += v;
			D[r] -= v;

			if(revMet[ind] = (l >= r)) {
				num += v;
				swap(L[ind], R[ind]);
			}
			
		}

		for(int i = 0; i < M; i++) {
			num += D[i];
			D[i] = 0;// gotta reset it now

			assert(num >= 0);

			int n = O[i];
			if(W[n] == -1) {
				//if(P[n] > 0 && P[n] <= num) proc.push_back(n);
				P[n] -= num;
			}
		}

		for(int n = 0; n < N; n++) {
			if(W[n] == -1 && P[n] <= 0) {

				//cerr << "processing " << n << " with a = " << a << " and P = " << P[n] << '\n';

				for(int b = min(T, K - a * T) - 1; b >= 0; b--) {
					int ind = a * T + b;
					int l = L[ind];
					int r = R[ind];
					ll v = V[ind];
					bool rev = revMet[ind];

					for(int i = B[n]; i < E[n]; i++) {
						int s = S[i];// sector

						if(rev != (l <= s && s < r)) {
							P[n] += v;
						}

					}

					//cerr << "processing " << n << " with a = " << a << " b = " << b << " and P = " << P[n] << '\n';

					if(P[n] > 0) {
						W[n] = ind; 
						break;
					}
				}

				assert(P[n] > 0);

				P[n] = 0;
				numLeft --;
				if(numLeft == 0) return;
			}
		}
	}
}

int main() {
	scanf("%d%d", &N, &M);

	for(int i = 0; i < M; i++) {
		scanf("%d", &O[i]);
		O[i] --;
	}

	for(int i = 0; i < N; i++) {
		scanf("%lld", &P[i]);
	}

	scanf("%d", &K);

	for(int i = 0; i < K; i++) {
		scanf("%d%d%lld", &L[i], &R[i], &V[i]);
		L[i]--; // R[i] --, R[i] ++;
	}

	go();

	for(int i = 0; i < N; i++) {
		if(W[i] == -1) printf("NIE\n");
		else printf("%d\n",  (W[i] + 1));
	}
}
