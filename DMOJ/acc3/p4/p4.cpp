#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = (1 << 20);
struct node {
	ll sz;
	ll tot;

	ll cterm;
	ll aterm;
} seg[S*2];

void update(int i) {
	if (i >= S) {
		seg[i].sz = 1;
		seg[i].tot = 0;
	} else {
		seg[i].sz = seg[2*i].sz + seg[2*i+1].sz;
		seg[i].tot = seg[2*i].tot + seg[2*i+1].tot;
	}
	seg[i].tot += seg[i].cterm * seg[i].sz;
	seg[i].tot += seg[i].aterm * (seg[i].sz * (seg[i].sz + 1) / 2);
}

void downdate(int i) {
	assert(i);
	if (i >= S) {
		return;
	} else {
		seg[2*i].cterm += seg[i].cterm;
		seg[2*i+1].cterm += seg[i].cterm;
		seg[2*i].aterm += seg[i].aterm;
		seg[2*i+1].aterm += seg[i].aterm;
		seg[2*i+1].cterm += seg[i].aterm * seg[2*i].sz;

		update(2*i);
		update(2*i+1);
	}
}

void increment(int l, int r, int a, int c = 0, int i = 1, int st = 0, int en = S) {
	assert(en - st == seg[i].sz);
	assert(st <= l && l < r && r <= en);
	if (r - l == en - st) {
		seg[i].aterm += a;
		seg[i].cterm += c;
		update(i);
		return;
	}
	downdate(i);
	int md = (st + en) / 2;
	if (l < md) {
		increment(l, min(r, md), a, c, 2*i, st, md);
		c += a * (min(r, md) - l);
	}
	if (r > md) {
		increment(max(l, md), r, a, c, 2*i+1, md, en);
		c += a * (r - max(l, md));
	}
	update(i);
}

ll sum(int l, int r, int i = 1, int st = 0, int en = S) {
	assert(en - st == seg[i].sz);
	assert(st <= l && l < r && r <= en);
	if (r - l == en - st) {
		return seg[i].tot;
	}
	downdate(i);
	ll res = 0;
	int md = (st + en) / 2;
	if (l < md) {
		res += sum(l, min(r, md), 2*i, st, md);
	}
	if (r > md) {
		res += sum(max(l, md), r, 2*i+1, md, en);
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, Q; cin >> N >> Q;
	for (int i = 2*S-1; i; i--) {
		update(i);
	}
	for (int q = 0; q < Q; q++) {
		int op;
		int l, r;
		cin >> op >> l >> r;
		l--;
		if (op == 1) {
			int a; cin >> a;
			increment(l, r, a);
		} else if (op == 2) {
			cout << sum(l, r) << '\n';
		} else assert(false);
	}

	return 0;
}
