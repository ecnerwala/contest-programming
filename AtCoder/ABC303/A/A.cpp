#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S, T; cin >> S >> T;
	bool good = true;
	for (int i = 0; i < N; i++) {
		char a = S[i];
		char b = T[i];
		if (a == b || (a == '1' && b == 'l') || (a == 'l' && b == '1') || (a == '0' && b == 'o') || (a == 'o' && b == '0')) {
		} else {
			good = false;

		}
	}
	cout << (good ? "Yes" : "No") << '\n';

	return 0;
}
