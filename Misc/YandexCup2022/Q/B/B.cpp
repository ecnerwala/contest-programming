#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N; cin >> N;
		int B; cin >> B;

		int64_t cnt_tot = 1;
		int64_t cnt_le = 1;
		while (N) {
			cnt_le = (N % 10 % B == 0 ? cnt_le : 0) + ((N % 10 + B - 1) / B) * cnt_tot;
			cnt_tot *= 9 / B + 1;
			N /= 10;
		}
		cout << cnt_le - 1 << '\n';
	}

	return 0;
}
