#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 6e5;

ll M, D;
int N;
ll X[MAXN];

int go() {
	ll pos = 0;
	for(int i = 0; i < N; i++) {
		if(X[i] <= (D - pos)) return 0;
		pos += X[i] - (D - pos);
		if(pos >= M) {
			return i + 1;
		} else if(pos >= D) {
			return 0;
		}
	}
	return false;
}

int main() {
	scanf("%lld %lld %d", &M, &D, &N);
	for(int i = 0; i < N; i++) {
		scanf("%lld", &X[i]);
	}
	sort(X, X + N);
	reverse(X, X + N);
	int res = go();
	if(res) {
		printf("%d\n", res);
	} else {
		int r;
		for(r = 0; r < N && X[r] >= M - D; r++);
		r--;
		if(r == -1) printf("0\n");
		else {
			M = D - (X[r] - (M - D)) / 2;
			for(int i = r; i + 1 < N; i++) {
				X[i] = X[i + 1];
			}
			N--;
			res = go();
			if(res) res++;
			printf("%d\n", res);
		}
	}
	return 0;
}
