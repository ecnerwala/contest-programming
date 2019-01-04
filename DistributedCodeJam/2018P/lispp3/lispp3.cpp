#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lispp3.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXNODES = 120;
const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int L = GetLength();

int split(int a, int i) {
	return (a / NODES) * i + min(a % NODES, i);
}

int splits[MAXNODES];
int levels[MAXNODES];
int bottoms[MAXNODES];
int botplus[MAXNODES];

int res = L;

void go() {
	{ // Compute splits
		for (int i = 0; i <= NODES; i++) {
			splits[i] = split(L, i);
			if (0 < splits[i] && splits[i] < L && GetCharacter(splits[i] - 1) == '(' && GetCharacter(splits[i]) == ')') {
				splits[i] --;
			}
		}
	}

	int st = splits[ID], en = splits[ID+1];

	{ // First check all +'s
		for (int i = st; i < en; i++) {
			if (GetCharacter(i) == '+') {
				if (!(i-1 >= 0 && GetCharacter(i-1) == ')')) {
					res = min(res, i);
					break;
				}
				if (!(i+1 < L && GetCharacter(i+1) == '(')) {
					res = min(res, i+1);
					break;
				}
			}
		}
	}

	{ // Now do some totlevel stuff
		int totlevel = 0;
		int minlevel = 0;
		int minplus = -1;
		for (int i = st; i < en; i++) {
			char c = GetCharacter(i);
			if (c == '(') totlevel ++;
			else if (c == ')') {
				totlevel --;
				if (totlevel < minlevel) {
					minlevel = totlevel;
					minplus = -1;
				}
			} else if (c == '+') {
				if (totlevel == minlevel) {
					if (minplus == -1) {
						minplus = i;
					}
				}
			} else assert(false);
		}

		for (int i = 0; i < NODES; i++) {
			PutInt(i, totlevel);
			PutInt(i, minlevel);
			PutInt(i, minplus);
			Send(i);
		}
		levels[0] = 0;
		for (int i = 0; i < NODES; i++) {
			Receive(i);
			levels[i+1] = levels[i] + GetInt(i);
			bottoms[i] = levels[i] + GetInt(i);
			botplus[i] = GetInt(i);
		}
	}

	vector<char> needs{true}; // whether we need from the left
	vector<int> inds{-1}; // error indices for our needs
	vector<char> has; // $(bottom+1)-indexed stack

	int level = levels[ID];
	int bottom = levels[ID];
	for (int i = st; i < en; i++) {
		assert(int(has.size()) == level - bottom);
		assert(int(needs.size()) == levels[ID] - bottom + 1);
		assert(int(inds.size()) == int(needs.size()));

		// We want to find the total level
		char c = GetCharacter(i);
		if (c == '(') {
			has.push_back(false);
			level ++;
		} else if (c == ')') {
			level --;
			if (level < bottom) {
				assert(has.empty()); // stays empty, but shifts down one
				bottom = level;
				if (needs.back()) {
					assert(inds.back() == -1);
					inds.back() = i;
				}
				needs.push_back(true);
				inds.push_back(-1);
			} else {
				assert(!has.empty());
				if (has.back() == false && !(i && GetCharacter(i-1) == '(')) {
					res = min(res, i);
				}
				has.pop_back();
			}
		} else if (c == '+') {
			if (level == bottom) {
				if (needs.back()) {
					needs.back() = false;
					inds.back() = i;
				} else {
					res = min(res, i);
				}
			} else {
				assert(!has.empty());
				if (has.back() == true) {
					res = min(res, i);
				}
				has.back() = true;
			}
		} else assert(false);
	}
}

void finish() {
	if (ID != 0) {
		PutInt(0, res);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			res = min(res, GetInt(i));
		}
		if (res == L && levels[NODES] == 0) {
			res = -1;
		}
		cout << res << '\n';
	}
}

int main() {
	go();
	finish();
	return 0;
}
