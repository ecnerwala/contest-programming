#include<bits/stdc++.h>
using namespace std;

mt19937 mt(1132094);
struct treap {
	treap* ch[2] = {nullptr, nullptr};

	mt19937::result_type pri = mt();
	mt19937::result_type key2 = mt();
	int key1, val;

	int minVal;
	treap() : key1(0), val(0) { update(); }
	treap(int key_, int val_): key1(key_), val(val_) { update(); }

	void update() {
		minVal = val;
		if (ch[0]) minVal = min(minVal, ch[0]->minVal);
		if (ch[1]) minVal = min(minVal, ch[1]->minVal);
	}
};

void split(int key1, mt19937::result_type key2, treap* r, treap* &a, treap* &b) {
	if (r == nullptr) {
		a = b = nullptr;
		return;
	}
	assert(r);
	if (make_pair(r->key1, r->key2) >= make_pair(key1, key2)) {
		// r is to the right
		b = r;
		split(key1, key2, r->ch[0], a, b->ch[0]);
	} else {
		a = r;
		split(key1, key2, r->ch[1], a->ch[1], b);
	}
	r->update();
}

void merge(treap* a, treap* b, treap* &r) {
	if (a == nullptr) {
		r = b;
		return;
	}
	if (b == nullptr) {
		r = a;
		return;
	}
	assert(a && b);
	if (a->pri < b->pri) {
		r = a;
		merge(a->ch[1], b, r->ch[1]);
	} else {
		r = b;
		merge(a, b->ch[0], r->ch[0]);
	}
	r->update();
}

const int MAXN = 3.1e5;
int N;
int K;
char T[MAXN];

int dp[MAXN];

treap* root = nullptr;
treap nodes[MAXN];

int main() {
	if(fopen("redistricting.in", "r")) {
		(void)freopen("redistricting.in", "r", stdin);
		(void)freopen("redistricting.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K >> T;
	int pref = 0;
	dp[0] = 0;
	for (int i = 0; i <= N; i++) {
		if (i > 0) { // do the query
			if (T[i-1] == 'G') {
				pref--;
			} else if (T[i-1] == 'H') {
				pref++;
			} else assert(false);

			treap *a, *b;
			split(pref, 0, root, a, b);

			// b is bad
			assert(a || b);
			if (a && b) {
				dp[i] = min(b->minVal + 1, a->minVal);
			} else if (b) {
				dp[i] = b->minVal + 1;
			} else if (a) {
				dp[i] = a->minVal;
			} else assert(false);

			merge(a, b, root);
		} else {
			dp[i] = 0;
		}

		{ // insert the new node
			nodes[i] = treap(pref, dp[i]);
			treap *a, *b;
			split(nodes[i].key1, nodes[i].key2, root, a, b);
			merge(a, &nodes[i], root);
			merge(root, b, root);
		}

		if (i >= K) {
			treap *a, *b;
			split(nodes[i-K].key1, nodes[i-K].key2, root, a, b);
			treap *m;
			split(nodes[i-K].key1, nodes[i-K].key2+1, b, m, b);
			assert(m == &nodes[i-K]);
			merge(a, b, root);
		}
	}

	cout << dp[N] << '\n';
	return 0;
}
