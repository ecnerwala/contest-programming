#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "flagpoles.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetNumFlagpoles() - 1;

ll GetDiff(ll i) {
	return GetHeight(i+1) - GetHeight(i);
}

struct data {
	ll first;
	ll pref;

	ll last;
	ll suff;

	ll best;

	bool all;

	data() {
		first = 0;
		pref = 0;
		last = 0;
		suff = 0;
		best = 0;
		all = true;
	}

	data(ll a) {
		first = last = a;
		pref = suff = best = 1;
		all = true;
	}

	bool empty() const {
		return pref == 0;
	}

	void append(const data &o) {
		if (empty()) {
			*this = o;
			return;
		} else if (o.empty()) {
			return;
		}

		best = max(best, o.best);
		if (last == o.first) {
			best = max(best, suff + o.pref);
		}

		if (all && o.all && first == o.first) {
			pref += o.pref;
			suff += o.suff;
			return;
		}

		if (all && first == o.first) {
			pref += o.pref;
		}

		if (o.all && last == o.last) {
			suff += o.suff;
		} else {
			last = o.last;
			suff = o.suff;
		}

		all = false;
		return;
	}

};

void PutData(int o, const data &d) {
	PutLL(o, d.first);
	PutLL(o, d.pref);
	PutLL(o, d.last);
	PutLL(o, d.suff);
	PutLL(o, d.best);
	PutChar(o, d.all);
}

data GetData(int o) {
	data res;
	res.first = GetLL(o);
	res.pref = GetLL(o);
	res.last = GetLL(o);
	res.suff = GetLL(o);
	res.best = GetLL(o);
	res.all = GetChar(o);
	return res;
}

int main() {
	ll st = (N / NODES) * ll(ID) + min(N % NODES, ll(ID));
	ll en = (N / NODES) * ll(ID+1) + min(N % NODES, ll(ID+1));
	data cur;
	for (ll i = st; i < en; i++) {
		cur.append(data(GetDiff(i)));
	}
	if (ID != 0) {
		PutData(0, cur);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			cur.append(GetData(i));
		}
		cout << cur.best + 1 << '\n';
	}
	return 0;
}
