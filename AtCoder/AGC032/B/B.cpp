#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;
int sum[MAXN];
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<pair<int, int>> edges;
	if (N % 2 == 0) {
		for (int i = 1; i <= N; i++) {
			for (int j = i+1; j <= N; j++) {
				if (i + j == N+1) continue;
				edges.emplace_back(i, j);
			}
		}
	} else {
		for (int i = 1; i <= N; i++) {
			for (int j = i+1; j <= N; j++) {
				if (i + j == N) continue;
				edges.emplace_back(i, j);
			}
		}
	}
	for (auto it : edges) {
		int a = it.first, b = it.second;
		assert(a < b);
		sum[a] += b;
		sum[b] += a;
	}
	for (int i = 1; i <= N; i++) {
		assert(sum[i] == sum[1]);
	}
	sort(edges.begin(), edges.end());
	assert(unique(edges.begin(), edges.end()) == edges.end());
	assert(int(edges.size()) >= N-1);
	cout << edges.size() << '\n';
	for (auto it : edges) {
		int a = it.first, b = it.second;
		cout << a << ' ' << b << '\n';
	}

	return 0;
}
