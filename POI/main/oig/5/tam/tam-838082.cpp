#include<bits/stdc++.h>
using namespace std;

typedef double ld;

const ld INF = 1e9;

const int MAXN = int(5e5) + 200;
int N;

int H[MAXN];
int R[MAXN];

const int MAXB = MAXN;
int B;
struct bin { 
	bin* left;
	bin* right;
	ld v;
	ld r;
	bool e;
};

bin bins[MAXB];

ld solve() {
	{ // Generate bins
		B = 0;
		bin* prv = NULL;
		int l = 0;
		for (int r = 1; r <= N; r++) {
			assert(r > l);
			if (H[r] >= H[l]) {
				bins[B].r = 0;
				for (int i = l; i < r; i++) {
					bins[B].r += R[i];
				}
				bins[B].v = ld(r - l) * H[l];
				bins[B].e = H[l] == H[r];
				bins[B].left = prv;
				bins[B].right = NULL;
				if(prv) prv->right = &bins[B];
				prv = &bins[B];
				l = r;
				B++;
			}
		}
	}
	if (B == 0) {
		return INF;
	}
	typedef pair<ld, bin*> pdb;
	priority_queue<pdb> pq;
	for (int i = 0; i < B; i++) {
		pq.push(pdb(-(bins[i].v / bins[i].r), &bins[i]));
	}
	while (!pq.empty()) {
		ld t = pq.top().first; bin* cur = pq.top().second; pq.pop();
		//cerr << -t << ' ' << int(cur - bins) << '\n';
		assert(cur);
		if (cur->left == cur) continue;
		//assert(t == -(cur->v / cur->r));
		if (cur->left == NULL) return -t;
		bin* l = cur->left;
		bin* r = cur->right;
		assert(l);
		assert(cur->v >= 0);
		assert(cur->r >= 0);
		if (cur->e) {
			l->v += cur->v / 2;
			l->r += cur->r / 2;
			pq.push(pdb(-(l->v / l->r), l));
			if (r) {
				r->v += cur->v / 2;
				r->r += cur->r / 2;
				pq.push(pdb(-(r->v / r->r), r));
			}
		} else {
			l->v += cur->v;
			l->r += cur->r;
			pq.push(pdb(-(l->v / l->r), l));
		}
		l->right = r;
		if (r) r->left = l;
		cur->left = cur->right = cur; // mark as done
	}
	assert(false);
	return INF;
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i <= N; i++) scanf("%d", &H[i]);
	for (int i = 0; i < N; i++) scanf("%d", &R[i]);
	ld res = solve();
	reverse(H, H + N + 1);
	reverse(R, R + N);
	res = min(res, solve());
	printf("%d\n", int(ceil(res)));
	return 0;
}
