#include<bits/stdc++.h>
using namespace std;

const int MAXN = 60;
int N;
char G[MAXN][MAXN];
char ans[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> G[i];
	}
	for (int transpose = 0; transpose < 2; transpose ++) {
		for (int fx = 0; fx < 2; fx++) {
			for (int fy = 0; fy < 2; fy++) {
				for (int dx = -N; dx <= 2 * N; dx++) {
					for (int dy = -N; dy <= 2 * N; dy++) {
						using pt = pair<int, int>;
						auto isGood = [&](pt a) {
							return 0 <= a.first && a.first < N && 0 <= a.second && a.second < N && G[a.first][a.second] == '#';
						};
						auto nxt = [&](pt a) {
							if (transpose) swap(a.first, a.second);
							if (fx) a.first *= -1;
							if (fy) a.second *= -1;
							a.first += dx;
							a.second += dy;
							return a;
						};
						auto prv = [&](pt a) {
							a.second -= dy;
							a.first -= dx;
							if (fy) a.second *= -1;
							if (fx) a.first *= -1;
							if (transpose) swap(a.first, a.second);
							return a;
						};
						for (int i = 0; i < N; i++) {
							for (int j = 0; j < N; j++) {
								ans[i][j] = G[i][j];
							}
						}
						for (int i = 0; i < N; i++) {
							for (int j = 0; j < N; j++) {
								if (ans[i][j] != '#') continue;
								pt start(i, j);
								deque<pt> path;
								pt cur = start;
								do {
									path.push_back(cur);
									cur = nxt(cur);
								} while (cur != start && isGood(cur));
								if (cur != start) {
									cur = prv(start);
									while (isGood(cur)) {
										path.push_front(cur);
										cur = prv(cur);
									}
								}
								if (path.size() & 1) {
									goto isBad;
								}
								for (int z = 0; !path.empty(); z++) {
									ans[path.front().first][path.front().second] = char('A' + (z & 1));
									path.pop_front();
								}
							}
						}
						cout << "YES" << '\n';
						for (int i = 0; i < N; i++) {
							for (int j = 0; j < N; j++) {
								cout << ans[i][j];
							}
							cout << '\n';
						}
						exit(0);
isBad:;
					}
				}
			}
		}
	}
	cout << "NO" << '\n';

	return 0;
}
