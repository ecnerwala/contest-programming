#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;

int N;
int X[MAXN];
int Y[MAXN];

int gcd(int a, int b) {
	if(a < 0) a = -a;
	if(b < 0) b = -b;
	while(a) {
		b %= a;
		swap(a, b);
	}
	return b;
}

int cross(int a, int b) {
	return X[a] * Y[b] - X[b] * Y[a];
}

bool go() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d %d", &X[i], &Y[i]);
	}

	int g = 0;
	for(int i = 0; i < N; i++) {
		bool bad = (X[i] || Y[i]);
		for(int j = 0; j < N; j++) {
			int v = cross(i, j);
			if(v < 0) bad = false;
			g = gcd(g, v);
		}
		if(bad) return false;
	}
	cerr << g << '\n';

	return g == 1;
}

int main() {
	int K; scanf("%d", &K);
	for(int i = 0; i < K; i++) {
		bool res = go();
		if(res) {
			printf("TAK\n");
		} else {
			printf("NIE\n");
		}
	}
}
