#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e6;
int N;
int A[MAXN];
int S;

int main() {
	scanf("%d", &N);
	S = 0;
	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		S += A[i];
	}
	int res = 0;
	for(int l = 0, r = 0, v = 0; r <= N; ) {
		assert(l <= r);
		res = max(res, min(v, S - v));
		if(v < S - v) {
			if(r == N) break;
			v += A[r];
			r++;
		} else {
			if(l == N) break;
			v -= A[l];
			l++;
		}
	}
	cout << res << '\n';
	return 0;
}
