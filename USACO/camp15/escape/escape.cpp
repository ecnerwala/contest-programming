#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) { if(!b) exit(0); }

const int MAXN = 2e5;

struct node {
	node * c[2];
	int v;
	int mi, ma;

	node (int val) : c{NULL, NULL}, v(val), mi(v), ma(v) {}

	node (node* l, node* r, int val) : c{l, r}, v(val), mi(min(l->mi, r->mi) + v), ma(max(l->ma, r->ma) + v) {}

};

#define md ((l + r) / 2)

node* build(int l, int r) {
	if(l == r) return new node(0);
	else return new node(build(l, md), build(md + 1, r), 0);
}

// increment x and all after by v
node* insert(node* n, int l, int r, int x, int v) {
	assert(l <= x && x <= r);
	if(l == r) return new node(n->v + v);
	assert(n->c[0]); assert(n->c[1]);
	if(l == x) return new node(n->c[0], n->c[1], n->v + v);

	if(x <= md) {
		return new node(insert(n->c[0], l, md, x, v), insert(n->c[1], md + 1, r, md + 1, v), n->v);
	} else {
		return new node(n->c[0], insert(n->c[1], md + 1, r, x, v), n->v);
	}
}

// inclusive
int query_max(node* n, int l, int r, int a, int b) {
	//cerr << l << ' ' << r << ' ' << a << ' ' << b << '\n';
	assert(n);
	assert(l <= a && a <= b && b <= r);
	if(r - l == b - a) {
		return n->ma;
	}
	assert(r - l > 0);
	int res;
	if(b <= md) {
		res = query_max(n->c[0], l, md, a, b);
	} else if (a >= md + 1) {
		res = query_max(n->c[1], md + 1, r, a, b);
	} else {
		assert(a <= md && b >= md + 1);
		res = max(query_max(n->c[0], l, md, a, md), query_max(n->c[1], md + 1, r, md + 1, b));
	}
	res += n->v;
	//cerr << l << ' ' << r << ' ' << a << ' ' << b << ' ' << res << '\n';
	return res;
}
int query_min(node* n, int l, int r, int a, int b) {
	//cerr << l << ' ' << r << ' ' << a << ' ' << b << '\n';
	assert(n);
	assert(l <= a && a <= b && b <= r);
	if(r - l == b - a) {
		return n->mi;
	}
	assert(r - l > 0);
	int res;
	if(b <= md) {
		res = query_min(n->c[0], l, md, a, b);
	} else if (a >= md + 1) {
		res = query_min(n->c[1], md + 1, r, a, b);
	} else {
		assert(a <= md && b >= md + 1);
		res = min(query_min(n->c[0], l, md, a, md), query_min(n->c[1], md + 1, r, md + 1, b));
	}
	res += n->v;
	//cerr << l << ' ' << r << ' ' << a << ' ' << b << ' ' << res << '\n';
	return res;
}


int N;
int A[MAXN];
pair<int, int> vals[MAXN];

node *rt[MAXN];

// returns true if median <= vals[v]
// a, b, c, d 0-indexed and inclusive
// v is 0-indexed
bool check(int v, int a, int b, int c, int d) {
	//cerr << v << ' ' << a << ' ' << b << ' ' << c << ' ' << d << '\n';
	v++; // computed as prefix sums
	c++, d++; // we're doing prefix sums

	assert(0 < v && v <= N);

	node* n = rt[v];
	assert(n);


	//cerr << "hi\n";
	// return true if for all ranges, i..j, sum(i..j) is >= 0
	// returns true if pref(j + 1) - pref(i) >= 0 for all i..j
	// returns true if min(pref(j + 1)) - max(pref(i)) >= 0
	
	return query_min(n, 0, N, c, d) >= query_max(n, 0, N, a, b);
}

int query(int a, int b, int c, int d) {
	assert(0 <= a && a < b && b < c && c < d && d < N);
	int mi = -1;
	int ma = N - 1;
	assert(check(ma, a, b, c, d));
	while(ma - mi > 1) {
		int m = (mi + ma) / 2;
		if(check(m, a, b, c, d)) {
			ma = m;
		} else {
			mi = m;
		}
	}
	return vals[ma].first;
}

int Q;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> Q;
	for(int i = 0; i < N; i++) {
		cin >> A[i];
		vals[i] = pair<int, int> (A[i], i + 1);
	}
	sort(vals, vals + N);


	rt[0] = build(0, N);

	for(int i = 1; i <= N; i++) {
		rt[0] = insert(rt[0], 0, N, i, -1);
	}

	for(int i = 0; i < N; i++) {
		rt[i + 1] = insert(rt[i], 0, N, vals[i].second, 2);
		assert(rt[i + 1]);
	}

	int m = 0;
	for(int i = 0; i < Q; i++) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		a -= m, b -= m, c -= m, d -= m;
		if(a < 0) a += N;
		if(b < 0) b += N;
		if(c < 0) c += N;
		if(d < 0) d += N;
		assert(a >= 0 && b >= 0 && c >= 0 && d >= 0);

		wssert(0 <= a && a < b && b < c && c < d && d < N);

		m = query(a, b, c, d);
		cout << m << '\n';
		m %= N;
	}
}
