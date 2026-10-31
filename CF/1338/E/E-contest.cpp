#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx2")  //Enable AVX

#include<bits/stdc++.h>
using namespace std;

auto c2 = [](int64_t a) -> int64_t { assert(a >= 0); return a * (a-1) / 2; };

using bs = bitset<8000>;

int64_t solveScc(int N, vector<vector<bool>> adj) {
	if (N == 0) {
		return 0;
	}
	assert(N >= 3);
	for (int i = 0; i < N; i++) {
		adj[i][i] = true;
	}

	vector<bs> nxt(N);
	vector<bs> prv(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			nxt[i][j] = adj[i][j];
			prv[i][j] = adj[j][i];
		}
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!adj[i][j]) {
				if ((nxt[i] & prv[j]).any()) {
					ans ++;
				}
			}
		}
	}

	return 2 * N * (N-1) - ans;
}

void test() {
	const int N = 8000;
	vector<vector<bool>> adj(N, vector<bool>(N));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			adj[i][j] = (i < j);
		}
	}
	cerr << solveScc(N, adj) << '\n';
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	//test();
	int N; scanf("%d", &N);
	vector<vector<bool>> adj(N, vector<bool>(N));
	{
		char* s = new char[N/4 + 10];
		for (int i = 0; i < N; i++) {
			scanf("%s", s);
			for (int j = 0; j < N/4; j++) {
				char c = s[j];
				assert(('0' <= c && c <= '9') || ('A' <= c && c <= 'F'));
				int v = ('0' <= c && c <= '9') ? (c - '0') : (10 + (c - 'A'));
				assert(0 <= v && v < 16);
				for (int z = 0; z < 4; z++) {
					adj[i][4*j+z] = bool(v & (1<<(3-z)));
				}
			}
		}
		delete[] s;
	}

	//for (int i = 0; i < N; i++) { for (int j = 0; j < N; j++) { cerr << adj[i][j]; } cerr << '\n'; } cerr << '\n';

	const int UNREACHABLE = 614 * N;

	vector<int> inDeg(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (adj[i][j]) inDeg[j]++;
		}
	}

	vector<int> dead; dead.reserve(N);
	for (int i = 0; i < N; i++) {
		if (inDeg[i] == 0) {
			dead.push_back(i);
		}
	}

	for (int z = 0; z < int(dead.size()); z++) {
		int cur = dead[z];
		for (int v = 0; v < N; v++) {
			if (adj[cur][v]) {
				assert(inDeg[v] > 0);
				inDeg[v]--;
				if (inDeg[v] == 0) {
					dead.push_back(v);
				}
			}
		}
	}

	int64_t ans = (UNREACHABLE + 1) * (c2(N) - c2(N - int(dead.size())));

	mt19937 mt(48);
	{ // relabel
		int newN = N - int(dead.size());
		vector<int> aliveInds; aliveInds.reserve(newN);
		for (int i = 0; i < N; i++) {
			if (inDeg[i] > 0) {
				aliveInds.push_back(i);
			}
		}
		assert(int(aliveInds.size()) == newN);
		shuffle(aliveInds.begin(), aliveInds.end(), mt);
		vector<vector<bool>> nxtAdj(newN, vector<bool>(newN));
		for (int i = 0; i < newN; i++) {
			for (int j = 0; j < newN; j++) {
				nxtAdj[i][j] = adj[aliveInds[i]][aliveInds[j]];
			}
		}

		N = newN;
		adj = nxtAdj;
	}

	//for (int i = 0; i < N; i++) { for (int j = 0; j < N; j++) { cerr << adj[i][j]; } cerr << '\n'; } cerr << '\n';

	ans += solveScc(N, adj);

	printf("%" PRId64 "\n", ans);

	return 0;
}
