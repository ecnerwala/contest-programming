#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;

	while (T--) {
		string S_; cin >> S_;
		int L = int(S_.size());
		std::vector<int> S(L);
		for (int i = 0; i < L; i++) {
			S[i] = S_[i] - '0';
		}
		std::vector<int> cnd(L);
		auto is_rep_digit = [&]() -> bool {
			for (int i = L-1; i >= 0; i--) {
				if (cnd[i] < 0) {
					if (i == 0) return false;
					cnd[i] += 10;
					cnd[i-1] --;
				}
			}
			int i = 0;
			while (i < L && cnd[i] == 0) i++;
			if (i == L) return false;
			int v = cnd[i];
			while (i < L) {
				if (cnd[i] != v) return false;
				i++;
			}
			return true;
		};
		for (int d = 1; d <= 9; d++) {
			for (int st = 0; st <= 1 && st < int(S.size()); st++) {
				if (st == 0 && d > S[0]) continue;
				std::copy(S.begin(), S.end(), cnd.begin());
				for (int i = st; i < L; i++) {
					cnd[i] -= d;
				}
				if (is_rep_digit()) {
					int i = 0;
					while (cnd[i] == 0) i++;
					for (; i < L; i++) cout << cnd[i];
					cout << ' ';
					for (int j = st; j < L; j++) cout << d;
					cout << '\n';
					goto done;
				}
			}
		}
		assert(false);
done:;
	}

	return 0;
}
