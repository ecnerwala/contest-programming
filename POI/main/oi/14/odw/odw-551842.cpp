#include<iostream>
#include<algorithm>

using namespace std;

const int INF = 2e9;

const int MAXN = 2e5;
const int MAXM = 2e5;
const int MAXK = 50;

int N, M, K;
int C[MAXN], V[MAXM], W[MAXK];

void precomp() {
	sort(C, C + N);
	sort(V, V + M);
	K = 1;
	W[0] = V[0];
	for(int i = 1; i < M; i++) {
		if(V[i] != W[K - 1]) {
			W[K++] = V[i];
		}
	}
	V[M] = INF;
	W[K] = INF;
}

int main() {
	cin >> N >> M;
	for(int i = 0; i < N; i++) cin >> C[i];
	for(int i = 0; i < M; i++) cin >> V[i];

	precomp();

	int res = 0;
	for(int i = 1; i <= K; i++) {
		for(int j = 0; j < N; j++) {
			while(V[res] < C[j] % W[i]) {
				C[j] -= V[res];
				res++;
			}
		}
	}
	cout << res << '\n';
	return 0;
}
