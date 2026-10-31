#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		auto read_vec = [&]() -> std::vector<int> {
			int k; cin >> k;
			std::vector<int> r(k);
			for (auto& v : r) { cin >> v; v--; }
			return r;
		};
		cout << '?' << ' ' << "R" << '\n' << flush;
		auto vR = read_vec();
		cout << '?' << ' ' << "RL" << '\n' << flush;
		auto vRL = read_vec();
		cout << '?' << ' ' << "L" << '\n' << flush;
		auto vL = read_vec();

		assert(int(vRL.size()) == int(vR.size()));
		std::vector<int> ans(N, -1);
		std::vector<int> idx(4*N, -1);
		for (int i = 0; i < N; i++) idx[A[i]] = i;
		for (int z = 0; z < int(vRL.size()); z++) {
			int i = idx[vRL[z]];
			assert(i != -1);
			ans[i] = vR[z] - vRL[z];
		}
		std::vector<bool> vL_has(4*N);
		for (auto v : vL) {
			v++;
			if (0 <= v && v < 4*N) vL_has[v] = true;
		}
		bool bad = false;
		for (int i = N-1; i >= 0; i--) {
			if (ans[i] == -1) {
				assert(i+1 < N);
				assert(A[i+1] >= A[i] + 1);
				assert(A[i+1] <= A[i] + 2);
				if (ans[i+1] == 0 && A[i+1] == A[i] + 1) {
					if (i > 0 && ans[i-1] == 0 && A[i-1] == A[i] - 1) {
						// then we don't know this guy
						bad = true;
						break;
					} else {
						// this guy is 1 iff vL has A[i]-1
						ans[i] = vL_has[A[i]] ? 1 : 2;
					}
				} else {
					// otherwise, we know it for sure
					ans[i] = 2;
				}
			}
		}
		if (bad) {
			cout << '!' << ' ' << -1 << '\n' << flush;
		} else {
			cout << '!'; for (int i = 0; i < N; i++) { cout << ' ' << ans[i]; } cout << '\n' << flush;
		}
	}

	return 0;
}
