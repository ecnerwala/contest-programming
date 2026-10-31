#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A; A.reserve(K+1); A.resize(K);
		for (int i = 0; i < K; i++) {
			cin >> A[i];
		}
		if (!A.empty() && A.back() == N) {
			A.pop_back();
		} else {
			A.push_back(N);
		}
		cout << A.size() << 'n';
		for (int i = 0; i < int(A.size()); i++) {
			cout << A[i] << " \n"[i+1==int(A.size())];
		}
	}

	return 0;
}
