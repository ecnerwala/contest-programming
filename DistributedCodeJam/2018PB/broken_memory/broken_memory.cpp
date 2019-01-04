#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "broken_memory.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

using hash_t = unsigned long long;
const int MAXL = 1.1e7;
const int L = int(GetLength());
using hash2_t = pair<hash_t, hash_t>;
hash2_t operator - (const hash2_t &a, const hash2_t &b) {
	return hash2_t(a.first - b.first, a.second - b.second);
}
hash2_t pref[MAXL+1];

int split (int a, int i, int n) {
	return (a / n) * i + min(a % n, i);
}
int split (int a, int b, int i, int n) {
	return split(b-a, i, n) + a;
}

const int ARY = 500;

int main() {
	assert(NODES % 2 == 0);
	assert(L < MAXL);
	pref[0] = hash2_t(0,0);
	for (int i = 0; i < L; i++) {
		hash_t v = hash_t(GetValue(i));
		pref[i+1] = hash2_t(pref[i].first + v, pref[i].second + v * hash_t(i));
	}

	vector<pair<int, int>> ranges;
	vector<int> differences;

	assert(L >= 2);
	ranges.emplace_back(0, L);

	while (!ranges.empty()) {
		for (auto it : ranges) {
			int a = it.first, b = it.second;
			for (int i = 0; i < ARY; i++) {
				int l = split(a,b,i,ARY), r = split(a,b,i+1,ARY);
				hash2_t h = pref[r] - pref[l];
				PutLL(ID^1, ll(h.first));
				PutLL(ID^1, ll(h.second));
			}
		}
		Send(ID^1);

		vector<pair<int, int>> nranges;
		Receive(ID^1);
		for (auto it : ranges) {
			int a = it.first, b = it.second;
			for (int i = 0; i < ARY; i++) {
				int l = split(a,b,i,ARY), r = split(a,b,i+1,ARY);
				hash2_t h = pref[r] - pref[l];
				hash2_t o;
				o.first = hash_t(GetLL(ID^1));
				o.second = hash_t(GetLL(ID^1));
				if (h != o) {
					assert(l < r);
					if (r-l == 1) {
						differences.push_back(l);
					} else {
						nranges.emplace_back(l, r);
					}
				}
			}
		}
		ranges = nranges;
	}
	assert(differences.size() == 2);
	PutInt((ID+2)%NODES, differences[0]);
	Send((ID+2)%NODES);
	{
		int n = Receive((ID+NODES-2) % NODES);
		int d = GetInt(n);
		PutLL(n, GetValue(d));
		Send(n);
	}

	Receive((ID+2)%NODES);
	ll v = GetLL((ID+2)%NODES);
	if (v == GetValue(differences[0])) {
		PutInt(0, differences[1]);
	} else {
		PutInt(0, differences[0]);
	}
	Send(0);
	if (ID == 0) {
		for (int n = 0; n < NODES; n++) {
			Receive(n);
			cout << GetInt(n) << (n == NODES-1 ? '\n' : ' ');
		}
	}

	return 0;
}
