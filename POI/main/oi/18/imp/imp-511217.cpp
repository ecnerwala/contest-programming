#include<iostream>
#include<cstring>
#include<cassert>

using namespace std;

const int MAXN = 6000;

int N,M;
bool adj[MAXN][MAXN];

bool in[MAXN];

void go() {
	memset(in, true, sizeof(in));

	for(int i = 0; i < N; i++) {
		if(!in[i]) continue;
		for(int j = i + 1; j < N; j++) {
			if(!in[j]) continue;

			if(!adj[i][j]) {
				in[i] = in[j] = false;
				break;
			}
		}
	}
}

int main( ){
	ios_base::sync_with_stdio(0);

	memset(adj, 0, sizeof(adj));

	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;

		adj[a][b] = true;
		adj[b][a] = true;
	}

	go();

	int v = 0;
	for(int i = 0; i < N / 3; i++) {
		while(!in[v]) {
			v++;
		}
		assert(v < N);

		cout << (v + 1) << '\n';

		v++;
	}

	return 0;
}
