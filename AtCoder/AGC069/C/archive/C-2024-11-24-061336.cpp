#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S, T; cin >> S >> T;

	const int INF = 1e9;
	std::array<int, 5> dp; dp.fill(INF); dp[0] = 0;
	for (int i = 0; i < N; i++) {
		std::array<int, 5> ndp; ndp.fill(INF);
		int st = S[i] == 'A';
		int en = T[i] == 'A';
		cerr << st << "->" << en << '\n';
		for (int carry = 0; carry < 5; carry++) {
			if (dp[carry] == INF) continue;
			cerr << "  " << carry << '\n';
			for (int ops = 0; ops <= st + carry; ops += 2) {
				if ((st - carry - 3 * ops - en) & 1) continue;
				int nxt_carry = (en + carry + 3 * ops) / 2;
				if (nxt_carry < int(ndp.size())) {
					cerr << carry << ' ' << nxt_carry << ' ' << ops << '\n';
					setmin(ndp[nxt_carry], dp[carry] + ops);
				}
			}
		}
		dp = ndp;
		for (int i = 0; i < 5; i++) cerr << (dp[i] == INF ? -1 : dp[i]) << '\n';
	}
	int ans = dp[0];
	if (ans < INF) cout << ans << '\n';
	else cout << -1 << '\n';

	return 0;
}
