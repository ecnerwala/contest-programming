#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		std::vector<std::pair<int, int>> B; B.reserve(N);
		{
			int cnt = 0;
			for (int i = 0; i < N; i++) {
				cnt = (i > 0 && A[i] == A[i-1] ? cnt + 1 : 0);
				B.push_back({i, -A[i]});
			}
		}
		std::sort(B.begin(), B.end());
		for (int i = 0; i < N; i++) cout << A[i] << " \n"[i+1==N];
		for (int i = 0; i < N; i++) cout << -B[i].second << " \n"[i+1==N];
	}

	return 0;
}
