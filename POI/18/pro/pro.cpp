#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<numeric>

using namespace std;

const int MAXN = 501, MAXM = 501, MAXV = MAXN + MAXN;

int N, M;
int R, T;
int K;

int numSolves[MAXN];
vector<int> adj[MAXN];
int solver[MAXM];

int Adist[MAXN];
int Bdist[MAXN];

//see if we can augment onto this person
bool dfs(int cur, int dist) {
	for (int i : adj[cur]) {
		if (Bdist[i] != dist) continue;

		// Outdegree == 1, so we can mark it now
		Bdist[i] = -1;

		int nxt = solver[i];
		if (nxt == -1 || (Adist[nxt] == dist+1 && dfs(nxt, dist+1))) {
			solver[i] = cur;
			return true;
		}
	}

	Adist[cur] = -1;
	return false;
}

int flow = 0;
long long cost = 0;

void go() {
	//memset(numSolves, 0, sizeof(numSolves)) // global
	//memset(vis, 0, sizeof(vis)); // global
	memset(solver, -1, sizeof(solver));
	T /= R;
	T = min(T, M);
	std::vector<int> q(N); std::iota(q.begin(), q.end(), 0);
	std::vector<int> nq; nq.reserve(N);
	std::vector<int> q2; q2.reserve(N);
	while (T--) {
		if (q.empty()) break;

		nq.clear();
		while (true) {
			// Run bfs
			for (int i = 0; i < N; i++) Adist[i] = -1;
			for (int j = 0; j < M; j++) Bdist[j] = -1;
			q2.clear();
			for (int i : q) {
				q2.push_back(i);
				Adist[i] = 0;
			}
			int sink_dist = -1;
			for (int z = 0; z < int(q2.size()); z++) {
				int i = q2[z];
				int d = Adist[i];
				if (d == sink_dist) break;
				for (int j : adj[i]) {
					if (solver[j] == i) continue;
					if (Bdist[j] != -1) continue;
					Bdist[j] = d;
					int ni = solver[j];
					if (ni == -1) {
						sink_dist = d+1;
					} else if (Adist[ni] == -1) {
						Adist[ni] = d+1;
						q2.push_back(ni);
					}
				}
			}
			if (sink_dist == -1) break;

			// Blocking flow
			q2.clear();
			for (int i : q) {
				if (dfs(i, 0)) {
					flow++;
					numSolves[i]++;
					cost += numSolves[i];
					nq.push_back(i);
				} else {
					q2.push_back(i);
				}
			}
			swap(q, q2);
		}
		swap(q, nq);
	}
	cost *= R;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M >> R >> T >> K;

	//memset(adj, 0, sizeof(adj)) // global
	for(int i = 0; i < K; i++) {
		int a, b; 
		cin >> a >> b;
		a--, b--;
		adj[a].push_back(b);
	}

	go();

	cout << flow << ' ' << cost << '\n';

	for(int i = 0; i < M; i++) {
		int p = solver[i];
		if(p >= 0) {
			numSolves[p] --;
			cout << (p + 1) << ' ' << (i + 1) << ' ' << (numSolves[p] * R) << '\n';
		}
	}

	return 0;
}
