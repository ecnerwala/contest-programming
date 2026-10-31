#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M, K; cin >> N >> M >> K;
	set<string> dict;
	set<string> almost_dict;
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		dict.insert(S);
		sort(S.begin(), S.end());
		almost_dict.insert(S);
	}

	vector<int> dp(K+1, 0);
	vector<int> ndp(K+1, 0);
	for (int j = 0; j < M; j++) {
		fill(ndp.begin(), ndp.end(), M+1);
		string T; cin >> T;
		if (dict.count(T)) {
			ndp[0] = *min_element(dp.begin(), dp.end());
		} else {
			sort(T.begin(), T.end());

			if (almost_dict.count(T)) {
				ndp[0] = *min_element(dp.begin(), dp.end());
			} else {
				// if we just correct it, we reset to 0
				ndp[0] = *min_element(dp.begin(), dp.end()) + 1;

				// we could just leave it
				for (int i = 0; i < K; i++) {
					ndp[i+1] = dp[i];
				}
			}
		}

		for (int i = 0; i < K; i++) {
			ndp[i+1] = min(ndp[i+1], ndp[i]);
		}
		swap(dp, ndp);
	}

	cout << *min_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
