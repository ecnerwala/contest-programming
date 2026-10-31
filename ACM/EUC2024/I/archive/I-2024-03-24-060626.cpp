#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 3>> A(N);
	for (int i = 0; i < N; i++) {
		int x, y, r; cin >> x >> y >> r;
		A[i] = {x, y, r};
	}
	std::string ans(N, '\0');
	std::vector<int> q; q.reserve(N);

	auto sq = [&](int64_t a) -> int64_t {
		return a * a;
	};
	for (int st = 0; st < N; st++) {
		if (ans[st] != '\0') continue;
		ans[st] = 'G';
		q.push_back(st);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt = 0; nxt < N; nxt++) {
				if (nxt == cur) continue;
				if (sq(A[cur][0] - A[nxt][0]) + sq(A[cur][1] - A[nxt][1]) == sq(A[cur][2] + A[nxt][2])) {
					// cur and nxt have to be opposite
					if (ans[nxt] == '\0') {
						ans[nxt] = ans[cur] ^ 'G' ^ 'S';
						q.push_back(nxt);
					} else if (ans[nxt] == ans[cur]) {
						cout << "NO" << '\n';
						exit(0);
					}
				}
			}
		}
		q.clear();
	}
	cout << "YES" << '\n';

	return 0;
}
