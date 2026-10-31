#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		string I, O; cin >> I >> O;

		cout << "Case #" << case_num << ": " << '\n';
		for (int i = 0; i < N; i++) {
			string res(N, 'N');
			res[i] = 'Y';
			for (int j = i-1; j >= 0; j--) {
				if (I[j] == 'Y' && O[j+1] == 'Y') {
					res[j] = 'Y';
				} else {
					break;
				}
			}
			for (int j = i+1; j < N; j++) {
				if (I[j] == 'Y' && O[j-1] == 'Y') {
					res[j] = 'Y';
				} else {
					break;
				}
			}
			cout << res << '\n';
		}
	}

	return 0;
}
