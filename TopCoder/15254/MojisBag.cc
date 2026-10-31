#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MojisBag {

	static const int MOD = int(1e9)+7;

	static const int S = 1<<17;
	vector<int> seg[2*S];

	struct trie {
		trie *ch[2] = {nullptr, nullptr};
		int cnt = 0;
		~trie() { delete ch[0]; delete ch[1]; }
	};

	trie* ROOT = nullptr;

	int check(trie* a, int p, int d = 30) {
		if (!a || !a->cnt) return 0;
		assert(a);
		assert(a->cnt);

		int dir = (p >> (d-1)) & 1;
		if (a->ch[!dir] && a->ch[!dir]->cnt) {
			return check(a->ch[!dir], p, d-1) | (1 << (d-1));
		} else {
			return check(a->ch[dir], p, d-1);
		}
	}

	void incr(trie* &a, int p, int v, int d = 30) {
		if (!a) a = new trie();
		assert(a);
		a->cnt += v;

		if (d == 0) {
			return;
		} else {
			int dir = (p >> (d-1)) & 1;
			incr(a->ch[dir], p, v, d-1);
		}
	}

public:

	int ans[S];
	void comp(int i, int best) {
		for (int v : seg[i]) {
			best = max(best, check(ROOT, v));
			incr(ROOT, v, +1);
		}
		if (i >= S) {
			ans[i-S] = best;
		} else {
			comp(2 * i, best);
			comp(2 * i + 1, best);
		}
		for (int v : seg[i]) {
			incr(ROOT, v, -1);
		}
	}

	int maximumXor(int Q, int base, int add, int rate) {
		vector<int> X(Q);
		X[0] = add;
		for (int i = 0; i+1 < Q; i++) {
			X[i+1] = int((1ll * X[i] * base + add) % MOD);
		}

		vector<pair<int, pair<int, int>>> elts;

		for (int i = 0; i < Q; i++) {
			if (X[i] % rate) {
				// insert X[i]
				elts.emplace_back(X[i], pair<int, int>(i, Q));
			} else {
				// erase X[i] % (# inserts)
				if (elts.empty()) continue;
				int a = (X[i] % int(elts.size()));
				elts[a].second.second = min(elts[a].second.second, i);
			}
		}

		for (auto it : elts) {
			int v = it.first;
			int a = it.second.first, b = it.second.second;
			for (a += S, b += S; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					seg[a].push_back(v);
					++a;
				}
				if (b & 1) {
					--b;
					seg[b].push_back(v);
				}
			}
		}

		comp(1, 0);

		int Z = 0;
		for (int i = 0; i < Q; i++) {
			Z = (1ll * Z * base + ans[i]) % MOD;
		}

		return Z;
	}
};

}  // namespace ecnerwala

using ecnerwala::MojisBag;

// vim:ft=cpp
