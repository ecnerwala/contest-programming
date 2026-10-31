#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		auto solve = [&]() -> bool {
			int r = 0;
			std::vector<int> vals(S.size());
			for (int i = 0; i < int(S.size()); i++) {
				vals[i] = r;
				int j = (i+1)%int(S.size());
				if ((S[j] - S[i] + 3) % 3 == 1) {
					r++;
				} else {
					r--;
				}
			}
			if (r) return false;

			for (int i = 1; i < N-1; i++) {
				if (abs(vals[i] - vals[(N-1)*3-i]) > N-1) {
					return false;
				}
				if (abs(vals[(N-1)+i] - vals[(N-1)*4-i]) > N-1) {
					return false;
				}
			}
			return true;
		};

		cout << (solve() ? "YES" : "NO") << '\n';
	}

	return 0;
}
