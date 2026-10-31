#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
const int MAXM = 2.1e5;
int N, M;
vector<pair<int, int>> adj[MAXN];
int dist[MAXN];
pair<int, pair<int, int>> bestDir[MAXN];
int val[MAXN];

int main() {
	if (fopen("ideal.in", "r")) {
		freopen("ideal.in", "r", stdin);
		freopen("ideal.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int a, b, c; cin >> a >> b >> c;
		adj[a].emplace_back(b, c);
		adj[b].emplace_back(a, c);
	}
	vector<int> q;
	dist[N] = 1;
	q.push_back(N);
	while (!q.empty()) {
		vector<int> nq;
		for (int cur : q) {
			for (auto it : adj[cur]) {
				int nxt = it.first;
				if (dist[nxt] == 0) {
					nq.push_back(nxt);
					dist[nxt] = dist[cur] + 1;
					bestDir[nxt] = pair<int, pair<int, int>>(it.second, pair<int, int>(val[cur], cur));
				} else if (dist[nxt] == dist[cur] + 1) {
					bestDir[nxt] = min(bestDir[nxt], pair<int, pair<int, int>>(it.second, pair<int, int>(val[cur], cur)));
				} else continue;
			}
		}
		sort(nq.begin(), nq.end(), [&](int i, int j) -> bool {
			return tie(bestDir[i], i) < tie(bestDir[j], j);
		});
		for (int i = 0; i < int(nq.size()); i++) {
			val[nq[i]] = i;
		}
		q = nq;
	}

	vector<int> res;
	for (int cur = 1; cur != N; cur = bestDir[cur].second.second) {
		res.push_back(bestDir[cur].first);
	}
	cout << res.size() << '\n';
	for (int i = 0; i < int(res.size()); i++) {
		cout << res[i] << " \n"[i+1 == int(res.size())];
	}
	return 0;
}
