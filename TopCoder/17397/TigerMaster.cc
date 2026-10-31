#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class TigerMaster {

public:

	vector <int> train(int N, int M, vector <int> X, vector <int> Y, vector <int> D) {
		std::vector<std::pair<int, int>> edges(M);
		for (int e = 0; e < M; e++) {
			edges[e] = {D[e], e};
		}
		std::sort(edges.begin(), edges.end());
		std::reverse(edges.begin(), edges.end());

		std::vector<std::array<int, 21>> prv(N);
		for (int i = 0; i < N; i++) {
			prv[i].fill(-1);
			prv[i][0] = 0;
		}

		std::vector<int> dist(N, 0);
		int best = -1;
		for (int z = 0; z < M; z++) {
			int e = edges[z].second;
			int x = X[e], y = Y[e];
			int dx = dist[x];
			int dy = dist[y];
			while (dist[y] < dx+1) {
				dist[y]++;
				prv[y][dist[y]] = e;
			}
			while (dist[x] < dy+1) {
				dist[x]++;
				prv[x][dist[x]] = e;
			}
			if (dist[x] == 20) {
				best = x;
				break;
			}
			if (dist[y] == 20) {
				best = y;
				break;
			}
		}
		std::vector<int> res; res.reserve(21);
		res.push_back(best);
		for (int l = 20; l > 0; l--) {
			int e = prv[best][l];
			res.push_back(e);
			best ^= X[e] ^ Y[e];
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TigerMaster;

// vim:ft=cpp
