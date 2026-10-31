#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A; A.reserve(N);
		int num_1 = 0;
		for (int i = 0; i < N; i++) {
			int v; cin >> v;
			assert(v >= 1);
			if (v == 1) num_1++;
			else A.push_back(v);
		}

		auto sprague_wins = [&]() -> bool {
			if (A.size() >= 2) return false;
			if (A.empty()) {
				return num_1 % 3 == 1;
			}
			assert(A.size() == 1);
			return num_1 % 3 == 0 || num_1 % 3 == 2;
		};

		if (sprague_wins()) {
			cout << "Sprague" << '\n';
		} else {
			cout << "Grundy" << '\n';
		}
	}

	return 0;
}
