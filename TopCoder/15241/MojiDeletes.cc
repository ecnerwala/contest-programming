#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MojiDeletes {
	using ll = long long;
	const int MOD = int(1e9)+7;

	struct trie;
	trie* EMPTY;
	struct trie {
		trie* ch[2];
		int cnt;
	};

	trie* incr(const trie* a, int p, int d = 30) {
		trie* r = new trie(*a);
		r->cnt ++;
		if (d > 0) {
			int dir = (p >> (d-1)) & 1;
			r->ch[dir] = incr(r->ch[dir], p, d-1);
		}
		return r;
	}

	int check(const trie* a, const trie* b, int p, int d = 30) {
		assert(b->cnt - a->cnt > 0);
		if (d == 0) {
			return 0;
		}

		int dir = (p >> (d-1)) & 1;

		if (b->ch[!dir]->cnt - a->ch[!dir]->cnt > 0) {
			return check(a->ch[!dir], b->ch[!dir], p, d-1) | (1 << (d-1));
		} else {
			return check(a->ch[dir], b->ch[dir], p, d-1);
		}
	}


public:

	long long maximumXor(int N, int Q, int Base, int Add, int QBase, int QAdd) {

		EMPTY = new trie();
		EMPTY->ch[0] = EMPTY->ch[1] = EMPTY;
		EMPTY->cnt = 0;

		vector<int> A(N);
		A[0] = Add;
		for (int i = 1; i < N; i++) {
			A[i] = int((1ll * A[i-1] * Base + Add) % MOD);
		}

		vector<int> xors;
		vector<trie*> roots;
		xors.push_back(0);
		roots.push_back(EMPTY);
		for (int i = 0; i < N; i++) {
			xors.push_back(xors.back() ^ A[i]);
			roots.push_back(incr(roots.back(), A[i]));
		}
		assert(int(roots.size()) == N+1);

		vector<int> L(Q), R(Q);
		L[0] = QAdd % N, R[0] = int((1ll * L[0] * QBase + QAdd) % N);
		for (int i = 1; i < Q; i++) {
			L[i] = int((1ll * R[i-1] * QBase + QAdd) % N);
			R[i] = int((1ll * L[i] * QBase + QAdd) % N);
		}

		ll res = 0;
		for (int q = 0; q < Q; q++) {
			int a = min(L[q], R[q]), b = max(L[q], R[q]) + 1;
			int x = xors[a] ^ xors[b];
			res += check(roots[a], roots[b], x);
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::MojiDeletes;

// vim:ft=cpp
