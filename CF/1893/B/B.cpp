#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(M); for (auto& b : B) cin >> b;
		std::vector<int> C; C.reserve(N+M);
		std::sort(B.begin(), B.end(), std::greater<>());
		auto it = B.begin();
		for (int i = 0; i < N; i++) {
			while (it != B.end() && *it >= A[i]) {
				C.push_back(*it);
				++it;
			}
			C.push_back(A[i]);
		}
		while (it != B.end()) {
			C.push_back(*it);
			++it;
		}
		for (int i = 0; i < N+M; i++) {
			cout << C[i] << " \n"[i+1==N+M];
		}
	}

	return 0;
}
