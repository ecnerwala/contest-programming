#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lemming.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

int R = int(GetRows());
int C = int(GetColumns());

int split(int a, int i, int n) {
	return (a / n) * i + min(a % n, i);
}

const int MAXR = 4e4;
const int MAXC = 400;
int c0, c1;
int S;
int T;
int dp[MAXC * MAXR];

int res = 0;

int convert(int a) {
	if (a == -1) {
		return -1;
	}
	assert(0 <= a && a < R * C);
	if (a / R == c0-1) {
		return (2*ID-1) * R + a%R;
	} else if (a / R == c1) {
		return (2*ID+2) * R + a%R;
	} else assert(false);
}

int dfs(int cur) {
	int v = dp[cur-S];
	if (v == -1) {
		return -1;
	} else if (v == -2) {
		res++;
		return -1;
	} else if (v < S || v >= T) {
		return v;
	} else {
		dp[cur-S] = -2;
		return dp[cur-S] = dfs(v);
	}
}

int main() {
	c0 = split(C, ID, NODES);
	c1 = split(C, ID+1, NODES);
	S = c0 * R;
	T = c1 * R;
	for (int i = c0; i < c1; i++) {
		for (int j = 0; j < R; j++) {
			int cur = i * R + j;
			int nxt = -1;
			char dir = GetDirection(j, i);
			if (dir == '>') {
				if (i+1 < C) nxt = (i+1) * R + j;
			} else if (dir == '<') {
				if (i-1 >= 0) nxt = (i-1) * R + j;
			} else if (dir == '^') {
				if (j-1 >= 0) nxt = i * R + (j-1);
			} else if (dir == 'v') {
				if (j+1 < R) nxt = i * R + (j+1);
			} else assert(false);

			if (nxt == -1) res ++;

			dp[cur-S] = nxt;
		}
	}
	for (int i = S; i < T; i++) {
		dfs(i);
	}
	PutInt(0, res);
	if (c1 - c0 >= 1) {
		for (int j = 0; j < R; j++) {
			PutInt(0, convert(dp[(c0 - c0) * R + j]));
		}
		for (int j = 0; j < R; j++) {
			PutInt(0, convert(dp[(c1-1-c0) * R + j]));
		}
	} else {
		// Always goes to the right
		if (ID == NODES-1) {
			for (int j = 0; j < R; j++) {
				PutInt(0, -1);
			}
		} else {
			for (int j = 0; j < R; j++) {
				PutInt(0, (2*ID+2)*R + j);
			}
		}
		if (ID == 0) {
			for (int j = 0; j < R; j++) {
				PutInt(0, -1);
			}
		} else {
			for (int j = 0; j < R; j++) {
				PutInt(0, (2*ID-1)*R + j);
			}
		}
	}
	Send(0);

	if (ID == 0) {
		res = 0;
		for (int n = 0; n < NODES; n++) {
			Receive(n);
			res += GetInt(n);
			for (int j = 0; j < 2 * R; j++) {
				dp[n * 2 * R + j] = GetInt(n);
			}
		}
		S = 0, T = 2 * NODES * R;
		for (int i = S; i < T; i++) {
			dfs(i);
		}
		cout << res << '\n';
	}

	return 0;
}
