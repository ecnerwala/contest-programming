#include<bits/stdc++.h>
using namespace std;

// gives wa if false
inline void wssert(bool b) {
	return;
	if(!b) {
		cout << "blah\n";
		exit(0);
	}
}

typedef long long ll;

const int MAXP = 2e6 + 200;
const int D = 21; // depth

const int S = 1 << D; // size of last row

const ll INF = 2e18;

struct node {
	// is leaf
	bool leaf;
	// left and right boundary of each range (inclusive)
	int s, t;
	// size of each range
	ll sz;
	// value of 1 in each range
	ll val;
	// number of cones living in this range: -INF means empty all
	ll lzy;
	// num cones in subtree
	ll cnt;
	// tot cost in subtree
	ll cst;
} N[S * 2];

ll T;

// compute cnt and cst
void update(int i) {
	if(N[i].lzy == -INF) {
		N[i].cnt = 0;
		N[i].cst = 0;
	} else {
		//wssert(N[i].lzy >= 0);
		N[i].cnt = N[i].lzy * N[i].sz;
		N[i].cst = N[i].lzy * N[i].val;
		if(N[i].leaf) {
			// no children
		} else {
			int l = i * 2, r = i * 2 + 1;
			//wssert(l < S * 2 && r < S * 2);

			N[i].cnt += N[l].cnt;
			N[i].cnt += N[r].cnt;

			N[i].cst += N[l].cst;
			N[i].cst += N[r].cst;
		}
	}
}

// propogate lazy downwards
void propogate(int i) {
	if(N[i].leaf) {
		// nothing to do
		if(N[i].lzy == -INF) {
			N[i].lzy = 0;
		}
	} else {
		int l = i * 2, r = i * 2 + 1;
		//wssert(r < S * 2);
		if(N[i].lzy == -INF) {
			N[l].lzy = N[r].lzy = -INF;
		} else {
			if(N[l].lzy == -INF) propogate(l);
			wssert(N[l].lzy >= 0);
			N[l].lzy += N[i].lzy;
			if(N[r].lzy == -INF) propogate(r);
			wssert(N[r].lzy >= 0);
			N[r].lzy += N[i].lzy;
		}
		N[i].lzy = 0;
		update(l);
		update(r);
	}
	wssert(N[i].lzy != -INF);
}

ll numcones() { return N[1].cnt; }
ll totcost() { return N[1].cst; }

void add(ll k, int p, int i) {
	T++;
	wssert(N[i].s <= p && p <= N[i].t);
	propogate(i);
	wssert(N[i].lzy != -INF);
	if(N[i].leaf) {
		N[i].lzy += k;
	} else {
		int l = i * 2, r = i * 2 + 1;
		wssert(r < S * 2);
		if(p <= N[l].t) {
			add(k, p, l);
		} else if(p >= N[r].s) {
			add(k, p, r);
		} else wssert(false);
	}
	update(i);
}
void add() {
	ll k, p; cin >> k >> p;
	add(k, p, 1);
}

void addrange(int a, int b, int i) {
	T++;
	wssert(N[i].s <= a && a <= b && b <= N[i].t);
	propogate(i);
	wssert(N[i].lzy != -INF);
	if(b - a + 1 == N[i].sz) {
		N[i].lzy ++;
	} else {
		int l = i * 2, r = i * 2 + 1;
		wssert(r < S * 2);
		if(a <= N[l].t) {
			addrange(a, min(b, N[l].t), l);
		}
		if(b >= N[r].s) {
			addrange(max(a, N[r].s), b, r);
		}
	}
	update(i);
}
void addrange() {
	int a, b; cin >> a >> b;
	addrange(a, b, 1);
}

ll buyamt(ll q, int i) {
	T++;
	wssert(q >= 0);
	wssert(q <= N[i].cst);

	if(!q) return 0;

	propogate(i);
	wssert(N[i].lzy != -INF);

	//cerr << s[i] << ' ' << t[i] << ' ' << N[i].cnt << ' ' << N[i].cst << ' ' << q << '\n';

	ll res = 0;
	if(q == N[i].cst) {
		res = N[i].cnt;
		N[i].lzy = -INF;
	} else {
		wssert(q < N[i].cst);
		if(N[i].leaf) {
			res = q / N[i].val;
			N[i].lzy -= res;
		} else {
			wssert(N[i].lzy == 0);

			int l = i * 2, r = i * 2 + 1;
			wssert(r < S * 2);

			if(q <= N[l].cst) {
				res += buyamt(q, l);
			} else {
				q -= N[l].cst;
				res += buyamt(N[l].cst, l);
				res += buyamt(q, r);
			}
		}
	}
	update(i);
	return res;
}
ll buyamt() {
	ll q; cin >> q;
	return buyamt(q, 1);
}

