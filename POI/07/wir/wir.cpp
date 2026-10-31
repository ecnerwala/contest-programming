#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<std::array<int, 2>> trie; trie.reserve(3.01e4);
	std::vector<bool> is_end; is_end.reserve(3.01e4);
	trie.push_back({-1,-1});
	is_end.push_back(false);
	int N; cin >> N;
	while (N--) {
		string S; cin >> S;
		int cur = 0;
		for (char c : S) {
			c -= '0';
			if (trie[cur][c] == -1) {
				trie[cur][c] = int(trie.size());
				trie.push_back({-1,-1});
				is_end.push_back(false);
			}
			cur = trie[cur][c];
		}
		is_end[cur] = true;
	}

	vector<int> fail(trie.size());
	vector<int> q; q.reserve(trie.size());
	fail[0] = -1;

	q.push_back(0);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		if (z && is_end[fail[cur]]) is_end[cur] = true;
		for (int c = 0; c < 2; c++) {
			if (trie[cur][c] == -1) {
				trie[cur][c] = (z ? trie[fail[cur]][c] : 0);
			} else {
				if (is_end[cur]) is_end[trie[cur][c]] = true;
				fail[trie[cur][c]] = (z ? trie[fail[cur]][c] : 0);
				q.push_back(trie[cur][c]);
			}
		}
	}

	vector<int> outDeg(trie.size());
	vector<vector<int>> inEdges(trie.size());
	for (int cur = 0; cur < int(trie.size()); cur++) {
		if (is_end[cur]) continue;
		for (int c = 0; c < 2; c++) {
			if (is_end[trie[cur][c]]) continue;
			outDeg[cur]++;
			inEdges[trie[cur][c]].push_back(cur);
		}
	}

	vector<int> sinks; sinks.reserve(trie.size());
	for (int cur = 0; cur < int(trie.size()); cur++) {
		if (is_end[cur]) continue;
		if (outDeg[cur] == 0) sinks.push_back(cur);
	}
	for (int z = 0; z < int(sinks.size()); z++) {
		int cur = sinks[z];
		for (int nxt : inEdges[cur]) {
			if (!--outDeg[nxt]) sinks.push_back(nxt);
		}
	}

	cout << (outDeg[0] ? "TAK" : "NIE") << '\n';
	return 0;
}
