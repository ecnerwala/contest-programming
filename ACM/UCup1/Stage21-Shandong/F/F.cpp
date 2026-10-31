#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, X, Y; cin >> N >> X >> Y;
	int X_LO = 1, X_HI = N;
	int Y_LO = 1, Y_HI = N;
	cout << "Yes" << '\n';
	cout << N-1 << '\n';
	while (N > 1) {
		assert(X_LO <= X && X <= X_HI && Y_LO <= Y && Y <= Y_HI);
		assert(X_HI - X_LO + 1 == N);
		assert(Y_HI - Y_LO + 1 == N);
		if (X == X_LO && Y == Y_LO) {
			cout << X_HI << ' ' << Y_HI << ' ' << -(N-1) << ' ' << -(N-1) << '\n';
			X_HI--, Y_HI--;
		} else if (X == X_LO) {
			cout << X_HI << ' ' << Y_LO << ' ' << -(N-1) << ' ' << +(N-1) << '\n';
			X_HI--, Y_LO++;
		} else if (Y == Y_LO) {
			cout << X_LO << ' ' << Y_HI << ' ' << +(N-1) << ' ' << -(N-1) << '\n';
			X_LO++, Y_HI--;
		} else {
			cout << X_LO << ' ' << Y_LO << ' ' << +(N-1) << ' ' << +(N-1) << '\n';
			X_LO++, Y_LO++;
		}
		N--;
	}

	return 0;
}
