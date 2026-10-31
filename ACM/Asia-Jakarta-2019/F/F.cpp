#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4100;
int N;
vector<int> adj[MAXN];

int sz[MAXN];
void dfsSz(int cur, int prv) {
	sz[cur] = 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsSz(nxt, cur);
		sz[cur] += sz[nxt];
	}
}

int dfsCentroid(int cur, int prv, int lim) {
	if (sz[cur] < lim) {
		return false;
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		int v = dfsCentroid(nxt, cur, lim);
		if (v) return v;
	}
	return cur;
}

vector<int> getCentroids(int root) {
	dfsSz(root, 0);
	int a = dfsCentroid(root, 0, int(sz[root] + 1) / 2);
	if (sz[a] * 2 == sz[root]) {
		return {a, dfsCentroid(root, 0, sz[a]+1)};
	} else {
		return {a};
	}
}

using hsh = uint64_t;
hsh hashSet(vector<hsh> vec) {
	sort(vec.begin(), vec.end());
	hsh seed = vec.size();
	for(auto i : vec) {
		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}

hsh dfsHash(int cur, int prv) {
	vector<hsh> chHash;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		chHash.push_back(dfsHash(nxt, cur));
	}
	return hashSet(chHash);
}
hsh getHash(int root) {
	auto v = getCentroids(root);
	if (v.size() == 1) {
		return dfsHash(v[0], 0);
	} else if (v.size() == 2) {
		return hashSet({dfsHash(v[0], v[1]), dfsHash(v[1], v[0])});
	} else assert(false);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int U = getCentroids(1)[0];

	for (int v : adj[U]) {
		int i = 0;
		while (adj[v][i] != U) i++;
		adj[v].erase(adj[v].begin() + i);
	}
	vector<hsh> hashes;
	for (int v : adj[U]) {
		hashes.push_back(getHash(v));
	}
	if (*min_element(hashes.begin(), hashes.end()) == *max_element(hashes.begin(), hashes.end())) {
		cout << adj[U].size() << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
