#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::multiset<int> A;
	for (int i = 0; i < N; i++) {
		int a; cin >> a; A.insert(a);
	}

	int cur = M+1;
	std::vector<int> B; B.reserve(N);
	for (int i = 0; i < N; i++) {
		auto it = A.lower_bound(M - cur);
		if (it == A.end()) {
			cout << -1 << '\n';
			exit(0);
		}
		B.push_back(cur = *it);
		A.erase(it);
	}
	for (int i = 0; i < N; i++) {
		cout << B[i] << " \n"[i+1==N];
	}

	return 0;
}
