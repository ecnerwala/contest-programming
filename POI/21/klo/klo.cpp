#include<bits/stdc++.h>
using namespace std;

const int MAXK = 2e6;
const int MAXN = 2e6;

int K;
int P, Q;
int C[MAXK];

int N;
int V[MAXN];

bool verify() {

	for(int i = 0; i + 1 < N; i++) {
		if(V[i] == V[i + 1]) return false;
	}

	for(int i = 0; i < N; i++) {
		if(V[i] >= K) return false;
		if(V[i] < 0) return false;
		C[V[i]] --;
	}
	for(int i = 0; i < K; i++) {
		if(C[i] != 0) return false;
	}

	return true;
}

bool go() {

	N = 0;
	for(int i = 0; i < K; i++) {
		N += C[i];
	}

	for(int i = 0; i < K; i++) {
		int len = N;
		if(i != P) len --;
		if(i != Q) len --;

		if(C[i] > (len + 1) / 2) return false;
	}

	for(int i = 0; i < K; i++) {
		int len = N;
		if(i != P) len --;
		if(i != Q) len --;

		if(C[i] * 2 - 1 == len) {
			int ind = (i == P) ? 0 : 1;
			for(int v = 0; v < C[i]; v++) {
				V[ind] = i;
				ind += 2;
			}

			V[0] = P;
			V[N - 1] = Q;

			ind = (i == P) ? 1 : 2;
			for(int c = 0; c < K; c++) {
				if(c == i) continue;
				int cnt = C[c];
				if(c == P) cnt --;
				if(c == Q) cnt --;
				for(int v = 0; v < cnt; v++) {
					V[ind] = c;
					ind += 2;
				}
			}
		}
	}

	for(int i = 0; i < K; i++) [
		int len = N;
		if(i != P) len --;
		if(i != Q) len --;

	]

	if(P == Q) {
	}

	return true;
}


int main() {
	scanf("%d%d%d", &K, &P, &Q);

	for(int i = 0; i < K; i++) {
		scanf("%d", &C[i]);
	}

	bool res = go();

	if(res) {

		assert(verify());

		printf("%d", V[0] + 1);
		for(int i = 1; i < N; i++) {
			printf(" %d", V[i] + 1);
		}
		printf("\n");
	} else {
		printf("0\n");
	}

	return 0;
}
