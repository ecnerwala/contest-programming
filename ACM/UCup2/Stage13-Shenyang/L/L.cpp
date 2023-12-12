#include <bits/stdc++.h>

// Floor of log_2(a); index of highest 1-bit
inline int floor_log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int ceil_log_2(int a) {
	return a ? floor_log_2(2*a-1) : -1;
}

inline int next_pow_2(int a) {
	return 1 << ceil_log_2(a);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](int N, auto&& query) -> std::vector<std::string> {
		auto solve_empty_row = [&](int r_empty) -> std::vector<std::string> {
			cerr << "solving empty row " << r_empty << '\n';
			std::vector<int> resp(N);
			for (int b = 0; (1 << b) < N; b++) {
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
			return ans;
		};

		std::vector<std::string> Q0(N, std::string(N, '0'));
		for (int i = 1; i < N; i++) {
			Q0[i][0] = Q0[i][i] = '1';
		}
		auto R0 = query(Q0);
		if (R0[0][0] == '0') {
			return solve_empty_row(0);
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
				return solve_empty_row(i);
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
						if (R[i][j] == '1') {
							v = j;
						} else {
							v = 0;
						}
						if (resp[i] == -2) resp[i] = v-1;
						assert(resp[i] == v-1);
					}
				}
			}
		}
		std::vector<std::string> ans(N, std::string(N, '0'));
		for (int i = 0; i < N; i++) ans[i][resp[i]+1] = '1';
		return ans;
	};

	constexpr bool TESTING = true;

	std::mt19937 mt(48);

	int T;
	if (TESTING) {
		T = 100;
	} else {
		cin >> T;
	}
	while (T--) {
		int N;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(3, 10)(mt);
		} else {
			cin >> N;
		}
		std::vector<std::string> TESTING_ANS;
		if (TESTING) {
			TESTING_ANS = std::vector<std::string>(N, std::string(N, '0'));
			while (true) {
				double p = std::uniform_real_distribution<double>(0, 1)(mt);
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						TESTING_ANS[i][j] = std::bernoulli_distribution(p)(mt) ? '1' : '0';
					}
				}
				bool good = true;
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						bool found = false;
						for (int k = 0; k < N; k++) {
							if (TESTING_ANS[i][k] == '1' || TESTING_ANS[k][j] == '1') {
								found = true;
								break;
							}
						}
						if (!found) good = false;
					}
				}
				if (good) break;
			}
			cout << "N = " << N << '\n';
			cout << "ANSWER" << '\n';
			for (const auto& r : TESTING_ANS) cout <<r << '\n';
			cout << flush;
		}
		assert(N >= 3);
		const int max_queries = 2 + ceil_log_2(N);
		int num_queries = 0;
		auto query = [&](const std::vector<std::string>& g) -> std::vector<std::string> {
			num_queries++;
			assert(num_queries <= max_queries);
			assert(int(g.size()) == N);
			for (const auto& r : g) assert(int(r.size()) == N);
			cout << '?' << '\n';
			for (const auto& r : g) cout << r << '\n';
			cout << flush;
			if (TESTING) {
				std::vector<std::string> resp(N, std::string(N, '0'));
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						for (int k = 0; k < N; k++) {
							if (g[i][k] == g[i][j] && TESTING_ANS[i][k] == '1') resp[i][j] = '1';
							if (g[k][j] == g[i][j] && TESTING_ANS[k][j] == '1') resp[i][j] = '1';
						}
					}
				}
				cout << 0 << '\n';
				for (int i = 0; i < N; i++) {
					cout << resp[i] << '\n';
				}
				return resp;
			} else {
				int code; cin >> code;
				if (code != 0) exit(0);
				std::vector<std::string> resp(N);
				for (int i = 0; i < N; i++) {
					cin >> resp[i];
					assert(int(resp[i].size()) == N);
				}
				return resp;
			}
		};
		auto answer = [&](const std::vector<std::string>& g) -> void {
			assert(int(g.size()) == N);
			for (const auto& r : g) assert(int(r.size()) == N);
			cout << '!' << '\n';
			for (const auto& r : g) cout << r << '\n';
			cout << flush;
			if (TESTING) {
				int num_found = 0;
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						if (g[i][j] == '1') {
							assert(TESTING_ANS[i][j] == '1');
							num_found++;
						}
					}
				}
				assert(num_found >= N);
				int code = 0;
				cout << code << '\n';
				assert(code == 0);
			} else {
				int code; cin >> code;
				if (code != 0) exit(0);
			}
		};

		answer(solve(N, query));
	}

	return 0;
}
