#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);


	int N, D; cin >> N >> D;

	vector<array<int, 2>> v(N);
	for (int i = 0; i < N; ++i) {
		cin >> v[i][0] >> v[i][1];


	}



			std::vector<int> q; q.reserve(N);
		q.push_back(0);
		std::vector<bool> vis(N);
		for (int z = 0; z < int(q.size()); z++) {

			int cur = q[z];

			for (int nxt = 0; nxt < N; nxt++) {
				if (vis[nxt]) continue;


				int dist = (v[cur][0] - v[nxt][0]) * (v[cur][0] - v[nxt][0]) + (v[cur][1] - v[nxt][1]) * (v[cur][1] - v[nxt][1]);

				if (dist <= D * D) {
					vis[nxt] = true;
					q.push_back(nxt);
				}
			}
		}

		for (int i = 0; i < N; i++) {
			if (vis[i]) cout << "Yes\n";
			else cout << "No\n";
		}

	return 0;
}
