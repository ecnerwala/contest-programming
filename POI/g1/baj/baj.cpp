#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

template <int S> struct ds {
	int par[S+10];
	int get_par(int a) {
		return par[a] == -1 ? a : (par[a] = get_par(par[a]));
	}
	bool merge(int a, int b) {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		par[a] = b;
		return true;
	}
	ds() { memset(par, -1, sizeof(par)); }
};

const int MAXN = 7050;
int N;
const int MAXM = int(3e5) + 200;
int M;
pii E[MAXM];

pii W[MAXM];

bool res[MAXM];
ds<MAXN> g; 

int main() {
	scanf("%d %d", &N, &M);
	for (int i = 0; i < M; i++) {
		scanf("%d %d %d", &E[i].first, &E[i].second, &W[i].first);
		E[i].first --, E[i].second --;
		W[i].second = i;
	}
	sort(W, W + M);
	for (int i = 0; i < M;) {
		int v = W[i].first;
		for (int j = i; j < M && W[j].first == v; j++) {
			int cur = W[j].second;
			int a = E[cur].first, b = E[cur].second;
			a = g.get_par(a), b = g.get_par(b);
			res[cur] = (a != b);
		}
		while (i < M && W[i].first == v) {
			int cur = W[i].second;
			int a = E[cur].first, b = E[cur].second;
			g.merge(a,b);
			i++;
		}
	}

	for (int i = 0; i < M; i++) {
		printf(res[i] ? "TAK\n" : "NIE\n");
	}

	return 0;
}
