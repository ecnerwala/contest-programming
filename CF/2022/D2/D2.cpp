#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;
	int T;
	if (TESTING) {
		T = 100000;
	} else {
		cin >> T;
	}
	std::mt19937 mt(48);
	while (T--) {
		int N;
		std::vector<int> testing_ans;
		if (TESTING) {
			//testing_ans = {0, 1, 0, -1, 0, 1, 0};
			//testing_ans = {0, 1, -1, 0};
			testing_ans.resize(std::uniform_int_distribution(3, 20)(mt));
			for (auto& v : testing_ans) {
				v = std::uniform_int_distribution(0, 1)(mt);
			}
			testing_ans[std::uniform_int_distribution(0, int(testing_ans.size())-1)(mt)] = -1;
			N = int(testing_ans.size());
		} else {
			cin >> N;
			if (N == -1) exit(0);
		}
		const int N_orig = N;
		int num_queries = 0;
		auto get_parity = [&](int a, int b) -> int {
			assert(a != b);
			num_queries++;
			cout << '?' << ' ' << a+1 << ' ' << b+1 << '\n' << flush;
			int resp;
			if (TESTING) {
				if (testing_ans[a] == -1) resp = !testing_ans[b];
				else if (testing_ans[b] == -1) resp = testing_ans[a];
				else resp = testing_ans[a] ^ testing_ans[b] ^ 1;
			} else {
				cin >> resp;
				if (resp == -1) exit(0);
			}
			assert(resp == 0 || resp == 1);
			return !resp;
		};
		int ans = [&]() -> int {
			assert(N >= 3);
			if (N == 3) {
				if (get_parity(0, 1) == get_parity(1, 0)) return 2;
				else if (get_parity(0, 2) == get_parity(2, 0)) return 1;
				else return 0;
			} else if (N & 1) {
				assert(N >= 5);
				// first do a chunk of size 3
				int a = get_parity(N-3, N-2);
				int b = get_parity(N-2, N-1);
				int c = get_parity(N-1, N-3);
				if (a ^ b ^ c) {
					if (get_parity(N-2, N-3) == a) return N-1;
					else if (get_parity(N-1, N-2) == b) return N-3;
					else return N-2;
				} else {
					N -= 3;
				}
			}
			assert(N % 2 == 0);
			assert(N >= 2);
			while (N > 2) {
				assert(N >= 4);
				if (get_parity(N-2, N-1) != get_parity(N-1, N-2)) {
					if (get_parity(N-2, N-3) != get_parity(N-3, N-2)) return N-2;
					else return N-1;
				}
				N -= 2;
			}
			assert(N == 2);
			if (get_parity(1, 2) != get_parity(2, 1)) return 1;
			else return 0;
		}();
		if (TESTING) {
			assert(testing_ans[ans] == -1);
			cerr << "solved " << N_orig << " with " << num_queries << " queries" << '\n';
			assert(num_queries <= std::max(N_orig, 4));
		}
		cout << '!' << ' ' << ans+1 << '\n' << flush;
		// if first query has size k (k >= 2), it costs k + max(ceil(log_2(k)), f(N-k))
		// F(1) == 0
		// F(2) == 2
		// F(3) == 4
		// F(4) == 4
		// F(5) == 5
		// F(6) == 
	}

	return 0;
}
