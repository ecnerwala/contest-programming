#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
		bool good = (A[0] == 0);
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
