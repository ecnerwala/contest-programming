#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "dodge.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int split (int a, int i, int n) {
	return (a / n) * i + min(a % n, i);
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int L = int(GetN());
int F = int(GetSpawningFrequency());

const int MAXF = 1.1e6;
const int MAXL = 31000;
const int MAXS = MAXL;
int cost[MAXS];

int RP[MAXL];
int CP[MAXL];
int ralive[MAXL]; // minimum value that it's alive for
int calive[MAXL]; // minimum column that it's alive for

vector<int> rtimes[MAXF];
void computeAlives() {
	// row i and row j kill each other if RP[i] - j == CP[j] - i <=> RP[i] + i == CP[j] + j % F
	for (int i = 0; i < L; i++) {
		RP[i] = int(RowGeneratorPosition(i));
		rtimes[(RP[i]+i)%F].push_back(i);
		ralive[i] = 0;
	}
	for (int j = L-1; j >= 0; j--) {
		CP[j] = int(ColumnGeneratorPosition(j));
		int t = (CP[j] + j) % F;
		if (rtimes[t].empty()) {
			// never dies
			calive[j] = 0;
		} else {
			int i = rtimes[t].back();
			rtimes[t].pop_back();
			assert((RP[i] - j) % F == (CP[j] - i) % F);
			calive[j] = i;
			ralive[i] = j;
		}
	}
	for (int i = 0; i < L; i++) {
		// which j are bad? RP[i] - j == i + j - 1 => 2j = (RP[i] - i + 1)
		int v = (RP[i] - i + 1) % F;
		if (F % 2 == 0) {
			if (v & 1) {
				RP[i] = -1;
			} else {
				RP[i] = v/2;
			}
		} else {
			if (v & 1) {
				v += F;
			}
			RP[i] = v/2;
		}
	}
	for (int j = 0; j < L; j++) {
		int v = (CP[j] - j + 1) % F;
		if (F % 2 == 0) {
			if (v & 1) {
				CP[j] = -1;
			} else {
				CP[j] = v/2;
			}
		} else {
			if (v & 1) {
				v += F;
			}
			CP[j] = v/2;
		}
	}
	if (F % 2 == 0) {
		F /= 2;
	}
}

int monsters(int i, int j) {
	// This is kinda dumb, but there's no cost at 0
	if (i == 0 && j == 0) return 0;
	assert(i >= 1 || j >= 1);
	int res = 0;
	if (ralive[i] <= j && RP[i] == j % F) {
		res++;
	}
	if (calive[j] <= i && CP[j] == i % F) {
		res ++;
	}
	return res;
}

int main() {
	assert(L >= 1);
	computeAlives();

	// each person computes a strip and sends it on
	int a = ID;
	const int x0 = split(L, a, NODES), x1 = split(L, a+1, NODES);
	// just kill extra nodes
	if (x0 == x1) return 0;
	assert(x0 < x1);

	if (x1 < L) assert(ID + 1 < NODES);

	const int INF = L * L * 2 + 1;
	for (int i = 0; i < x1 - x0; i++) {
		cost[i] = INF;
	}
	if (x0 == 0) cost[0] = 0;
	if (a > 0) assert(x0 > 0);
	for (int b = 0; b < NODES; b++) {
		int y0 = split(L, b, NODES), y1 = split(L, b+1, NODES);
		if (y0 == y1) continue;
		assert(y0 < y1);
		// we're going to receive from ID-1
		if (x0 > 0) {
			assert(ID > 0);
			Receive(ID-1);
		}
		for (int j = y0; j < y1; j++) {
			if (x0 > 0) {
				cost[0] = min(cost[0], GetInt(ID-1));
			}
			cost[0] += monsters(x0, j);
			for (int i = 1; i < x1-x0; i++) {
				cost[i] = min(cost[i], cost[i-1]) + monsters(x0+i, j);
			}
			if (x1 < L) {
				PutInt(ID+1, cost[x1-x0-1]);
			}
		}
		if (x1 < L) {
			Send(ID+1);
		}
	}

	if (x1 == L) {
		cout << cost[x1-1-x0] << '\n';
	}

	return 0;
}
