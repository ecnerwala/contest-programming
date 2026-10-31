#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1100;
int N;
char W[MAXN][20];
bool vis[MAXN][20][MAXN][20];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> W[i];
	}
	int ans = 0;
	vector<pair<pair<int, int>, pair<int, int>>> q;
	for (int w1 = 0; w1 < N; w1++) {
		for (int w2 = w1 + 1; w2 < N; w2++) {
			q.push_back({{w1,0},{w2,0}});
		}
	}
	while (!q.empty()) {
		ans++;
		vector<pair<pair<int, int>, pair<int, int>>> nq;
		for (int z = 0; z < int(q.size()); z++) {
			auto state = q[z];
			int w1 = state.first.first, i1 = state.first.second;
			int w2 = state.second.first, i2 = state.second.second;
			if (w1 == N) {
				assert(i1 == 0);
				for (int w = 0; w < N; w++) {
					q.push_back({{w,0},{w2,i2}});
				}
				continue;
			}
			if (w2 == N) {
				assert(i2 == 0);
				for (int w = 0; w < N; w++) {
					q.push_back({{w1,i1},{w,0}});
				}
				continue;
			}
			assert(W[w1][i1] != '\0');
			assert(W[w2][i2] != '\0');
			if (W[w1][i1] != W[w2][i2]) {
				continue;
			}
			i1++, i2++;
			if (W[w1][i1] == '\0') w1 = N, i1 = 0;
			if (W[w2][i2] == '\0') w2 = N, i2 = 0;
			if (w1 == N && w2 == N) {
				cout << ans << '\n';
				exit(0);
			}
			if (vis[w1][i1][w2][i2]) {
				continue;
			}
			vis[w1][i1][w2][i2] = true;
			nq.push_back({{w1,i1},{w2,i2}});
		}
		q = std::move(nq);
	}
	cout << 0 << '\n';

	return 0;
}
