#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	const int L = 12;
	std::array<int, L+1> pow_3;
	pow_3[0] = 1;
	for (int i = 0; i < L; i++) pow_3[i+1] = pow_3[i] * 3;

	int N; cin >> N;
	std::vector<int> S(N);
	for (auto& s : S) cin >> s;

	std::vector<bool> is_S(pow_3[L]);
	for (int i = 0; i < N; i++) {
		is_S[S[i]] = true;
	}
	std::vector<bool> vis(pow_3[L]);
	std::vector<int> q; q.reserve(pow_3[L]);
	vis[S[0]] = true;
	q.push_back(S[0]);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int i = 0; i < L; i++) {
			int d = cur / pow_3[i] % 3;
			for (int nd = (is_S[cur] ? 0 : d); nd < 3; nd++) {
				int nxt = cur + (nd - d) * pow_3[i];
				if (!vis[nxt]) {
					vis[nxt] = true;
					q.push_back(nxt);
				}
			}
		}
	}

	cout << (vis[S[N-1]] ? "Yes" : "No") << '\n';

	return 0;
}
