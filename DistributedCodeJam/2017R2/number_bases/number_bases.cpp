#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "number_bases.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 4e18;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetLength();

struct bases {
	ll base = 2;
	char opt = 0; // 0 is min-base, 1 is fixed, -1 is impossible

	bases() {}
	bases(ll b, char o) : base(b), opt(o) {}

	void merge(const bases &b) {
		if (opt == -1 || b.opt == -1) {
			opt = -1;
		} else if (opt == 0 && b.opt == 0) {
			base = max(base, b.base);
		} else if (opt == 0 && b.opt == 1) {
			if (base <= b.base) {
				base = b.base;
				opt = 1;
			} else {
				opt = -1;
			}
		} else if (opt == 1) {
			if (b.opt == 1 && base == b.base) {
				// we're good
			} else if (b.opt == 0 && b.base <= base) {
			} else {
				opt = -1;
			}
		} else assert(false);
	}
};

struct data {
	ll len = 0;
	pair<bases, bool> base[2] = {
		pair<bases, bool>(bases(), false),
		pair<bases, bool>(bases(), false)
	};

	void append(const data &d) {
		if (d.len == 0) {
			return;
		}
		if (len == 0) {
			*this = d;
			return;
		}
		for (int z = 0; z < 2; z++) {
			bool c = base[z].second;
			base[z].first.merge(d.base[c].first);
			base[z].second = d.base[c].second;
		}
		len += d.len;
	}

	data () {}

	data (ll a, ll b, ll c) {
		for (int z = 0; z < 2; z++) {
			ll s = a + b + z;
			if (s == c) {
				base[z] = pair<bases, bool>(bases{c+1, 0}, false);
			} else if (s > c && (s - c) > max(max(a,b),c)) {
				// must carry forward
				base[z] = pair<bases, bool>(bases{s - c, 1}, true);
			} else {
				base[z] = pair<bases, bool>(bases{2, -1}, false);
			}

		}
		len = 1;
	}
};

ll split (int id) {
	return (N / NODES) * id + min(ll(id), N % NODES);
}

data GetData(ll i) {
	return data(GetDigitX(i), GetDigitY(i), GetDigitZ(i));
}

int main() {
	ll st = split(ID), en = split(ID + 1);
	data d;
	for (ll i = st; i < en; i++) {
		d.append(GetData(i));
	}
	if (ID) {
		PutLL(0, d.len);
		for (int z = 0; z < 2; z++) {
			PutLL(0, d.base[z].first.base);
			PutChar(0, d.base[z].first.opt);
			PutChar(0, d.base[z].second);
		}
		Send(0);
	} else {
		for (int id = 1; id < NODES; id++) {
			Receive(id);
			data o;
			o.len = GetLL(id);
			for (int z = 0; z < 2; z++) {
				o.base[z].first.base = GetLL(id);
				o.base[z].first.opt = GetChar(id);
				o.base[z].second = GetChar(id);
			}
			d.append(o);
		}
		assert(d.len == N);
		if (d.base[0].second || d.base[0].first.opt == -1) {
			cout << "IMPOSSIBLE" << '\n';
		} else if (d.base[0].first.opt == 0) {
			cout << "NON-UNIQUE" << '\n';
		} else {
			cout << d.base[0].first.base << '\n';
		}
	}
	return 0;
}
