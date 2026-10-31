#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int K; cin >> K; K--;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }

	std::vector<int> invA(N);
	for (int i = 0; i < N; i++) invA[A[i]] = i;

	std::vector<std::array<int, 2>> perms; perms.reserve(N*(N+1) / 2);

	for (int i = 0; i < N; i++) {
		for (int v = 0; v < A[i]; v++) {
			int j = invA[v];
			if (j > i) perms.push_back({i, j});
		}
	}
	for (int i = 0; i < N; i++) {
		perms.push_back({i, i});
	}
	for (int i = N-1; i >= 0; i--) {
		for (int v = A[i]+1; v < N; v++) {
			int j = invA[v];
			if (j > i) perms.push_back({i, j});
		}
	}

	{
		auto [i, j] = perms[K];
		std::reverse(A.begin() + i, A.begin() + j + 1);
	}
	for (int i = 0; i < N; i++) {
		cout << A[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
