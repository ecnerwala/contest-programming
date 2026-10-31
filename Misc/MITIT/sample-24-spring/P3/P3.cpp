#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string N; cin >> N;
		if (N.back() == '2') {
			cout << N << ' ' << 2 << '\n';
		} else {
			int v = 0;
			for (char c : N) {
				v += (c - '0');
				v %= 3;
			}
			if (v % 3 == 0) {
				cout << N << ' ' << 3 << '\n';
			} else if (v % 3 == 1) {
				assert(N.back() == '1' && N.size() >= 2);
				N.pop_back();
				cout << N << ' ' << 3 << '\n';
			} else {
				auto it = std::find(N.begin(), N.end(), '2');
				if (it != N.end()) {
					assert(N.size() >= 2);
					N.erase(it);
					cout << N << ' ' << 3 << '\n';
				} else {
					if (N.size() % 2 != 0) {
						assert(N.size() >= 2);
						N.pop_back();
					}
					cout << N << ' ' << 11 << '\n';
				}
			}
		}
	}

	return 0;
}
