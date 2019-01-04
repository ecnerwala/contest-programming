#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lispp3.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();
const int MASTER = NODES - 1;

const int MAXN = int(1e9) + 5000;
const int N = GetLength();

template <typename T> T split(T n, T i, T tot) {
	return n / tot * i + min(i, n % tot);
}

const int MAXNODES = 120;
int levels[MAXNODES];
int bottoms[MAXNODES];

const int MAXD = MAXN / 100;
int has_plus[MAXD]; // location of the plus
bool is_mine[MAXD];
int needs[MAXD]; // location of the next plus if it exists

int go_seg(int st, int en) {
	int lvl = levels[ID];
	int base_level = bottoms[ID];
	memset(has_plus, 0, sizeof(has_plus));
	memset(is_mine, 0, sizeof(is_mine));
	memset(needs, 0, sizeof(needs));
	for (int i = st; i < en; i++) {
		assert(lvl >= base_level);
		char c = GetCharacter(i);
		if (c == '(') {
			lvl++;
			has_plus[lvl - base_level] = false;
			is_mine[lvl - base_level] = true;
		} else if (c == ')') {
			if (lvl == 0) return i;
			assert(i);
			if (GetCharacter(i-1) == '(') {
				// we're good, doesn't matter
			} else {
				// we must have a plus
				if (is_mine[lvl - base_level]) {
					if (!has_plus[lvl - base_level]) {
						return i;
					}
				} else {
					needs[lvl - base_level] = has_plus[lvl - base_level];
					if (has_plus[lvl - base_level]) {
						needs[lvl - base_level] = has_plus[lvl - base_level];
					} else {
						assert(i);
						needs[lvl - base_level] = -i;
					}
				}
			}
			lvl--;
		} else if (c == '+') {
			if (i == 0 || GetCharacter(i-1) != ')') {
				return i;
			}
			if (lvl == 0) return i;
			assert(i);
			// check the plus
			if (has_plus[lvl - base_level]) {
				return i;
			}
			has_plus[lvl - base_level] = i;
			if (i == N-1 || GetCharacter(i+1) != '(') {
				return i+1;
			}
		} else assert(false);
	}
	return N;
}

int main() {
	int st = split(N, ID, NODES), en = split(N, ID+1, NODES);
	int my_level = 0;
	int my_min_level = 0;
	for (int i = st; i < en; i++) {
		char c = GetCharacter(i);
		if (c == '(') my_level ++;
		if (c == ')') my_level --;
		my_min_level = min(my_min_level, my_level);
	}
	for (int i = 0; i < NODES; i++) {
		PutInt(i, my_level);
		PutInt(i, my_min_level);
		Send(i);
	}
	levels[0] = 0;
	for (int i = 0; i < NODES; i++) {
		Receive(i);
		levels[i+1] = levels[i] + GetInt(i);
		bottoms[i] = levels[i] + GetInt(i);
	}
	int res = go_seg(st, en);
	vector<int> plusses;
	{
		for (int oid = ID+1; oid < NODES; oid++) {
			for (int i = levels[ID+1]; i >= bottoms[ID+1] && i >= bottoms[ID]; i--) {
			}
			for (int i = bottoms[ID]; i <= levels[ID+1])
				if (has_plus[i - bottoms[ID]]) {
				}
			Send(oid);
		}
	}
	for (int oid = ID-1; oid >= 0; oid--) {
		// receive from the previous
		Receive(oid);
		int nres = GetInt(oid);
		res = min(res, nres);
		if (nres == N) {
			int sz = GetInt(oid);
			for (int i = 0; i < sz; i++)  {
				int v = GetInt(oid);
				if (v < bottoms[ID]) {
					plusses.push_back(v);
				} else {
					assert(v <= levels[ID]);
					if (v == bottoms[ID]) {
						assert(!is_mine[v-bottoms[ID]]);
						if (has_plus[v-bottoms[ID]]) {
							res = min(res, has_plus[v-bottoms[ID]]);
						} else {
							has_plus[v-bottoms[ID]] = 1;
						}
					} else {
						if (needs[v-bottoms[ID]] > 0) {
							res = min(res, needs[v-bottoms[ID]]);
						} else if (needs[v - bottoms[ID]] == 0) {
						} else if (needs[v - bottoms[ID]] < 0) {
							needs[v - bottoms[ID]] = 0;
						}
					}
				}
			}
		}
	}
	assert(needs[0] == 0);
	for (int i = bottoms[ID]; i <= levels[ID]; i++) {
		if (needs[i-bottoms[ID]] < 0) {
			res = min(res, -needs[i - bottoms[ID]]);
		}
	}
	if (ID < NODES - 1) {
		PutInt(ID+1, res);
		if (res == N) {
			for (int i = bottoms[ID]; i <= levels[ID+1]; i++) {
				if (has_plus[i - bottoms[ID]]) {
					plusses.push_back(i);
				}
			}
			PutInt(ID+1, int(plusses.size()));
			for (int i : plusses) {
				PutInt(ID+1, i);
			}
		}
		Send(ID+1);
	}
	if (ID == NODES-1) {
		assert(res <= N);
		assert(res >= 0);
		if (res == N && levels[NODES] == 0) {
			res = -1;
		}
		cout << res << '\n';
	}
	return 0;
}
