#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e3;
const int N = 1e3;

struct pt {
	int x, y, z;
	int r;
};

pt pts[MAXN];

int main() {
	for (int i = 0; i < N; i++) {
		int v = scanf("pos=<%d,%d,%d>, r=%d\n", &pts[i].x, &pts[i].y, &pts[i].z, &pts[i].r);
		assert(v == 4);
	}

	int res = 0;
	int bestSum = 0;
	// the 4 planes are x+y+z, x+y-z, x-y+z, x-y-z
	// any optimum lies on the intersection of 3, so it either includes the first 2 or the last 2
	// this just corresponds to negating y
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < N; i++) {
			for (int zi = 0; zi < 2; zi++) {
				int ival = pts[i].x + pts[i].y + pts[i].z + (zi ? -pts[i].r : +pts[i].r);
				for (int j = 0; j < N; j++) {
					for (int zj = 0; zj < 2; zj++) {
						int jval = pts[j].x + pts[j].y - pts[j].z + (zj ? -pts[j].r : +pts[j].r);

						// we're intersecting these two planes, so we get a fixed z, and a fixed x+y
						// these may be half-integers, so we store the doubled ones
						int z2 = ival - jval;
						int xy2 = ival + jval;

						vector<pair<int, int>> locs;
						for (int k = 0; k < N; k++) {
							int xyRadius2 = 2 * pts[k].r - abs(2 * pts[k].z - z2);
							if (xyRadius2 < 0) continue;

							// radius of the x and y is max abs difference of x+y and x-y, we know x+y so just check that
							int xyDist2 = abs(2 * (pts[k].x + pts[k].y) - xy2); // double the x+y dist
							if (xyDist2 > xyRadius2) continue;

							int diffCenter2 = 2 * (pts[k].x - pts[k].y);
							locs.emplace_back(diffCenter2 - xyRadius2, -1);
							locs.emplace_back(diffCenter2 + xyRadius2, +1);
						}
						sort(locs.begin(), locs.end());
						int cnt = 0;
						int lastDiff = -1;
						for (const auto& it : locs) {
							int diff2 = it.first;
							int coordSum = abs(z2);

							if (lastDiff <= 0 && diff2 >= 0) {
								coordSum += abs(xy2);
							} else {
								coordSum += max(abs(xy2), abs(diff2));
							}
							lastDiff = diff2;

							if (cnt > res || (cnt == res && coordSum < bestSum)) {
								res = cnt;
								bestSum = coordSum;
								//cerr << res << ' ' << bestSum << '\n';
							}

							cnt -= it.second;
							assert(cnt >= 0);

							if (cnt > res || (cnt == res && coordSum < bestSum)) {
								res = cnt;
								bestSum = coordSum;
								//cerr << res << ' ' << bestSum << '\n';
							}
						}
						assert(cnt == 0);
					}
				}
			}
			cerr << i << ' ' << res << ' ' << bestSum << '\n';
		}

		for (int i = 0; i < N; i++) {
			pts[i].y *= -1;
		}
	}
	cerr << res << ' ' << bestSum << '\n';
	cout << bestSum / 2;
	if (bestSum % 2) cout << ".5";
	cout << '\n';
	assert(bestSum % 2 == 0);

	return 0;
}
