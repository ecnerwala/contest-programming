#include "game.h"
#include <bits/stdc++.h>

namespace ecnerwala { namespace {

int N;
int K;
std::vector<std::vector<int>> outEdges;
std::vector<std::vector<int>> inEdges;

std::vector<std::array<int, 2>> cur_range;

void init(int N_, int K_) {
	N = N_ + 1;
	K = K_ + 1;
	outEdges.resize(N);
	inEdges.resize(N);

	cur_range.resize(N);
	for (int i = 0; i < K; i++) {
		cur_range[i] = {i, i+1};
	}
	for (int i = K; i < N; i++) {
		cur_range[i] = {0, K};
	}
}

bool checkVert(int cur);

bool checkEdge(int st, int en) {
	// st must be left-ish of en

	if (cur_range[st][1] <= cur_range[en][0]) {
		// always good
		return false;
	}
	// Contradiction!
	if (cur_range[st][0] >= cur_range[en][1]) {
		return true;
	}

	if (cur_range[st] == cur_range[en]) return false;

	// st is on top, we need to keep moving it down until en is in the right half
	if ((cur_range[st][0] + cur_range[st][1]) / 2 >= cur_range[en][1]) {
		cur_range[st][1] = (cur_range[st][0] + cur_range[st][1]) / 2;
		return checkVert(st);
	} else if ((cur_range[en][0] + cur_range[en][1]) / 2 <= cur_range[st][0]) {
		cur_range[en][0] = (cur_range[en][0] + cur_range[en][1]) / 2;
		return checkVert(en);
	}
	return false;
}

bool checkVert(int cur) {
	for (int nxt : inEdges[cur]) {
		if (checkEdge(nxt, cur)) return true;
	}
	for (int nxt : outEdges[cur]) {
		if (checkEdge(cur, nxt)) return true;
	}
	return false;
}

bool add_teleporter(int u, int v) {
	u++;
	v++;
	if (v < K) v--;

	outEdges[u].push_back(v);
	inEdges[v].push_back(u);
	return checkEdge(u, v);
}

} } // end namespaces

void init(int n, int k) {
	ecnerwala::init(n, k);
}

int add_teleporter(int u, int v) {
	return ecnerwala::add_teleporter(u, v) ? 1 : 0;
}
