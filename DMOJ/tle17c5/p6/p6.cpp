#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e5) + 200;
const int MAXM = int(2e5) + 200;
const int MAXV = MAXN + MAXM;
int N, M, V;
int P[MAXM][2];

bool vals[MAXV];

char res[MAXN];

bool eval() {
	for (int i = 0; i < M; i++) {
		// NAND
		vals[i+N] = !(vals[P[i][0]] && vals[P[i][1]]);
	}
	return vals[V-1];
}

bool eval_split(int p) {
	for (int i = 0; i < N; i++) {
		vals[i] = (i >= p);
	}
	return eval();
}

int main() {
	scanf("%d %d", &N, &M);
	assert(N >= 1);
	V = N + M;
	for (int i = 0; i < M; i++) {
		int a, b; scanf("%d %d", &a, &b); a--, b--;
		assert(0 <= a && a < (N+i) && 0 <= b && b < (N+i));
		P[i][0] = a, P[i][1] = b;
	}

	int mi = 0, ma = N;
	bool vmi = eval_split(mi), vma = eval_split(ma);

	if (vmi == vma) {
		// we can just pick the constant function
		for (int i = 0; i < N; i++) res[i] = '0';
	} else {
		// we'll binary search for a transition point
		assert(vmi + vma == 1);
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			bool vmd = eval_split(md);
			if (vmd == vmi) {
				mi = md;
			} else {
				ma = md;
			}
		}
		assert(0 <= mi && mi < N);
		// mi is the one that twiddles
		for (int i = 0; i < N; i++) {
			res[i] = (i == mi) ? 'x' : ((i < mi) ? '0' : '1');
		}
	}
	res[N] = '\0';
	printf("%s\n", res);

	return 0;
}
