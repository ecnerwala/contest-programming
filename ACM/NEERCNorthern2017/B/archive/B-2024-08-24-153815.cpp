#include <bits/stdc++.h>

std::string solve_cyc(int N, int GG, int BB) {
	assert(0 <= GG && GG <= N);
	assert(0 <= BB && BB <= N);
	if (GG == N && BB == 0) {
		return std::string(N, 'G');
	} else if (GG == 0 && BB == N) {
		return std::string(N, 'B');
	}
	assert(GG + BB + 2 <= N);
	assert((N - GG - BB) % 2 == 0);
	std::string res;
	res.reserve(N);
	res += std::string(GG+1, 'G');
	res += std::string(BB+1, 'B');
	while (int(res.size()) < N) res += "GB";
	return res;
}

int main() {
	if (fopen("boysgirls.in", "r")) {
		freopen("boysgirls.in", "r", stdin);
		freopen("boysgirls.out", "w", stdout);
	}

	using namespace std;

	int N, X, Y; cin >> N >> X >> Y;

	int G_only = N - X;
	int B_only = N - Y;

	if (G_only + B_only > N) {
		cout << "Impossible" << '\n';
		exit(0);
	}
	// G_only means GG
	// B_only means BB
	// if you alternate at least once, then G_only + B_only + 2 * k = N for k >= 1
	// otherwise, G_only == N or B_only == N
	if ((G_only ^ B_only ^ N) & 1) {
		cout << "Impossible" << '\n';
		exit(0);
	}
	if (G_only + B_only == N) {
		// must be tight
		if (G_only == N) {
			cout << std::string(N, 'G') << '\n';
			exit(0);
		} else if (B_only == N) {
			cout << std::string(N, 'B') << '\n';
			exit(0);
		} else if (G_only == B_only) {
			std::string res(N, '?');
			for (int i = 0; i < N; i++) {
				res[i] = "GB"[i&1];
			}
			cout << res << '\n';
			exit(0);
		} else {
			cout << "Impossible" << '\n';
			exit(0);
		}
	} else {
		int diff = N - (G_only + B_only);
		assert(!(diff & 1));
		diff /= 2;
		if (N % 2 == 0) {
			int ag, ab;
			if (G_only >= N/2) {
				// one side is alternting G's
				ag = N/2, ab = 0;
			} else if (B_only >= N/2) {
				ag = 0, ab = N/2;
			} else {
				if (G_only == N/2 - 1 && B_only == N/2 - 1) {
					cout << "Impossible" << '\n';
					exit(0);
				}
				assert(G_only + B_only <= N - 4);
				ag = std::min(G_only, N/2-2);
				ab = std::min(B_only, N/2 - 2 - ag);
				if ((N - ag - ab) & 1) {
					if (ab) {
						ab--;
					} else if (ag) {
						ag--;
					} else {
						cout << "Impossible" << '\n';
						exit(0);
					}
				}
			}
			int bg = G_only - ag;
			int bb = B_only - ab;
			std::string a = solve_cyc(N/2, ag, ab);
			std::string b = solve_cyc(N/2, bg, bb);
			std::string res;
			for (int i = 0; i < N/2; i++) {
				res += a[i];
				res += b[i];
			}
			cout << res << '\n';
			exit(0);
		} else {
			std::string a = solve_cyc(N, G_only, B_only);
			std::string res(N, '?');
			for (int i = 0; i < N; i++) {
				res[(2*i)%N] = a[i];
			}
			cout << res << '\n';
			exit(0);
		}
		assert(false);
	}

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

	assert(0 <= G_only && G_only < N);
	assert(0 <= B_only && B_only < N);

	if (G_only >= 1) G_only++;
	if (B_only >= 1) B_only++;

	// we look at the alternating sequences
	cerr << G_only << ' ' << B_only << '\n';

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
