#include <bits/stdc++.h>

using namespace std;

mt19937_64 mt(48);
struct treap_node {
	treap_node* c[2] = { nullptr, nullptr };
	int sz;
	mt19937_64::result_type pri = mt();

	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node* r;
	if (a->pri < b->pri) {
	//if (mt() % (a->sz + b->sz) < a->sz) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

std::pair<treap_node*, treap_node*> split(treap_node* r, int k) {
	if (k == 0) {
		return {nullptr, r};
	}
	assert(r);
	assert(0 <= k && k <= r->sz);
	if (k == r->sz) {
		return {r, nullptr};
	}

	treap_node *a, *b;
	int idx = r->c[0] ? r->c[0]->sz : 0;
	if (idx < k) {
		// we're on the left side
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], k - idx - 1);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], k);
	}
	r->update();
	return {a,b};
}

void solve(string S) {
	const int N = int(1e6);
	//const int N = int(S.size());
	vector<int> A;
	for (char c : S) {
		A.push_back(c - '1');
	}
	A.resize(N);
	iota(A.begin() + int(S.size()), A.end(), int(S.size()));

	vector<int> nxt(N);
	for (int i = 0; i < N; i++) {
		nxt[A[i]] = A[(i+1)%N];
	}

	int cur = A[0];
	for (int z = 0; z < int(1e7); z++) {
		int a = nxt[cur];
		int b = nxt[a];
		int c = nxt[b];

		nxt[cur] = nxt[c];

		int dest = cur-1;
		while (true) {
			if (dest < 0) dest += N;
			if (dest == a || dest == b || dest == c) {
				dest--;
				continue;
			}
			break;
		}

		nxt[c] = nxt[dest];
		nxt[dest] = a;

		cur = nxt[cur];
	}

	int a = nxt[0]+1;
	int b = nxt[nxt[0]]+1;
	cerr << a << ' ' << b << '\n';
	cout << int64_t(a)*b << '\n';
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	
	solve("389125467");
	solve("364297581");

	return 0;
}
