#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		string V(N, '\0');
		for (int i = 0; i < N; i++) {
			V[i] = S[i+i];
		}
		cout << V << '\n';
	}

	return 0;
}
