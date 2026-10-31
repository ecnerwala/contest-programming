#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		string S; cin >> S;
		string R;
		for (int z = 0, lo = 0, hi = K; z < K; z++) {
			if (z & 1) {
				R += S[--hi];
			} else {
				R += S[lo++];
			}
		}
		std::reverse(R.begin(), R.end());
		R.insert(R.end(), S.begin()+K, S.end());
		cout << R << '\n';
	}

	return 0;
}
