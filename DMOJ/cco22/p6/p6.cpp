#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	std::map<string, std::pair<string, std::array<int, 2>>> prv;
	prv[S] = {"", {-1, -1}};
	std::vector<string> q;
	q.push_back(S);
	for (int z = 0; z < int(q.size()); z++) {
		if (q[z] == "") {
			std::vector<std::array<int, 2>> ops;
			for (string cur = q[z]; cur != S; cur = prv[cur].first) {
				ops.push_back(prv[cur].second);
			}
			reverse(ops.begin(), ops.end());
			cout << ops.size() << '\n';
			for (auto [x, y] : ops) {
				cout << x+1 << ' ' << y-x << '\n';
			}
			exit(0);
		}
		string cur = q[z];
		for (int i = 0; i < int(cur.size()); i++) {
			for (int j : {i+2, i+3}) {
				if (j > int(cur.size())) break;
				string v = cur.substr(i, j-i);
				if (v == "AA" || v == "AAA" || v == "BB" || v == "BBB") {
					string next = cur.substr(0, i) + cur.substr(j);
					if (prv.count(next)) continue;
					prv[next] = {cur, {i, j}};
					q.push_back(next);
				}
			}
		}
	}

	cout << -1 << '\n';

	return 0;
}
