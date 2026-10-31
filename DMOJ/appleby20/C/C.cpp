#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	string S, T; cin >> S >> T;
	vector<array<int, 26>> nxt_char(N);
	array<int, 26> cur; cur.fill(2*N);
	for (int i = N-1; i >= 0; i--) {
		cur[S[i] - 'a'] = N+i;
	}
	for (int i = N-1; i >= 0; i--) {
		cur[S[i] - 'a'] = i;
		nxt_char[i] = cur;
	}

	int64_t loc = 0;
	for (char c : T) {
		int v = nxt_char[loc%N][c-'a'];
		if (v == 2*N) {
			cout << -1 << '\n';
			exit(0);
		}
		loc += v - (loc%N) + 1;
	}
	cout << loc << '\n';

	return 0;
}
