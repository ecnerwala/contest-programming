#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "baby_blocks.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = int(GetNumberOfBlocks());

template <typename T> T split(T n, T i, T tot) {
	return n / tot * i + min(i, n % tot);
}

const int MAXNODES = 200;
ll sums[MAXNODES];

// compare the two ranges and look for things
ll compare_ranges(int a, int b) {
	int sa = split(N, a, NODES), ea = split(N, a+1, NODES);
	int sb = split(N, b, NODES), eb = split(N, b+1, NODES);
	if (sb == eb) return 0;
	if (sa == ea) return 0;
	int ca = sa, cb = eb-1;
	ll cur_val = sums[a] + sums[b+1] - GetBlockWeight(eb-1);
	ll cnt = 0;
	while (true) {
		if (cur_val == sums[NODES]) {
			cnt++;

			cur_val += GetBlockWeight(ca);
			ca++;
			cb--;
			if (ca >= ea) break;
			if (cb < sb) break;
			cur_val -= GetBlockWeight(cb);
		} else if(cur_val < sums[NODES]) {
			cur_val += GetBlockWeight(ca);
			ca++;
			if (ca >= ea) break;
		} else if (cur_val > sums[NODES]) {
			cb--;
			if (cb < sb) break;
			cur_val -= GetBlockWeight(cb);
		} else assert(false);
	}
	return cnt;
}

int main() {
	int st = split(N, ID, NODES), en = split(N, ID+1, NODES);
	for (int i = st; i < en; i++) {
		sums[ID+1] += GetBlockWeight(i);
	}
	for (int i = 0; i < NODES; i++) {
		PutLL(i, sums[ID+1]);
		Send(i);
	}
	sums[0] = 0;
	for (int i = 0; i < NODES; i++) {
		Receive(i);
		sums[i+1] = GetLL(i);
	}
	for (int i = 1; i <= NODES; i++) {
		sums[i] += sums[i-1];
	}
	ll cnt = 0;

	int ind = 0;
	for (int i = 0; i < NODES; i++) {
		for (int j = 0; j < NODES; j++) {
			if (sums[i] + sums[j] <= sums[NODES] && sums[i+1] + sums[j+1] > sums[NODES]) {
				if (ind % NODES == ID) {
					cnt += compare_ranges(i, j);
				}
				ind++;
			}
		}
	}
	assert(ind <= 4 * NODES);

	if (ID > 0) {
		PutLL(0, cnt);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			cnt += GetLL(i);
		}
		cout << (cnt + 1) / 2 << '\n';
	}

	return 0;
}
