#include <bits/stdc++.h>

std::vector<int> get_lis(const std::vector<int>& A) {
	std::vector<int> dp; dp.reserve(int(A.size()));
	for (auto v : A) {
		int mi = -1;
		int ma = int(dp.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (v > dp[md]) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == int(dp.size())) {
			dp.push_back(v);
		} else {
			dp[ma] = v;
		}
	}
	return dp;
}

std::optional<std::vector<int>> solve(std::vector<int> A, int M) {
	int NA = int(A.size());
	int NB = M - NA;
	std::vector<int> B; B.reserve(NB);
	{
		std::vector<bool> used(M);
		for (auto a : A) {
			used[a] = true;
		}
		for (int i = 0; i < M; i++) {
			if (!used[i]) B.push_back(i);
		}
		assert(int(B.size()) == NB);
	}

	std::array<std::vector<int>, 2> dp;
	std::array<int, 2> has_incr;
	for (int z = 0; z < 2; z++) {
		dp[z] = get_lis(A);
		has_incr[z] = B.back() > dp[z].back();

		{
			for (auto& a : A) a = M-1-a;
			std::reverse(A.begin(), A.end());
			for (auto& b : B) b = M-1-b;
			std::reverse(B.begin(), B.end());
		}
	}

	if (has_incr[0] == has_incr[1]) {
		std::reverse(B.begin(), B.end());
		return B;
	}
	assert(has_incr[0] != has_incr[1]);
	bool should_flip = has_incr[1];
	if (should_flip) {
		for (auto& a : A) a = M-1-a;
		std::reverse(A.begin(), A.end());
		for (auto& b : B) b = M-1-b;
		std::reverse(B.begin(), B.end());
		std::swap(dp[0], dp[1]);
		std::swap(has_incr[0], has_incr[1]);
	}

	assert(has_incr[0]);
	assert(!has_incr[1]);

	assert(dp[0].size() == dp[1].size());
	int VA = int(dp[0].size());

	{
		int la = VA;
		for (int j = 0; true; j++) {
			if (j == NB) return std::nullopt;
			while (la > 0 && M-1-dp[1][la-1] < B[j]) {
				la--;
			}
			if (j+1 + la > VA) {
				assert(j + 1 + la == VA + 1);
				// we're hopefully good
				std::reverse(B.begin(), B.begin() + j + 1);
				std::reverse(B.begin(), B.end());

				std::vector<int> cnd(M);
				std::copy(A.begin(), A.end(), cnd.begin());
				std::copy(B.begin(), B.end(), cnd.begin() + NA);
				if (VA + 1 == int(get_lis(cnd).size())) {
					if (should_flip) {
						for (auto &b : B) b = M - 1 - b;
						std::reverse(B.begin(), B.end());
					}
					return B;
				} else {
					return std::nullopt;
				}
			}
		}
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		auto res = solve(A, M);
		if (res) {
			cout << "Yes" << '\n';
			for (int i = 0; i < M-N; i++) {
				cout << (*res)[i]+1 << " \n"[i+1==M-N];
			}
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
