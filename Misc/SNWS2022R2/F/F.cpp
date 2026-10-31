#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<bool> has(M);
	int ans = 0;
	for (int z = 0; z < N; z++) {
		char op; cin >> op;
		string S; cin >> S;
		if (op == '+') {
			for (int i = 0; i < M; i++) {
				has[i] = has[i] || (S[i] == '1');
			}
		} else if (op == '-') {
			bool bad = false;
			for (int i = 0; i < M; i++) {
				if (S[i] == '1') {
					if (!has[i]) bad = true;
					has[i] = false;
				}
			}
			ans += bad;
		} else assert(false);
	}
	cout << ans << '\n';

	return 0;
}
