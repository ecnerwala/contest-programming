#include "message.h"
#include "johnny.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = NumberOfCards();

const int B = (N + NODES - 1) / NODES;
const int S = max(0, ID * B);
const int T = min(N, (ID + 1) * B);

const int MAXN = 4e4;

int cnt[MAXN]; // number that you beat

int val[MAXN];

int maxval[MAXN];

int main() {
	for(int i = S; i < T; i++) {
		for(int j = 0; j < N; j++) {
			if(IsBetter(i, j)) {
				cnt[i] ++;
			}
		}
	}
	if(ID) {
		for(int i = S; i < T; i++) {
			PutInt(0, cnt[i]);
		}
		Send(0);

		Receive(0);
		for(int i = 0; i < N; i++) {
			cnt[i] = GetInt(0);
		}
	} else {
		for(int b = 1; b < NODES; b++) {
			int so = Receive(-1);
			for(int i = max(0, so * B); i < min(N, (so + 1) * B); i++) {
				cnt[i] = GetInt(so);
			}
		}

		for(int b = 1; b < NODES; b++) {
			for(int i = 0; i < N; i++) {
				PutInt(b, cnt[i]);
			}
			Send(b);
		}
	}

	for(int i = S; i < T; i++) {
		val[i] = cnt[i];
		for(int j = 0; j < N; j++) {
			if(IsBetter(i, j)) {
				val[i] = max(val[i], cnt[j]);
			}
		}
	}

	if(ID) {
		for(int i = S; i < T; i++) {
			PutInt(0, val[i]);
		}
		Send(0);
	} else {
		for(int b = 1; b < NODES; b++) {
			int so = Receive(-1);
			for(int i = max(0, so * B); i < min(N, (so + 1) * B); i++) {
				val[i] = GetInt(so);
			}
		}

		//for(int i = 0; i < N; i++) cerr << cnt[i] << ' ' << val[i] << '\n';

		memset(maxval, -1, sizeof(maxval));

		for(int i = 0; i < N; i++) {
			assert(val[i] >= cnt[i]);
			maxval[cnt[i]] = max(maxval[cnt[i]], val[i]);
		}

		int s = 0;
		while(maxval[s] == -1) s++;
		for(int i = s; i <= s; i++) {
			s = max(s, maxval[i]);
		}
		int res = 0;
		for(int i = 0; i < N; i++) {
			if(cnt[i] > s) res++;
		}
		if(res == 0) {
			printf("IMPOSSIBLE\n");
		} else {
			printf("%d\n", res);
		}
	}
	return 0;
}
