#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::string S; cin >> S;
		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';
		cerr << S << '\n';

		std::vector<std::array<std::vector<int>, 2>> ops; ops.reserve(10*N);
		while (true) {
			cerr << S << '\n';
			int lo = 0;
			while (lo < 2*N && S[lo] == '0') lo++;
			int hi = 2*N;
			while (hi >= 0 && S[hi-1] == '1') hi--;
			if (lo == hi) break;

			if (lo & 1) lo--;
			if (hi & 1) hi++;

			std::vector<int> A; A.reserve(N);
			std::vector<int> B; B.reserve(N);
			for (int z = 0; z < lo; z += 2) {
				assert(S[z] == '0' && S[z+1] == '0');
				A.push_back(z);
				B.push_back(z+1);
			}
			[&]{
				std::array<int, 2> cnt{0, 0};
				for (int i = lo; i < hi; i++) cnt[S[i] - '0']++;
				assert(hi - lo >= 4);
				if (cnt[0] > cnt[1]) {
					std::vector<int> tmp; tmp.reserve(2);
					for (int i = lo+2; tmp.size() < 2 && i < hi; i++) {
						if (S[i] == '0') tmp.push_back(i);
					}
					assert(tmp.size() == 2);
					A.push_back(lo+0);
					B.push_back(tmp[0]);
					A.push_back(lo+1);
					B.push_back(tmp[1]);
					int cur_cnt = 0;
					for (int i = lo; i < hi; i++) {
						if (i == lo || i == lo+1 || i == tmp[0] || i == tmp[1]) continue;
						if (cur_cnt < int(hi-lo-4) / 2) A.push_back(i);
						else B.push_back(i);
						cur_cnt++;
					}
					assert(cur_cnt == (hi - lo - 4));
				} else if (cnt[1] > cnt[0]) {
					std::vector<int> tmp; tmp.reserve(2);
					for (int i = hi-3; tmp.size() < 2 && i >= lo; i--) {
						if (S[i] == '1') tmp.push_back(i);
					}
					assert(tmp.size() == 2);
					int cur_cnt = 0;
					for (int i = lo; i < hi; i++) {
						if (i == hi-2 || i == hi-1 || i == tmp[0] || i == tmp[1]) continue;
						if (cur_cnt < int(hi-lo-4) / 2) A.push_back(i);
						else B.push_back(i);
						cur_cnt++;
					}
					assert(cur_cnt == (hi - lo - 4));
					A.push_back(tmp[0]);
					B.push_back(hi-2);
					A.push_back(tmp[1]);
					B.push_back(hi-1);
				} else {
					assert(cnt[0] == cnt[1]);
					// todo
					assert(false);
				}
			}();
			for (int z = hi; z < 2*N; z += 2) {
				assert(S[z] == '1' && S[z+1] == '1');
				A.push_back(z);
				B.push_back(z+1);
			}
			ops.push_back({A, B});
			assert(int(A.size()) == N);
			assert(std::is_sorted(A.begin(), A.end()));
			assert(int(B.size()) == N);
			assert(std::is_sorted(B.begin(), B.end()));
			for (int i = 0; i < N; i++) {
				std::swap(S[A[i]], S[B[i]]);
			}
		}

		cout << "Case #" << case_num << ": " << ops.size() << '\n';
		for (const auto& op : ops) {
			for (const auto& a : op) {
				for (int z = 0; z < N; z++) {
					cout << a[z]+1 << " \n"[z+1==N];
				}
			}
		}
		cout << flush;
		assert(int(ops.size()) <= 10*N);
		assert(std::is_sorted(S.begin(), S.end()));
	}

	return 0;
}
