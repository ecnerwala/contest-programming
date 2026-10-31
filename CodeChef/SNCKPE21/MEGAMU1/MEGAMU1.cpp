#include <bits/stdc++.h>

bool is_0(std::vector<int> A) {
	int N = int(A.size());
	if (N == 0) return false;
	if (A[0] == 1) return true;
	int i = 0;
	if (A[0] == 2) {
		int len = 0;
		for (; i < N && A[i] == 2; i++) {
			len++;
		}
		if (i < N && A[i] == 1) {
			if (!(len & 1)) {
				return true;
			}
			while (i < N && A[i] <= 2) i++;
		} else {
			assert(i == N || A[i] >= 3);
			if (len & 1) {
				return true;
			}
		}
	}
	assert(i == N || A[i] >= 3);
	std::vector<std::pair<int, int>> B; B.reserve(N);
	int pref_max = 0;
	for (; i < N; i++) {
		if (A[i] == 1) continue;
		pref_max = std::max(pref_max, A[i]);
		if (A[i] <= pref_max - 2) {
			return true;
		}
		if (B.empty() || B.back().first != A[i]) {
			B.push_back({A[i], 1});
		} else {
			B.back().second++;
		}
	}
	for (int z = 0; z < int(B.size()); z++) {
		if ((z == 0 || B[z-1].first < B[z].first) && (z==int(B.size())-1 || B[z].first < B[z+1].first)) {
			if (B[z].second & 1) {
				return true;
			}
		}
	}
	return false;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A; A.reserve(N);
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			if (a == 0) continue;
			A.push_back(a);
		}
		cout << (is_0(A) ? "yes" : "no") << '\n';
	}

	return 0;
}
