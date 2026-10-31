#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, A, B; cin >> N >> A >> B;

	string S; cin >> S;

	vector<int> bigs; bigs.reserve(A);
	vector<int> smalls; smalls.reserve(B);

	for (int i = 0; A > 0 && i+1 < N; i++) {
		if (S[i] == '0' && S[i+1] == '0') {
			S[i] = '1';
			S[i+1] = '1';
			A--;
			bigs.push_back(i);
		}
	}
	for (int i = 0; B > 0 && i < N; i++) {
		if (S[i] == '0') {
			S[i] = '1';
			B--;
			smalls.push_back(i);
		}
	}

	if (A || B) {
		cout << "NO" << '\n';
		exit(0);
	}
	cout << "YES" << '\n';
	for (int i : bigs) cout << i+1 << ' ' << i+2 << '\n';
	for (int i : smalls) cout << i+1 << '\n';

	return 0;
}
