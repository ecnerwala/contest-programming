#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "median.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = GetN();

template <typename T> T split(T n, T i, T tot) {
	return n / tot * i + min(i, n % tot);
}

const int INF = int(1e9) + 200;

int go_small() {
	assert(N % 2 == 1);
	int st = split(N, ID, NODES), en = split(N, ID+1, NODES);
	int mi = -1, ma = INF;
	while (ma - mi > 1) {
		const int B = 1500;
		int mds[1505];
		for (int k = 0; k <= B; k++) {
			mds[k] = (ma - mi) / B * k + mi;
		}
		assert(mds[0] == mi);
		assert(mds[B] == ma);
		int cnts[1505];
		memset(cnts, 0, sizeof(cnts));
		for (int i = st; i < en; i++) {
			int v = GetData(i);
			if (v < mi) continue;
			int ind;
			if (v >= ma) {
				ind = B;
			} else {
				ind = (v - mi) / ((ma - mi) / B);
			}
			cnts[ind]++;
		}
		for (int k = 0; k < B; k++) {
			cnts[k+1] += cnts[k];
		}
		for (int j = 0; j < NODES; j++) {
			if (j == ID) continue;
			for (int k = 0; k <= B; k++) {
				PutInt(j, cnts[k]);
			}
			Send(j);
		}
		for (int j = 0; j < NODES; j++) {
			if (j == ID) continue;
			Receive(j);
			for (int k = 0; k <= B; k++) {
				cnts[k] += GetInt(j);
			}
		}
		//cerr << mi << ' ' << ma << '\n';
		//cerr << cnts[0] << ' ' << cnts[B] << '\n';
		assert(cnts[0] * 2 < N);
		assert(cnts[B] * 2 > N);
		for (int k = 0; k <= B; k++) {
			if (cnts[k] * 2 > N) {
				ma = min(ma, mds[k]);
			} else {
				mi = max(mi, mds[k]);
			}
		}
	}
	return ma;
}

int main() {
	int res = go_small();
	if (ID == 0) {
		cout << res << '\n';
	}
	return 0;
}
