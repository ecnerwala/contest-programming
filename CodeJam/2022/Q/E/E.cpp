#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		if (N == -1) exit(0);
		std::uniform_int_distribution<int> dist(1, N);

		std::vector<int> sizes(N, -1);
		{
			// This is somewhat useless since it's not random.
			int R, P; cin >> R >> P;
			if (R == -1) exit(0);
			R--;
			sizes[R] = P;
		}

		int64_t tot_p0 = 0;
		int64_t tot_p0_cnt = 0;
		for (int z = 0; z < K/2; z++) {
			cout << 'T' << ' ' << dist(mt) << '\n' << flush;
			int R0, P0; cin >> R0 >> P0;
			if (R0 == -1) exit(0);
			R0--;
			sizes[R0] = P0;
			tot_p0 += P0;
			tot_p0_cnt++;
			cout << 'W' << '\n' << flush;
			int R1, P1; cin >> R1 >> P1;
			if (R1 == -1) exit(0);
			R1--;
			sizes[R1] = P1;
		}

		int64_t ans = 0;
		int num_unknown = 0;
		for (int i = 0; i < N; i++) {
			if (sizes[i] != -1) ans += sizes[i];
			else num_unknown++;
		}
		ans = ans + int64_t(num_unknown * double(tot_p0) / double(tot_p0_cnt) + 0.5);
		ans /= 2;
		//cerr << tot_p0 << ' ' << tot_p0_cnt << ' ' << ans << '\n';
		cout << 'E' << ' ' << ans << '\n' << flush;
	}

	return 0;
}
