#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN];

#ifdef WALA
const bool TEST = false;
#else
const bool TEST = false;
#endif

namespace test {
int ans;
int dist[MAXN];
void init_ans() {
	cin >> ans; ans--;

	memset(dist, -1, sizeof(dist));
	queue<int> q;
	dist[ans] = 0;
	q.push(ans);

	while (!q.empty()) {
		int cur = q.front(); q.pop();
		for (int nxt : adj[cur]) {
			if (dist[nxt] == -1) {
				dist[nxt] = dist[cur] + 1;
				q.push(nxt);
			}
		}
	}
}

bool query(int x, vector<int> vs) {
	for (int v : vs) {
		if (dist[v] < dist[x]) {
			return 0;
		}
	}
	return 1;
}
}

vector<int> verts;
bool alive[MAXN];

int sz[MAXN];
void dfs_sz(int cur, int prv = -1) {
	sz[cur] = 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		dfs_sz(nxt, cur);
		sz[cur] += sz[nxt];
	}
}

int find_centroid(int cur, int prv = -1) {
	if (sz[cur] * 2 < int(verts.size())) {
		return -1;
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		int v = find_centroid(nxt, cur);
		if (v != -1) return v;
	}
	return cur;
}

void push_nodes(vector<int>& v, int cur, int prv) {
	v.push_back(cur);
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		push_nodes(v, nxt, cur);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	if (TEST) {
		test::init_ans();
	}

	verts.resize(N);
	iota(verts.begin(), verts.end(), 0);

	while (verts.size() > 1) {
		if (TEST) {
			cerr << "verts " << verts.size() << '\n';
		}
		for (int i = 0; i < N; i++) {
			alive[i] = false;
		}
		for (int v : verts) {
			alive[v] = true;
		}

		dfs_sz(verts[0]);
		int centroid = find_centroid(verts[0]);

		dfs_sz(centroid);
		vector<pair<int, int>> subtrees;
		for (int nxt : adj[centroid]) {
			if (!alive[nxt]) continue;
			subtrees.emplace_back(sz[nxt], nxt);
		}

		sort(subtrees.begin(), subtrees.end());
		reverse(subtrees.begin(), subtrees.end());

		int i = 0;
		int cur = 0;
		vector<int> q;
		while (cur * 3 < int(verts.size())) {
			cur += subtrees[i].first;
			q.push_back(subtrees[i].second);
			i++;
		}
		assert(i);

		cout << "?" << ' ' << q.size() << ' ' << centroid+1;
		for (int v : q) cout << ' ' << v+1;
		cout << '\n' << flush;

		int resp;
		if (TEST) {
			resp = test::query(centroid, q);
			cerr << resp << '\n';
		} else {
			cin >> resp;
		}
		vector<int> nverts;
		if (resp == 1) {
			// it's in the back half
			nverts.push_back(centroid);
			for (int j = i; j < int(subtrees.size()); j++) {
				push_nodes(nverts, subtrees[j].second, centroid);
			}
		} else {
			// it's in the front half
			if (i > 1) {
				nverts.push_back(centroid);
				// we have to push ourself
			}
			for (int j = 0; j < i; j ++) {
				push_nodes(nverts, subtrees[j].second, centroid);
			}
		}
		verts = nverts;
	}

	cout << "!" << ' ' << verts[0]+1 << '\n' << flush;
	if (TEST) {
		assert(verts[0] == test::ans);
	}

	return 0;
}
