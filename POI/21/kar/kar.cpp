#include<bits/stdc++.h>
using namespace std;

#define A first
#define B second
typedef pair<int, int> pii;
typedef pair<pii, pii> ppp;

const int INF = 1e9;

const int MAXN = 3e5;
int N;

const int S = 1 << 19;

ppp seg[S * 2];

const int MAXM = 2e6;
int M;

int eval(int a, ppp r) {
	if(a <= r.A.A) return r.A.B;
	else if(a <= r.B.A) return r.B.B;
	else return INF;
}

ppp merge(ppp l, ppp r) {
	l.A.B = eval(l.A.B, r);
	l.B.B = eval(l.B.B, r);
	return l;
}

void update(int a) {
	for(int i = (a + S) / 2; i; i /= 2) {
		int l = 2 * i, r = 2 * i + 1;
		seg[i] = merge(seg[l], seg[r]);
	}
}

bool query(int a, int b) {
	swap(seg[S + a], seg[S + b]);
	update(a);
	update(b);

	return seg[1].A.B < INF;
}

int main() {
	scanf("%d", &N);

	for(int i = 0; i < S; i++) {
		seg[S + i] = ppp(pii(INF - 1, INF - 1), pii(INF - 1, INF - 1));
	}

	for(int i = 0; i < N; i++) {
		int x, y;
		scanf("%d%d", &x, &y);

		if(x > y) swap(x, y);

		assert(x <= y);

		seg[S + i] = ppp(pii(x, x), pii(y, y));
	}

	for(int i = S - 1; i > 0; i--) {
		seg[i] = merge(seg[2 * i], seg[2 * i + 1]);
	}

	scanf("%d",  &M);
	for(int q = 0; q < M; q++) {
		int a, b;
		scanf("%d%d", &a, &b);
		a--, b--;

		bool res = query(a, b);
		printf((res ? "TAK\n" : "NIE\n"));
	}
	return 0;
}
