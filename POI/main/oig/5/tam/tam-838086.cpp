#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e9;

ll ceil(ll a, ll b) {
	return (a + b - 1) / b;
}

const int MAXN = int(5e5) + 200;
int N;

int H[MAXN];
int R[MAXN];

const int MAXB = MAXN;
int B;
struct bin { 
	bin* left;
	bin* right;
	ll vl, vr;
	ll rl, rr;

	ll get_time() {
		return ceil(vl + vr, rl + rr);
	}
};

bin bins[MAXB];

ll solve() {
	{ // Generate bins
		B = 0;
		bin* prv = NULL;
		int l = 0;
		for (int r = 1; r <= N; r++) {
			assert(r > l);
			if (H[r] >= H[l]) {
				bins[B].vl = ll(r - l) * H[l];
				bins[B].rl = 0;
				for (int i = l; i < r; i++) {
					bins[B].rl += R[i];
				}
				if (H[l] == H[r]) {
					assert(false);
					bins[B].vr = bins[B].vl;
					bins[B].rr = bins[B].rl;
				} else {
					bins[B].vr = 0;
					bins[B].rr = 0;
					bins[B].vl *= 2;
					bins[B].rl *= 2;
				}
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
	typedef pair<ll, bin*> pdb;
	priority_queue<pdb> pq;
	for (int i = 0; i < B; i++) {
		pq.push(pdb(-bins[i].get_time(), &bins[i]));
	}
	while (!pq.empty()) {
		ll t = pq.top().first; bin* cur = pq.top().second; pq.pop();
		//cerr << -t << ' ' << int(cur - bins) << '\n';
		assert(cur);
		if (cur->left == cur) continue;
		//assert(t == -(cur->v / cur->r));
		if (cur->left == NULL) return -t;
		bin* l = cur->left;
		bin* r = cur->right;
		assert(l);
		l->vl += cur->vl;
		l->rl += cur->rl;
		pq.push(pdb(-l->get_time(), l));
		if (r) {
			r->vr += cur->vr;
			r->rr += cur->rr;
			pq.push(pdb(-r->get_time(), r));
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
	ll res = solve();
	reverse(H, H + N + 1);
	reverse(R, R + N);
	res = min(res, solve());
	printf("%lld\n", res);
	return 0;
}
