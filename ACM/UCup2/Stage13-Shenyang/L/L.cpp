#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		[&]() -> void {
			auto query = [&](const std::vector<std::string>& g) -> std::vector<std::string> {
				cout << '?' << '\n';
				for (const auto& r : g) cout << r << '\n';
				cout << flush;
				int code; cin >> code;
				if (code != 0) exit(0);
				std::vector<std::string> resp(N);
				for (int i = 0; i < N; i++) {
					cin >> resp[i];
					assert(int(resp[i].size()) == N);
				}
				return resp;
			};
			auto answer = [&](const std::vector<std::string>& g) -> void {
				cout << '!' << '\n';
				for (const auto& r : g) cout << r << '\n';
				cout << flush;
				int code; cin >> code;
				if (code != 0) exit(0);
			};

			auto solve_empty_row = [&](int r_empty) -> void {
				std::vector<int> resp(N);
				for (int b = 0; b < (1 << N); b++) {
					std::vector<std::string> Q(N, std::string(N, '0'));
					for (int i = 0; i < N; i++) {
						for (int j = 0; j < N; j++) {
							if (j % (1 << b) == resp[i] && (j & (1 << b))) Q[j][i] = '1';
						}
						Q[r_empty][i] = '1';
					}
					auto R = query(Q);
					for (int i = 0; i < N; i++) {
						if (R[r_empty][i] == '1') {
							resp[i] += (1 << b);
						}
					}
				}
				std::vector<std::string> ans(N, std::string(N, '0'));
				for (int i = 0; i < N; i++) {
					ans[resp[i]][i] = '1';
				}
				answer(ans);
			};

			std::vector<std::string> Q0(N, std::string(N, '0'));
			for (int i = 1; i < N; i++) {
				Q0[i][0] = Q0[i][1] = '1';
			}
			auto R0 = query(Q0);
			if (R0[0][0] == '0') {
				solve_empty_row(0);
				return;
			}

			std::vector<std::string> Q1(N, std::string(N, '0'));
			Q1[0][0] = '1';
			Q1[0][1] = '1';
			for (int i = 1; i < N; i++) {
				if (R0[i][i] == '1') {
					Q1[i][0] = '1';
				}
			}
			auto R1 = query(Q1);
			for (int i = 1; i < N; i++) {
				if (R0[i][i] == '0' && R1[i][i] == '0') {
					solve_empty_row(i);
					return;
				}
			}
			// Just fill in this fake answer so it's in the same place
			R0[0][0] = R1[0][1];
			std::vector<int> resp(N);
			for (int i = 0; i < N; i++) {
				if (R0[i][i] == '1') {
					resp[i] = -2;
				}
			}
			for (int b = 0; (1 << b) < N; b++) {
				std::vector<std::string> Q(N, std::string(N, '0'));
				for (int i = 0; i < N; i++) {
					if (R0[i][i] == '0') {
						// binary search
						Q[i][0] = '1';
						for (int j = 1; j < N; j++) {
							if ((j-1) % (1 << b) == resp[i] && ((j-1) & (1 << b))) {
								Q[i][j] = '1';
							}
						}
					} else {
						int j = (i == 0 ? 1 : i);
						bool can_check = false;
						if (!((j-1) & (1 << b))) {
							if (j == 1) {
								assert(i == 0 || i == 1);
								if (i == (b & 1)) {
									can_check = true;
								}
							} else {
								can_check = true;
							}
						}
						if (can_check) {
							Q[i][j] = '1';
						}
					}
				}
				auto R = query(Q);
				for (int i = 0; i < N; i++) {
					if (R0[i][i] == '0') {
						if (R[i][0] == '1') {
							resp[i] += (1 << b);
						}
					} else {
						int j = (i == 0 ? 1 : i);
						if (Q[i][j] == '1') {
							int v;
							if (R0[i][j] == '1') {
								v = j;
							} else {
								v = -1;
							}
							if (resp[i] == -2) resp[i] = v;
							assert(resp[i] == v);
						}
					}
				}
			}
			std::vector<std::string> ans(N, std::string(N, '0'));
			for (int i = 0; i < N; i++) ans[i][resp[i]+1] = '1';
			answer(ans);
		}();
	}

	return 0;
}
