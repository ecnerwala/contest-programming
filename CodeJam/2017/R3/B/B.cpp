#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second
pii E[20];
int res[20];
vector<int> adj[5];

int o(int a, pii p) {
	return p.A ^ p.B ^ a;
}
int o(int a, int e) { return o(a, E[e]); }

vector<int> remove(int a, vector<int> v) {
	vector<int> r;
	for (int i : v) {
		if (i == a) continue;
		r.push_back(i);
	}
	return r;
}

bool go(vector<int> V) {
	if (V.size() == 0) return true;
	if (V.size() == 1) return true;
	for (int v : V) {
		if (adj[v].size() == 1) {
			return false;
		}
	}
	for (int v : V) {
		if (adj[v].size() == 0) {
			return go(remove(v, V));
		}
	}
	for (int v : V) {
		if (adj[v].size() == 2 && o(v,adj[v][0]) == o(v, adj[v][1])) {
			int u = o(v, adj[v][0]);
			res[adj[v][0]] ++;
			res[adj[v][1]] ++;
			// we should delete these
			adj[u] = remove(adj[v][0], remove(adj[v][1], adj[u]));
			return go(remove(v, V));
		}
	}
	assert(V.size() >= 3);
	if (V.size() == 3) {
		sort(V.begin(), V.end());
		int a = V[0], b = V[1], c = V[2];
		vector<pair<pii, int>> edges;
		for (int i : V) {
			for (int e : adj[i]) {
				edges.emplace_back(E[e], e);
			}
		}
		sort(edges.begin(), edges.end());
		edges.resize(size_t(unique(edges.begin(), edges.end()) - edges.begin()));
		for (auto e : edges) {
			bool repeats = false;
			for (auto f : edges) {
				if (e.A.A == f.A.B && e.A.B == f.A.A) {
					repeats = true;
				}
			}
			int v = repeats ? 1 : 2;
			bool forwards = (e.A.A == a && e.A.B == b) || (e.A.A == b && e.A.B == c) || (e.A.A == c && e.A.B == a);
			res[e.B] += (forwards ? 1 : -1) * v;
		}
		return true;
	}
	if (V.size() == 4) {
		vector<int> con[4][4];
		memset(con, 0, sizeof(con));
		vector<pair<pii, int>> edges;
		for (int i : V) {
			for (int e : adj[i]) {
				edges.emplace_back(E[e], e);
				con[E[e].A][E[e].B].push_back(e);
				con[E[e].B][E[e].A].push_back(-1-e);
			}
		}
		sort(V.begin(), V.end());
		do {
			int a = V[0], b = V[1], c = V[2], d = V[3];
			if (con[a][b].empty() || con[b][c].empty() || con[c][d].empty() || con[d][a].empty()) return false;
			if (con[a][c].empty() && con[b][d].empty()) {
				// cycle
				for (int i = 0; i < 4; i++) {
					int u = V[i], v = V[(i+1)%4];
				}
			} else if (con[a][c].empty()) {
			} else if (con[b][d].empty()) {
				continue; //use ac
			} else {
				// K4
			}
			return true;
		} while (next_permutation(V.begin(), V.end()));
		assert(false);
		return false;
	}
}

int F, P;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> F >> P;
		vector<int> st;
		for (int i = 0; i < F; i++) {
			adj[i].clear();
			st.push_back(i);
		}
		for (int i = 0; i < P; i++) {
			cin >> E[i].A >> E[i].B;
			E[i].A --, E[i].B--;
		}

		bool poss = go(st);
		
		cout << "Case #" << case_num << ":";
		if (poss) {
			for (int i = 0; i < P; i++) {
				cout << ' ' << res[i];
			}
			cout << '\n';
		} else {
			cout << ' ' << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
