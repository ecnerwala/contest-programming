#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int M; cin >> M;
	std::vector<std::vector<int>> adj(N);
	std::vector<std::pair<int, int>> people(N);
	for (int i = 0; i < N; i++) {
		int k; cin >> k;
		adj[i].resize(k);
		for (auto& v : adj[i]) {
			cin >> v; v--;
		}
		people[i] = {k, i};
	}

	std::vector<int> cur_owner(M, -1);
	std::sort(people.begin(), people.end());
	std::vector<int> num_killed(N);
	std::vector<int> cur_killed; cur_killed.reserve(N);
	for (auto [_, i] : people) {
		cur_killed.clear();
		for (int v : adj[i]) {
			if (cur_owner[v] == -1) {
				cur_owner[v] = i;
			} else {
				int j = cur_owner[v];
				cur_owner[v] = i;

				if (num_killed[j] == 0) {
					cur_killed.push_back(j);
				}
				num_killed[j]++;
			}
		}
		for (int j : cur_killed) {
			if (num_killed[j] != int(adj[j].size())) {
				cout << "YES" << '\n';
				cout << i+1 << ' ' << j+1 << '\n';
				exit(0);
			}
		}
	}
	cout << "NO" << '\n';

	return 0;
}
