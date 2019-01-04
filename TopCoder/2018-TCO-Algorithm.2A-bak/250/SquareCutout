#include <bits/stdc++.h>
using namespace std;

class SquareCutout {

public:

	int verify(vector <string> cutout) {
		int N = int(cutout.size());
		assert(N > 0);
		int M = int(cutout[0].size());
		assert(M > 0);
		bool seen = false;
		int minx = N;
		int maxx = -1;
		int miny = M;
		int maxy = -1;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (cutout[i][j] == '#') {
					seen = true;
					minx = min(minx, i);
					maxx = max(maxx, i);
					miny = min(miny, j);
					maxy = max(maxy, j);
				}
			}
		}
		if (!seen) {
			return true;
		}
		assert(minx <= maxx);
		assert(miny <= maxy);
		return false;
	}
};

// vim:ft=cpp
