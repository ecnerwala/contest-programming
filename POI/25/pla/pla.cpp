#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	if (N == 1) {
		cout << "TAK" << '\n';
		exit(0);
	} else if (N == 2) {
		cout << "TAK" << '\n';
		cout << 1 << ' ' << 2 << ' ' << 1 << '\n';
		exit(0);
	}
	vector<array<int, 2>> D(N-2);
	for (auto& d : D) cin >> d[0];
	for (auto& d : D) cin >> d[1];

	{ // First check if it's possible to be adjacent
		int cnd = abs(D[0][0] - D[0][1]);
		bool good = cnd > 0;
		for (auto d : D) {
			if (abs(d[0] - d[1]) != cnd) {
				good = false;
				break;
			}
		}
		if (good) {
			cout << "TAK" << '\n';
			cout << 1 << ' ' << N << ' ' << cnd << '\n';
			for (int i = 0; i < N-2; i++) {
				int closer = (D[i][0] < D[i][1]) ? 1 : N;
				cout << closer << ' ' << i+2 << ' ' << min(D[i][0], D[i][1]) << '\n';
			}
			exit(0);
		}
	}

	int cnd = D[0][0] + D[0][1];
	for (auto d : D) cnd = min(cnd, d[0] + d[1]);

	assert(cnd > 0);

	map<int, int> path;
	path.insert({0, 1});
	path.insert({cnd, N});
	for (int i = 0; i < N-2; i++) {
		auto d = D[i];
		if (d[0] + d[1] == cnd) {
			if (path.count(d[0])) {
				cout << "NIE" << '\n';
				exit(0);
			}
			path[d[0]] = i+2;
		}
	}

	vector<tuple<int, int, int>> edges; edges.reserve(N-1);
	for (auto it = path.begin(); next(it) != path.end(); ++it) {
		edges.push_back({it->second, next(it)->second, next(it)->first - it->first});
	}
	for (int i = 0; i < N-2; i++) {
		auto d = D[i];
		if (d[0] + d[1] == cnd) continue;
		if ((d[0] + d[1] - cnd) & 1) {
			cout << "NIE" << '\n';
			exit(0);
		}
		int w = d[0] - (d[0] + d[1] - cnd) / 2;
		if (!path.count(w)) {
			cout << "NIE" << '\n';
			exit(0);
		}
		edges.push_back({path[w], i+2, (d[0] + d[1] - cnd) / 2});
	}


	cout << "TAK" << '\n';
	for (auto [x, y, d] : edges) {
		cout << x << ' ' << y << ' ' << d << '\n';
	}

	return 0;
}
