#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> A(N);
	for (int t = 0; t < 2*N; t++) {
		char c; cin >> c;
		assert(c == '+' || c == '-');
		int v; cin >> v; v--;
		A[v][c == '+' ? 0 : 1] = t;
	}
	for (int i = 0; i < N; i++) {
		assert(A[i][0] < A[i][1]);
	}
	std::string ans(N, '\0');
	std::vector<int> q; q.reserve(N);
	for (int st = 0; st < N; st++) {
		if (ans[st] != '\0') continue;
		ans[st] = 'G';
		q.push_back(st);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt = 0; nxt < N; nxt++) {
				if (nxt == cur) continue;
				if (
					(A[cur][0] < A[nxt][0] && A[nxt][0] < A[cur][1] && A[cur][1] < A[nxt][1]) ||
					(A[nxt][0] < A[cur][0] && A[cur][0] < A[nxt][1] && A[nxt][1] < A[cur][1])) {
					// cur and nxt have to be opposite
					if (ans[nxt] == '\0') {
						ans[nxt] = ans[cur] ^ 'G' ^ 'S';
						q.push_back(nxt);
					} else if (ans[nxt] == ans[cur]) {
						cout << "*" << '\n';
						exit(0);
					}
				}
			}
		}
		q.clear();
	}
	cout << ans << '\n';

	return 0;
}
