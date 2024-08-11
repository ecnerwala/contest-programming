#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(N); for (auto& b : B) cin >> b;
		std::vector<int> B2(B.rbegin(), B.rend());
		cout << ((A == B || A == B2) ? "Bob" : "Alice") << '\n';
	}

	return 0;
}
