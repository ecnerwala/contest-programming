#include<bits/stdc++.h>
using namespace std;

struct node {
	node* p = NULL;
	node* c[2] = {NULL, NULL};

	inline int d() { return this == p->c[1]; }
	inline int r() { return p == NULL || this != p->c[d()]; }

	bool flip = false;
	int val;
	int totval;
}

const int MAXN = 2e5;
const int MAXM = 2e5;
const int MAXT = 3e5;
const int MAXE = 3e5;

const int MAXV = MAXN + MAXE;

int N, M, T;
int K[MAXM];
pair<int, int> queries[MAXN]
int E;

int main() {
	return 0;
}
