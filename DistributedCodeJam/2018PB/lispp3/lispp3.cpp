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

int split (int a, int i, int n) {
	assert(0 <= i && i <= n);
	return (a / n) * i + min(a % n, i);
}

const int L = GetLength();
int splits[MAXNODES+1];
int height[MAXNODES+1];
int minHeight[MAXNODES+1];
int minPlus[MAXNODES+1];

const int MAXSPLIT = 1.1e7;

int res = L;

char plusses[MAXSPLIT];

struct simulator {
	int baseHeight;
	int maxHeight;
	int curHeight;

	simulator (int base, int ceil, int cur) {
		baseHeight = base;
		maxHeight = ceil;
		curHeight = cur;
		assert(ceil >= base);
		memset(plusses, -1, size_t(ceil - base + 1));
	}

	// Returns true if good
	bool processChar(int i) {
		if (res < i) {
			// We don't need to do anything
			return false;
		}
		char c = GetCharacter(i);
		if (c == ')' && i > 0 && GetCharacter(i-1) == '+') {
			res = i;
			return false;
		}
		if (c == '+' && !(i > 0 && GetCharacter(i-1) == ')')) {
			res = i;
			return false;
		}

		if (c == '+') {
			if (curHeight <= 0) {
				res = i;
				return false;
			}
			if (baseHeight <= curHeight && curHeight <= maxHeight) {
				if (plusses[curHeight - baseHeight] == 1) {
					res = i;
					return false;
				} else {
					plusses[curHeight - baseHeight] = 1;
				}
			}
		} else if (c == ')') {
			if (curHeight <= 0) {
				res = i;
				return false;
			}
			if (baseHeight <= curHeight && curHeight <= maxHeight) {
				if (i > 0 && GetCharacter(i-1) == '(') {
					// we're just good
					plusses[curHeight - baseHeight] = 1;
				}

				if (plusses[curHeight - baseHeight] == 0) {
					res = i;
					return false;
				}
				// doesn't really matter
				//plusses[curHeight - baseHeight] = -1;
			}
			curHeight --;
		} else if (c == '(') {
			if (curHeight < 0) { // strictly less
				res = i;
				return false;
			}
			curHeight ++;
			if (baseHeight <= curHeight && curHeight <= maxHeight) {
				// need a plus
				plusses[curHeight - baseHeight] = 0;
			}
		} else {
			assert(false);
		}
		return true;
	}

	void processDown(int newHeight) {
		assert(newHeight <= curHeight);
		/*
		for (int i = min(curHeight, maxHeight); i >= max(newHeight + 1, baseHeight); i--) {
			// just ignore
			plusses[i - baseHeight] = -1;
		}
		*/
		curHeight = newHeight;
	}

	void processUp(int newHeight) {
		assert(newHeight >= curHeight);
		for (int i = max(curHeight+1, baseHeight); i <= min(newHeight, maxHeight); i++) {
			// We don't care about these opens
			plusses[i - baseHeight] = -1;
		}
		curHeight = newHeight;
	}
};

int main() {
	for (int i = 0; i <= NODES; i++) {
		splits[i] = split(L, i, NODES);
	}
	{
		int h = 0;
		int minH = 0;
		int minP = -1;
		for (int i = splits[ID]; i < splits[ID+1]; i++) {
			char c = GetCharacter(i);
			if (c == ')') {
				h--;
				if (h < minH) {
					minH = h;
					minP = -1;
				}
			} else if (c == '+') {
				if (h == minH && minP == -1) {
					minP = i;
				}
			} else if (c == '(') {
				h++;
			}
		}
		for (int z = 0; z < NODES; z++) {
			int n = (ID + z) % NODES;
			PutInt(n, h);
			PutInt(n, minH);
			PutInt(n, minP);
			Send(n);
		}
	}
	for (int z = 0; z < NODES; z++) {
		int n = (ID + NODES - z) % NODES;
		Receive(n);
		height[n+1] = GetInt(n);
		minHeight[n] = GetInt(n);
		minPlus[n] = GetInt(n);
	}
	for (int n = 0; n < NODES; n++) {
		height[n+1] += height[n];
		minHeight[n] += height[n];
	}

	{ // phase 1: simulate yourself
		simulator sim(minHeight[ID], minHeight[ID] + (splits[ID+1] - splits[ID]) + 1, height[ID]);
		for (int i = splits[ID]; i < splits[ID+1]; i++) {
			if (!sim.processChar(i)) goto phase1_dead;
		}
		int h = height[ID+1];
		for (int a = ID+1; a < NODES; a++) {
			if (minHeight[a] <= h && minPlus[a] != -1) {
				sim.processDown(minHeight[a]);
				h = minHeight[a];
				if (!sim.processChar(minPlus[a])) goto phase1_dead;
			}
		}
	}
phase1_dead:;
	{ // phase 2: iterate through all jobs, find who closes each opening
		int jobCnt = 0;
		for (int l = 0; l < NODES; l++) {
			int up = height[l+1];
			int down = minHeight[l];
			for (int r = l+1; up > down && r < NODES; r++) {
				int top, bot;
				if (minHeight[r] < up) {
					// we close something out, so let's process it
					top = up, bot = max(down, minHeight[r]);
					up = bot;
				} else {
					continue;
				}

				if (jobCnt % NODES == ID) {
					simulator sim(bot, top, height[l]);
					for (int i = splits[l]; i < splits[l+1]; i++) {
						if (!sim.processChar(i)) goto job_dead;
					}
					sim.processDown(top);
					for (int a = l+1; a < r; a++) {
						if (minHeight[a] == top && minPlus[a] != -1) {
							if (!sim.processChar(minPlus[a])) goto job_dead;
						}
					}
					sim.processUp(height[r]);
					for (int i = splits[r]; i < splits[r+1]; i++) {
						if (!sim.processChar(i)) goto job_dead;
					}
				}
job_dead:;
				jobCnt ++;
			}
		}
		assert(jobCnt <= 5 * NODES);
		//cerr << jobCnt << '\n';
	}

	// final phase: print result
	if (ID != 0) {
		PutInt(0, res);
		Send(0);
	} else {
		for (int n = 1; n < NODES; n++) {
			Receive(n);
			res = min(res, GetInt(n));
		}
		if (res == L && height[NODES] == 0) {
			res = -1;
		}
		cout << res << '\n';
	}

	return 0;
}
