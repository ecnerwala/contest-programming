#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		int K = 0;
		array<vector<int>, 2> subseqs;
		vector<int> ans(N);
		for (int i = 0; i < N; i++) {
			int v = S[i] - '0';
			if (subseqs[v].empty()) {
				subseqs[v].push_back(K++);
			}
			subseqs[!v].push_back(ans[i] = subseqs[v].back());
			subseqs[v].pop_back();
		}
		cout << K << '\n';
		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
