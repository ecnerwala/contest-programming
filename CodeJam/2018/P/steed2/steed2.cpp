#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int D, N; cin >> D >> N;
		double res = 0;
		for (int i = 0; i < N; i++) {
			int K, S;
			cin >> K >> S;
			assert(0 < K && K < D);
			assert(S > 0);
			res = max(res, double(D-K) / double(S));
		}

		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(10) << D / res << '\n';
	}

	return 0;
}
