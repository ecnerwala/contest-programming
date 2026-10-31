#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	for (int i = 0; i+1 < N; i++) {
		if (S[i+1] > S[i]) {
			std::swap(S[i], S[i+1]);
			break;
		}
	}
	cout << S << '\n';

	return 0;
}
