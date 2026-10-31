#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, L, R; cin >> N >> L >> R;
		vector<int> A(N); for (auto& a : A) cin >> a;
		cout << [&]() -> int64_t {
			vector<int> pos(N+1);
			vector<int> neg(N+1);
			for (int i = 0; i <= N; i++) {
				pos[i] = max(0, (i == N ? 0 : A[i]) - (i == 0 ? 0 : A[i-1]));
				neg[i] = max(0, (i == 0 ? 0 : A[i-1]) - (i == N ? 0 : A[i]));
			}
			for (int i = 0, j = 0; i < N; ) {
				if (!pos[i]) {
					i++;
					continue;
				}
				if (!neg[j]) {
					j++;
					continue;
				}
				int v = min(pos[i], neg[j]);
				assert(v > 0);
				if (j - i < L) return -1;
				pos[i] -= v, neg[j] -= v;
			}

			for (int i = 0; i <= N; i++) {
				pos[i] = max(0, (i == N ? 0 : A[i]) - (i == 0 ? 0 : A[i-1]));
				neg[i] = max(0, (i == 0 ? 0 : A[i-1]) - (i == N ? 0 : A[i]));
			}
			int64_t ans = 0;
			for (int i = N, j = N; i >= 0; ) {
				if (!pos[i]) {
					i--;
					continue;
				}
				if (j > i + R) {
					j--;
					continue;
				}
				if (j >= i+L && !neg[j]) {
					j--;
					continue;
				}
				if (j - i < L) {
					ans += 2 * int64_t(pos[i]);
					pos[i] = 0;
					continue;
				}
				int v = min(pos[i], neg[j]);
				pos[i] -= v;
				neg[j] -= v;
				ans += v;
			}
			return ans;
		}() << '\n';
	}

	return 0;
}
