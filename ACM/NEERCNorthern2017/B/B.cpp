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

std::string solve(int N, int X, int Y) {
	int G_only = N - X;
	int B_only = N - Y;

	if (G_only + B_only > N) {
		return "Impossible";
	}
	// G_only means GG
	// B_only means BB
	// if you alternate at least once, then G_only + B_only + 2 * k = N for k >= 1
	// otherwise, G_only == N or B_only == N
	if ((G_only ^ B_only ^ N) & 1) {
		return "Impossible";
	}
	if (G_only + B_only == N) {
		// must be tight
		if (G_only == N) {
			return std::string(N, 'G');
		} else if (B_only == N) {
			return std::string(N, 'B');
		} else if (G_only == B_only) {
			std::string res(N, '?');
			for (int i = 0; i < N; i++) {
				res[i] = "GB"[i&1];
			}
			return res;
		} else {
			return "Impossible";
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
					return "Impossible";
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
						return "Impossible";
					}
				}
			}
			int bg = G_only - ag;
			int bb = B_only - ab;
			std::cerr << N << ' ' << G_only << ' ' << B_only << ' ' << ag << ' ' << ab << ' ' << bg << ' ' << bb << '\n';
			std::string a = solve_cyc(N/2, ag, ab);
			std::string b = solve_cyc(N/2, bg, bb);
			std::string res;
			for (int i = 0; i < N/2; i++) {
				res += a[i];
				res += b[i];
			}
			return res;
		} else {
			std::string a = solve_cyc(N, G_only, B_only);
			std::string res(N, '?');
			for (int i = 0; i < N; i++) {
				res[(2*i)%N] = a[i];
			}
			return res;
		}
		assert(false);
	}
}

void test() {
	for (int N = 2; N <= 10; N++) {
		for (int X = 0; X <= N; X++) {
			for (int Y = 0; Y <= N; Y++) {
				std::cerr << solve(N, X, Y) << '\n';
			}
		}
	}
}

int main() {
	if (fopen("boysgirls.in", "r")) {
		freopen("boysgirls.in", "r", stdin);
		freopen("boysgirls.out", "w", stdout);
	}

	test();
	using namespace std;

	int N, X, Y; cin >> N >> X >> Y;
	cout << solve(N, X, Y) << '\n';
	return 0;
}

// N - k of them are girl only, so there are
