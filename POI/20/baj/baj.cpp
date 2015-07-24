#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;

int N;
int X[MAXN];

int go() {
	if(X[0] == 1) {
		int res = 0;
		for(int i = 0; i < N; i++) {
			res += 1 - X[i];
		}
		return res;
	} else if(X[0] == 0) {
		for(int i = 0; i < N; i++) {
			if(X[i] == -1) {
				return -1;
			} else if(X[i] == 1) {
				int res = 0;
				for(int j = i; j < N; j++) {
					res += 1 - X[j];
				}
				return res;
			}
		}
		return 0;
	} else if(X[0] == -1) {
		int res;

		int val = 0;
		for(int i = 0; i < N; i++) {
			val += X[i] - (-1);
		}

		res = val;

		X[N] = 1;
		int r = N;

		while(r >= 0) {
			assert(X[r] == 1);
			int v = val;
			for(int l = r - 1; l >= 0; l--) {
				if(X[l] == -1) {
					break;
				} else if(X[l] == 1) {
					v --;
					break;
				} else {
					v--;
				}
			}
			res = min(res, v);
			do {
				r--;
				if(r < 0) break;
				val -= X[r] - (-1);
				val += 1 -  X[r];
			} while(X[r] != 1);
		}
		return res;
	} else {
		return -1;
	}
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d", &X[i]);
	}

	int res = go();
	if(res == -1) printf("BRAK\n");
	else printf("%d\n", res);

	return 0;
}

// format:
// a bunch: decrease to -1
// at most 1: decrease to 0
// a bunch: leave at 0
// a bunch: increase to 1
