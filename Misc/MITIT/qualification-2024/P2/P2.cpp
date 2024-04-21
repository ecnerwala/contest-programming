#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> bigs; bigs.reserve(N);
		std::vector<int> solos; solos.reserve(N);
		int S = 0;
		for (int i = 0; i < N; i++) {
			S += A[i];
			if (A[i] > 1) {
				bigs.push_back(i);
			} else if (A[i] == 1) {
				solos.push_back(i);
			}
		}
		if (!(S & 1)) {
			cout << "NO" << '\n';
			continue;
		}
		std::vector<int> ops; ops.reserve(S);
		std::vector<int> pad; pad.reserve(S);
		for (int t = 0; t < S; t++) {
			if (t & 1) {
				if (pad.empty()) {
					break;
				}
				ops.push_back(pad.back());
				pad.pop_back();
			} else {
				if (!bigs.empty()) {
					ops.push_back(bigs.back());
					pad.insert(pad.end(), A[bigs.back()] - 1, bigs.back());
					bigs.pop_back();
				} else if (!solos.empty()) {
					ops.push_back(solos.back());
					solos.pop_back();
				} else {
					ops.push_back(pad.back());
					pad.pop_back();
				}
			}
		}
		if (int(ops.size()) == S) {
			cout << "YES" << '\n';
			std::reverse(ops.begin(), ops.end());
			for (int i = 0; i < S; i++) {
				cout << ops[i]+1 << " \n"[i+1==S];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
