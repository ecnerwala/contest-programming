#include<bits/stdc++.h>
using namespace std;

const int MAXV = 1.1e5;
const int V = 99999;
int bucketNum[32][MAXV];
int zeroCost[MAXV];

const int MAXC = 80;
const int C = 45;

pair<int, int> interval[MAXC][MAXV];

int bucketVal[MAXV];
vector<pair<int, int>> evts[MAXV];

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	for (int v = 1; v <= V; v++) {
		for (int i = 0, p10 = 1; i < 5; i++, p10 *= 10) {
			if (v / p10 % 10) zeroCost[v] ++;
		}

		for (int m = 0; m < 32; m++) {
			for (int i = 0, p10 = 1; i < 5; i++, p10 *= 10) {
				if (!(m & (1<<i))) {
					bucketNum[m][v] += v / p10 % 10 * p10;
				}
			}
		}
		assert(bucketNum[0][v] == v);
		assert(bucketNum[31][v] == 0);
	}

	for (int c = 0; c <= C; c++) {
		for (int i = 1; i <= V; i++) {
			interval[c][i] = {max(0, i-2), min(i+2, V+1)};
		}
	}

	for (int cost = 0; cost < C; cost++) {
		for (int m = 1; m < 32; m++) {
			int ncost = cost + 1 + __builtin_popcount(m);

			{
				for (int buck = 0; buck <= V; buck++) {
					bucketVal[buck] = 0;
				}
				for (int i = 0; i <= V+1; i++) {
					evts[i] = {};
				}

				for (int i = 1; i <= V; i++) {
					evts[interval[cost][i].first].emplace_back(bucketNum[m][i], interval[cost][i].second);
				}

				for (int i = 0; i <= V+1; i++) {
					for (auto e : evts[i]) {
						setmax(bucketVal[e.first], e.second);
					}
					if (1 <= i && i <= V) {
						setmax(interval[ncost][i].second, bucketVal[bucketNum[m][i]]);
					}
				}
			}
			{
				for (int buck = 0; buck <= V; buck++) {
					bucketVal[buck] = V+1;
				}
				for (int i = 0; i <= V+1; i++) {
					evts[i] = {};
				}
				for (int i = 1; i <= V; i++) {
					evts[interval[cost][i].second].emplace_back(bucketNum[m][i], interval[cost][i].first);
				}

				for (int i = V+1; i >= 0; i--) {
					for (auto e : evts[i]) {
						setmin(bucketVal[e.first], e.second);
					}
					if (1 <= i && i <= V) {
						setmin(interval[ncost][i].first, bucketVal[bucketNum[m][i]]);
					}
				}
			}
		}
	}
	for (int i = 1; i <= V; i++) {
		assert(interval[C][i].first == 0);
		assert(interval[C][i].second == V+1);
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int L, R; cin >> L >> R;
		int ans = C;
		for (int i = L; i <= R; i++) {
			for (int c = 0; c < ans; c++) {
				if (interval[c][i].first < L && R < interval[c][i].second) {
					setmin(ans, c + 1 + zeroCost[i]);
					break;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
