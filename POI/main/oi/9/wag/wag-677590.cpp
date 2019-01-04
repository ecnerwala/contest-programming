#include<bits/stdc++.h>
using namespace std;

struct node {
	node* prv;
	int val;
	int cnt;

	node() : prv(NULL), val(0), cnt(1) {}
	node(node* p, int v) : prv(p), val(v), cnt(1) {}
};

void dec(node* n) {
	while(n && !(--n->cnt)) {
		node* p = n->prv;
		delete n;
		n = p;
	}
}

const int MAXN = 2e3;
const int MAXW = 2e5;

const int M = 1e5;
int N;
int W[MAXN];
node* dp[MAXW];
node* ndp[MAXW];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%d", &W[i]);
	sort(W, W + N);
	dp[M] = new node();
	int S = 0;
	for(int z = 0; z < N; z++) {
		int w = W[z];
		for(int i = -S; i <= S; i++) {
			if(dp[i + M]) {
				if(!ndp[i - w + M]) {
					dp[i + M]->cnt++;
					ndp[i - w + M] = new node(dp[i + M], i - w);
				}
				if(!ndp[i + w + M]) {
					dp[i + M]->cnt++;
					ndp[i + w + M] = new node(dp[i + M], i + w);
				}
			}
		}
		S += w;
		for(int i = -S; i <= S; i++) {
			if(ndp[i + M]) {
				dp[i + M] = ndp[i + M];
				ndp[i + M] = NULL;
			}
		}
	}
	vector<int> a, b;
	for(node* res = dp[M]; res->prv; res = res->prv) {
		if(res->prv->val < res->val) {
			a.push_back(res->val - res->prv->val);
		} else {
			b.push_back(res->prv->val - res->val);
		}
	}

	cout << a.size() << ' ' << b.size() << '\n';
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	assert(!a.empty());
	assert(!b.empty());
	if(!a.empty()) {
		cout << a[0];
		for(int i = 1; i < int(a.size()); i++) {
			cout << ' ' << a[i];
		}
	}
	cout << '\n';

	if(!b.empty()) {
		cout << b[0];
		for(int i = 1; i < int(b.size()); i++) {
			cout << ' ' << b[i];
		}
	}
	cout << '\n';
}
