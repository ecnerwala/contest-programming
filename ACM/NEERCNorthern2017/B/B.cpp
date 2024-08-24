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

	int other = N - (G_only - B_only);
	if (other < 0) {
		cout << "Impossible" << '\n';
		exit(0);
	} else if (other == 4) {
		cout << "G" + std::string(G_only, 'G') + "GB" + std::string(B_only, 'B') + "B" << '\n';
		exit(0);
	} else if (other >= 6) {
		std::string res = "G" + std::string(G_only, 'G') + "GB" + std::string(B_only, 'B') + "B";
		other -= 4;
		assert(other >= 2);
		if (other & 1) {
			res += "GGB";
			other -= 3;
		}
		for (; other; other -= 2) {
			res += "GB";
		}
		cout << res << '\n';
		exit(0);
	} else if (other == 5) {
	}

	return 0;
}

// N - k of them are girl only, so there are
