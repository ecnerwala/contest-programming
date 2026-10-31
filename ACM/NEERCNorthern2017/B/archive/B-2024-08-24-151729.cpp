#include <bits/stdc++.h>

int main() {
	if (fopen("boysgirls.in", "r")) {
		freopen("boysgirls.in", "r", stdin);
		freopen("boysgirls.out", "w", stdout);
	}

	using namespace std;

	int N, X, Y; cin >> N >> X >> Y;
	
	if (X + Y < N) {
		cout << "Impossible" << '\n';
		exit(0);
	}
	if (X == 0 && Y == N) {
		cout << std::string(N, 'G') << '\n';
		exit(0);
	} else if (Y == 0 && X == N) {
		cout << std::string(N, 'B') << '\n';
		exit(0);
	}
	assert(X >= 1 && Y >= 1);

	int G_only = N - X;
	int B_only = N - Y;

	assert(0 <= G_only && G_only < N);
	assert(0 <= B_only && B_only < N);

	if (G_only >= 1) G_only++;
	if (B_only >= 1) B_only++;


	int other = N - (G_only - B_only);
	if (other < 2) {
		cout << "Impossible" << '\n';
		exit(0);
	}
	if (other == 3) {
		// we might be bad
		if (G_only == 0) {
			cout << "GGB" + std::string(N-3, 'B') << '\n';
			exit(0);
		} else if (B_only == 0) {
			cout << "BBG" + std::string(N-3, 'G') << '\n';
			exit(0);
		} else {
			cout << "Impossible" << '\n';
			exit(0);
		}
	} else {
		other -= 2;
		G_only++;
		B_only++;
		assert(other != 1);
		std::string res;
		res += std::string(G_only, 'G');
		res += std::string(B_only, 'B');
		if (other & 1) {
			res += "GGB";
			other -= 3;
		}
		while (other) {
			res += "GB";
			other -= 2;
		}
		cout << res << '\n';
		exit(0);
	}
	assert(false);

	return 0;
}

// N - k of them are girl only, so there are
