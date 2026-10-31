#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<int> S(N);
		for (auto& s : S) cin >> s;
		std::sort(S.begin(), S.end());

		bool is_good = K * 2 >= N;
		for (int i = 0; i+2 < N; i++) {
			if (S[i+2] == S[i]) is_good = false;
		}

		cout << "Case #" << case_num << ": " << (is_good ? "YES" : "NO") << '\n';
	}

	return 0;
}
