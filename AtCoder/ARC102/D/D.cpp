#include<bits/stdc++.h>
using namespace std;

int main() {
	int L;
	cin >> L;
	int N = 20;
	vector<pair<pair<int, int>, int>> edges;
	for (int i = 2; i + 1 <= N; i++) {
		edges.emplace_back(pair<int, int>(i, i + 1), 0);
		edges.emplace_back(pair<int, int>(i, i + 1), 1 << (N - i - 1));
	}
	int st = 0;
	for (int i = 2; i <= N; i++) {
		while ((L - st) >= (1 << (N - i))) {
			edges.emplace_back(pair<int, int>(1, i), st);
			st += (1 << (N - i));
		}
	}
	cout << N << ' ' << edges.size() << '\n';
	for (auto it : edges) {
		assert(it.first.first < it.first.second);
		cout << it.first.first << ' ' << it.first.second << ' ' << it.second << '\n';
	}

	return 0;
}
