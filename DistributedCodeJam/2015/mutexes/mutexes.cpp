#include "message.h"
#include "mutexes.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int INF = 1e8;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N[2] = { NumberOfOperations(0), NumberOfOperations(1) };
const int MAXN = 1e5;
int G[2][MAXN];

const int MAXV = 2e5;
bool has[2][MAXV];

const int S = (N[0] + NODES - 1) / NODES;
//const int S = 1e5;

int main() {
	for(int t = 0; t < 2; t++) {
		for(int i = 0; i < N[t]; i++) {
			G[t][i] = GetOperation(t, i);
		}
	}

	int res = INF;

	for(int i = 0; i < min(N[0], ID * S); i++) {
		has[0][abs(G[0][i])] = (G[0][i] > 0);
	}
	for(int i = ID * S; i < min(N[0], (ID + 1) * S); i++) {
		if(G[0][i] > 0) {
			memset(has[1], 0, sizeof(has[1]));
			int numsame = 0;
			for(int j = 0; j < N[1]; j++) {
				if(numsame == 0 && G[1][j] > 0) {
					if(has[0][G[1][j]] && has[1][G[0][i]]) {
						//cerr << i << ' ' << j << '\n';
						//for(int k = 0; k <= i; k++) cerr << G[0][k] << ' '; cerr << '\n';
						//for(int k = 0; k <= j; k++) cerr << G[1][k] << ' '; cerr << '\n';
						res = min(res, i + j + 2);
						break;
					}
				}

				has[1][abs(G[1][j])] = (G[1][j] > 0);
				if(has[0][abs(G[1][j])]) {
					numsame += (G[1][j] > 0) ? 1 : -1;
				}
			}
		}
		has[0][abs(G[0][i])] = (G[0][i] > 0);
	}

	if(ID) {
		PutInt(0, res);
		Send(0);
	} else {
		for(int i = 1; i < NODES; i++) {
			int source = Receive(-1);
			res = min(res, GetInt(source));
		}
		if(res == INF) {
			printf("OK\n");
		} else {
			printf("%d\n", res);
		}
	}


	return 0;
}
