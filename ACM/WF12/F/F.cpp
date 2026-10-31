#include<bits/stdc++.h>
using namespace std;

const pair<int, int> BAD(int(1e9), int(1e9));

pair<int, int> go() {
	//cerr << "Start Case =========" << '\n';

	map<char, int> ringInds;

	vector<pair<int, int>> edges;
	vector<pair<int, int>> cnts;

	auto getRingID = [&](char c) -> int {
		if ('A' <= c && c <= 'M') return -1;
		else if ('N' <= c && c <= 'Z') return -2;
		else if ('a' <= c && c <= 'z') {
			if (ringInds.count(c)) return ringInds[c];
			int ind = int(ringInds.size());
			ringInds[c] = ind;
			cnts.emplace_back(0, 0);
			return ind;
		} else assert(false);
	};

	while (true) {
		string s;
		if (!(cin >> s)) exit(0);
		if (s == "0") break;
		int a = getRingID(s[0]), b = getRingID(s[1]);
		assert(!(a < 0 && b < 0));
		if (a == -1) {
			cnts[b].first ++;
		} else if (a == -2) {
			cnts[b].second ++;
		} else if (b == -1) {
			cnts[a].first ++;
		} else if (b == -2) {
			cnts[a].second ++;
		} else {
			edges.emplace_back(a, b);
		}
	}

	/*
	for (auto it : ringInds) {
		cerr << it.first << ' ' << it.second << ' ' << cnts[it.second].first << ' ' << cnts[it.second].second << '\n';
	}
	for (auto it : edges) {
		cerr << it.first << ' ' << it.second << '\n';
	}
	*/

	int V = int(ringInds.size());
	assert(V >= 1);

	vector<vector<bool>> connected(V, vector<bool>(V));
	for (int i = 0; i < V; i++) connected[i][i] = true;
	for (auto it : edges) {
		connected[it.first][it.second] = true;
		connected[it.second][it.first] = true;
	}

	for (int k = 0; k < V; k++) {
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				if (connected[i][k] && connected[k][j]) connected[i][j] = true;
			}
		}
	}

	vector<vector<int>> ccs; // the nonempty ccs
	vector<vector<int>> emptyCCs;
	for (int i = 0; i < V; i++) {
		bool isRoot = true;
		for (int j = 0; j < i; j++) {
			if (connected[j][i]) isRoot = false;
		}
		if (!isRoot) continue;
		vector<int> cc;
		for (int j = i; j < V; j++) {
			if (connected[i][j]) cc.push_back(j);
		}
		//for (int j : cc) { cerr << j << ' '; }cerr << '\n';
		bool isEmpty = true;
		for (int v : cc) {
			if (cnts[v].first || cnts[v].second) isEmpty = false;
		}
		if (isEmpty) {
			emptyCCs.push_back(cc);
		} else {
			ccs.push_back(cc);
		}
	}

	pair<int, int> totCnt;
	for (int i = 0; i < V; i++) {
		totCnt.first += cnts[i].first;
		totCnt.second += cnts[i].second;
	}
	if (totCnt.first == 0 && totCnt.second == 0) {
		return {0,0};
	}
	assert(!ccs.empty());

	if (totCnt.first == 0 || totCnt.second == 0) {
		// join all nonempty ones
		return {0, int(ccs.size()) - 1};
	}
	assert(totCnt.first > 0 && totCnt.second > 0);
	if (V == 1) {
		return BAD; // not enough rings;
	}

	vector<vector<int>> adj(V);
	for (auto it : edges) {
		adj[it.first].push_back(it.second);
		adj[it.second].push_back(it.first);
	}
	// now, we need to assign each vertex to true or false
	// 4 states: is 0, is 1, all 0, all 1
	const auto dfs = [&](int cur, int prv, auto& dfs_rec) -> vector<pair<int, int>> {
		vector<pair<int, int>> res({BAD, BAD, BAD, BAD});
		res[2] = pair<int, int>{2 * cnts[cur].second, 0};
		res[3] = pair<int, int>{2 * cnts[cur].first, 0};
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto chres = dfs_rec(nxt, cur, dfs_rec);
			vector<pair<int, int>> nres({BAD, BAD, BAD, BAD});
			for (int i = 0; i < 4; i++) {
				if (res[i] == BAD) continue;
				for (int j = 0; j < 4; j++) {
					if (chres[j] == BAD) continue;
					pair<int, int> cnd(res[i].first + chres[j].first, res[i].second + chres[j].second + 2 * (i % 2 != j % 2)); // cut and join
					int ni;
					if (i % 2 != j % 2) {
						// becomes i & 1
						ni = i & 1;
					} else {
						ni = i & j; // bad if either is bad
					}
					nres[ni] = min(nres[ni], cnd);
				}
			}
			res = nres;
		}
		//cerr << cur << '\n';
		//for (int i = 0; i < 4; i++) { cerr << res[i].first << ',' << res[i].second << ' '; } cerr << '\n';
		return res;
	};

	vector<pair<int, int>> res;
	for (const auto& cc : ccs) {
		auto chres = dfs(cc[0], -1, dfs);
		if (res.empty()) {
			res = chres;
		} else {
			vector<pair<int, int>> nres(4, BAD);
			for (int i = 0; i < 4; i++) {
				if (res[i] == BAD) continue;
				for (int j = 0; j < 4; j++) {
					if (chres[j] == BAD) continue;
					pair<int, int> cnd(res[i].first + chres[j].first, res[i].second + chres[j].second + 1); // costs 1 to join them
					int ni;
					if (i % 2 != j % 2) {
						ni = i & 1;
					} else {
						ni = i & j;
					}
					nres[ni] = min(nres[ni], cnd);
				}
			}
			res = nres;
		}
	}

	auto ans = min(res[0], res[1]);
	if (!emptyCCs.empty()) {
		// we can take one extra from the second
		if (res[2] != BAD) {
			res[2].second ++; // take one extra
			ans = min(ans, res[2]);
		}
		if (res[3] != BAD) {
			res[3].second ++; // take one extra
			ans = min(ans, res[3]);
		}
	}

	assert(ans != BAD);
	ans.second --; // one cut doesn't have to be rejoined
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	for (int t = 1; true; t++) {
		pair<int, int> ans = go();

		cout << "Case " << t << ": ";
		if (ans == BAD) cout << "impossible" << '\n';
		else cout << ans.first << ' ' << ans.second << '\n';
	}

	return 0;
}
