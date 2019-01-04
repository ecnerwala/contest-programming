#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;

typedef complex<int> pt;
#define X real()
#define Y imag()

int N;
pt P[MAXN];

int gcd(int a, int b) {
	if(a < 0) a = -a;
	if(b < 0) b = -b;
	while(a) {
		b %= a;
		swap(a, b);
	}
	return b;
}

int cross(pt a, pt b) {
	return a.X * b.Y - a.Y * b.X;
}

bool go() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d %d", &P[i].X, &P[i].Y);
	}

	int g = 0;
	for(int i = 0; i < N; i++) {
		bool bad = true;
		for(int j = 0; j < N; j++) {
			int v = cross(P[i], P[j]);
			if(v < 0) bad = false;
			g = gcd(g, v);
		}
		if(bad) return false;
	}

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
