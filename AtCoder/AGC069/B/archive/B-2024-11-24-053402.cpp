#include <bits/stdc++.h>

bool slow(const std::vector<std::string>& S) {
	int N = int(S.size());
	std::vector<int> A(N); std::iota(A.begin(), A.end(), 0);
	std::vector<int> B(N); std::iota(B.begin(), B.end(), 0);
	do {
		do {
			auto curS = S;
			bool good = true;
			for (int i = 0; i < N-1; i++) {
				bool has0 = 0;
				for (int j = 0; j < N; j++) {
					if (curS[A[i]][j] == '0') {
						has0 = true;
						curS[A[i]][j] = '1';
					}
					if (curS[j][B[i]] == '0') {
						has0 = true;
						curS[j][B[i]] = '1';
					}
				}
				if (!has0) { good = false; break; }
			}
			if (good) return true;
		} while (std::next_permutation(B.begin(), B.end()));
	} while (std::next_permutation(A.begin(), A.end()));
	return false;
}

bool solve(const std::vector<std::string>& S) {
	int N = int(S.size());
	std::vector<int> par(2*N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (S[i][j] == '0') {
				cnt += merge(i, N+j);
			}
		}
	}
	return cnt >= N-1;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);
	for (int z = 0; z < 10000; z++) {
		cerr << "test " << z << '\n';
		int N = std::uniform_int_distribution<int>(1, 5)(mt);
		double p = std::uniform_real_distribution<double>(0, 1)(mt);
		std::vector<std::string> S(N);
		cerr << N << '\n';
		for (int i = 0; i < N; i++) {
			S[i].reserve(N);
			for (int j = 0; j < N; j++) {
				S[i].push_back('0' + std::bernoulli_distribution(p)(mt));
			}
			cerr << S[i] << '\n';
		}
		cerr << solve(S) << '\n';
		cerr << slow(S) << '\n';
		assert(solve(S) == slow(S));
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> S(N);
		for (auto& s : S) cin >> s;
		if (solve(S)) {
			cout << "Yes" << '\n';
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
