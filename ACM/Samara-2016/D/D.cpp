#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

using loc = pair<int, int>;
using state = pair<loc, int>;

const int MAXN = 60;
const int MAXM = 60;
const int MAXW = 60;
int N, M;
loc CENTER;

int cost[MAXN][MAXM];
int minDist[MAXN][MAXM][MAXW];
state prv[MAXN][MAXM][MAXW];
bool used[MAXN][MAXN];

bool valid(loc l) {
	return 0 <= l.first && l.first < N && 0 <= l.second && l.second < M;
}

template <typename T> T& get(T a[MAXN][MAXM][MAXW], state s) {
	return a[s.first.first][s.first.second][s.second];
}

int ijk(int start_c) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			used[i][j] = false;
			for (int w = 0; w < MAXW; w++) {
				minDist[i][j][w] = INF;
			}
		}
	}
	assert(0 <= start_c && start_c < CENTER.second);
	state START(loc(CENTER.first, start_c), 0);
	priority_queue<pair<int, state>> pq;
	get(prv, START) = START; // whatever
	pq.emplace(-(get(minDist, START) = 0), START);
	while (!pq.empty()) {
		int d = -pq.top().first;
		const state s = pq.top().second;
		const loc& l = s.first;
		pq.pop();
		if (get(minDist, s) != d) continue;
		if (s.first == START.first && s.second > 0) {
			for (state cur = s; cur != START; cur = get(prv, cur)) {
				used[cur.first.first][cur.first.second] = true;
			}
			// do stuff
			return d;
		}
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) continue;
				state ns = s;
				loc& nl = ns.first;
				nl.first += dx;
				nl.second += dy;
				if (!valid(nl)) continue;
				if (nl == CENTER) continue;
				if (max(l.second, nl.second) <= CENTER.second) {
					if (l.first == CENTER.first && nl.first == CENTER.first - 1) {
						ns.second ++;
					}
					if (l.first == CENTER.first - 1 && nl.first == CENTER.first) {
						ns.second --;
					}
				}
				if (ns.second < 0) continue;
				if (ns.second >= MAXW) continue;
				int nd = d + cost[nl.first][nl.second];
				if (nd < get(minDist, ns)) {
					get(prv, ns) = s;
					pq.emplace(-(get(minDist, ns) = nd) , ns);
				}
			}
		}
	}
	// there should always be a path
	assert(false);
}

int main() {
	ios_base::sync_with_stdio(0);
	int R, C;
	cin >> N >> M >> R >> C; R--, C--;
	CENTER = loc(R, C);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> cost[i][j];
		}
	}

	assert(CENTER.second > 0);
	int bestC = 0;
	int bestDist = ijk(0);
	for (int c = 1; c < CENTER.second; c++) {
		int cnd = ijk(c);
		if (cnd < bestDist) {
			bestDist = cnd;
			bestC = c;
		}
	}

	cout << ijk(bestC) << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << ".X"[used[i][j]];
		}
		cout << '\n';
	}

	return 0;
}
