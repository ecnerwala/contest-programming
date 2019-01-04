#include<bits/stdc++.h>
using namespace std;

const int MAXN = 310;
const int MAXM = MAXN * MAXN / 2;
const int MAXK = MAXN;

int N;
int M;
int S[MAXM];

int K;
int res[MAXK][MAXM];

int main() {
	scanf("%d", &N);
	M = N * (N - 1) / 2;
	for(int i = 0; i < M; i++) {
		scanf("%d", &S[i]);
	}
	sort(S, S + M);
	for(int i = 2; i < N; i++) {
		// position of x[1] + x[2]
		if(i > 2 && S[i] == S[i - 1]) {
			continue;
		}
		int s3 = S[0] + S[1] + S[i];
		if(s3 % 2 != 0) continue;
		s3 /= 2;
		res[K][0] = s3 - S[i];
		if(res[K][0] <= 0) continue;
		priority_queue<int, vector<int>, greater<int> > pq;
		bool good = true;
		for(int j = 0, q = 1; j < M; j++) {
			if(!pq.empty() && pq.top() == S[j]) {
				pq.pop();
				continue;
			}
			if(q == N) {
				good = false;
				break;
			}
			res[K][q] = S[j] - res[K][0];
			if(res[K][q] == res[K][q - 1]) {
				good = false;
				break;
			}
			for(int k = 1; k < q; k++) {
				pq.push(res[K][k] + res[K][q]);
			}
			q++;
		}
		if(!good) continue;
		K++;
	}
	printf("%d\n", K);
	for(int k = 0; k < K; k++) {
		printf("%d", res[k][0]);
		for(int i = 1; i < N; i++) {
			printf(" %d", res[k][i]);
		}
		printf("\n");
	}
	return 0;
}
