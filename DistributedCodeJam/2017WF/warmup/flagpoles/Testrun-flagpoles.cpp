#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "flagpoles.h"
#endif
// dummy file
#include <cassert>
#include "message.h"

long long GetNumFlagpoles() {
    return 0;
}

long long GetHeight(long long i) {
    assert(false);
    return 0;
}
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetNumFlagpoles() - 1;

struct data {
	ll len;
	ll plen;
	ll pval;
	ll slen;
	ll sval;
	ll mlen;

	data() : len(0), plen(0), pval(0), slen(0), sval(0), mlen(0) { }

	void append(ll val) {
		if (len == 0) {
			len = plen = slen = 1;
			pval = sval = val;
			mlen = 1;
		} else {
			len++;
			if (val == sval) {
			} else {
				slen = 0;
				sval = val;
			}
			slen++;
			mlen = max(mlen, slen);
			if (slen == len) {
				plen = len;
				pval = sval;
			}
		}
	}

	void append(data other) {
		if (len == 0) {
			*this = other;
			return;
		} else if (other.len == 0) {
			return;
		}
		mlen = max(mlen, other.mlen);
		if (sval == other.pval) {
			mlen = max(mlen, slen + other.plen);
		}
		if (other.slen == other.len && other.sval == sval) {
			slen += other.slen;
		} else {
			slen = other.slen;
		}
		sval = other.sval;
		if (plen == len && other.pval == pval) {
			plen += other.plen;
		}
		len += other.len;
	}

	void write_msg(int dest) {
		PutLL(dest, len);
		PutLL(dest, plen);
		PutLL(dest, pval);
		PutLL(dest, slen);
		PutLL(dest, sval);
		PutLL(dest, mlen);
	}
	void get_msg(int src) {
		len = GetLL(src);
		plen = GetLL(src);
		pval = GetLL(src);
		slen = GetLL(src);
		sval = GetLL(src);
		mlen = GetLL(src);
	}
};

ll get_boundary(int i) {
	return N / NODES * i + min(ll(i), N % NODES);
}

int main() {
	data d;
	ll st = get_boundary(ID), en = get_boundary(ID+1);
	for (ll i = st; i < en; i++) {
		ll diff = GetHeight(i+1) - GetHeight(i);
		d.append(diff);
	}
	if (ID > 0) {
		d.write_msg(0);
		Send(0);
	} else {
		for (int src = 1; src < NODES; src++) {
			Receive(src);
			data other; other.get_msg(src);
			d.append(other);
		}
		cout << d.mlen + 1 << '\n';
	}

	return 0;
}