ll buylow(ll n, int i) {
	T++;
	wssert(n >= 0);
	wssert(n <= N[i].cnt);

	if(!n) return 0;

	propogate(i);

	//cerr << s[i] << ' ' << t[i] << ' ' << N[i].cnt << ' ' << N[i].cst << ' ' << n << '\n';

	ll res = 0;
	if(n == N[i].cnt) {
		res = N[i].cst;
		N[i].lzy = -INF;
	} else {
		wssert(n < N[i].cnt);
		if(N[i].leaf) {
			wssert(N[i].sz == 1);
			wssert(N[i].cnt == N[i].lzy);

			N[i].lzy -= n;
			res = n * N[i].val;
		} else {
			wssert(N[i].lzy == 0);

			int l = i * 2, r = i * 2 + 1;
			wssert(r < S * 2);

			if(n <= N[l].cnt) {
				res += buylow(n, l);
			} else {
				n -= N[l].cnt;
				res += buylow(N[l].cnt, l);
				res += buylow(n, r);
			}
		}
	}

	update(i);
	return res;
}
ll buylow() {
	ll n; cin >> n;
	return buylow(min(numcones(), n), 1);
}

ll buyhigh(ll n, ll i) {
	T++;
	wssert(n >= 0);
	wssert(n <= N[i].cnt);

	if(!n) return 0;

	propogate(i);

	//cerr << s[i] << ' ' << t[i] << ' ' << N[i].cnt << ' ' << N[i].cst << ' ' << n << '\n';

	ll res = 0;
	if(n == N[i].cnt) {
		res = N[i].cst;
		N[i].lzy = -INF;
	} else {
		wssert(n < N[i].cnt);
		if(N[i].leaf) {
			wssert(N[i].sz == 1);
			wssert(N[i].cnt == N[i].lzy);

			N[i].lzy -= n;
			res = n * N[i].val;
		} else {
			wssert(N[i].lzy == 0);

			int l = i * 2, r = i * 2 + 1;
			wssert(r < S * 2);

			if(n <= N[r].cnt) {
				res += buyhigh(n, r);
			} else {
				n -= N[r].cnt;
				res += buyhigh(N[r].cnt, r);
				res += buyhigh(n, l);
			}
		}
	}

	update(i);
	return res;
}
ll buyhigh() {
	ll n; cin >> n;
	return buyhigh(min(numcones(), n), 1);
}

ll cost(ll n, ll i) {
	T++;
	wssert(n > 0);
	wssert(n <= N[i].cnt);

	propogate(i);

	if(N[i].leaf) {
		return N[i].val;
	} else {
		wssert(N[i].lzy == 0);

		int l = i * 2, r = i * 2 + 1;
		wssert(r < S * 2);

		if(n <= N[l].cnt) {
			return cost(n, l);
		} else {
			return cost(n - N[l].cnt, r);
		}
	}
}

ll cost() {
	ll n; cin >> n;
	if(n > numcones()) return -1;
	else return cost(n, 1);
}

void init() {
	for(int i = 0; i < S; i++) {
		N[S + i].s = N[S + i].t = i;
		N[S + i].sz = 1;
		N[S + i].val = i;
		N[S + i].lzy = 0;
		N[S + i].leaf = true;
		update(S + i);
	}
	for(int i = S - 1; i; i--) {
		int l = i * 2, r = i * 2 + 1;
		N[i].s = N[l].s;
		N[i].t = N[r].t;
		N[i].sz = N[l].sz + N[r].sz;
		N[i].val = N[l].val + N[r].val;
		N[i].lzy = 0;
		N[i].leaf = false;
		update(i);
	}
}
int main() {
	ios_base::sync_with_stdio(0);

	init();

	int Q; cin >> Q;
	for(int i = 0; i < Q; i++) {
		string op; cin >> op;
		T = 0;
		if(op == "ADD") {
			add();
		} else if(op == "ADDRANGE") {
			addrange();
		} else if(op == "BUYAMT") {
			cout << buyamt() << '\n';
		} else if(op == "BUYLOW") {
			cout << buylow() << '\n';
		} else if(op == "BUYHIGH") {
			cout << buyhigh() << '\n';
		} else if(op == "COST") {
			cout << cost() << '\n';
		} else if(op == "NUMCONES") {
			cout << numcones() << '\n';
		} else if(op == "TOTALCOST") {
			cout << totcost() << '\n';
		}
		assert(T <= D * 4);
	}

}

