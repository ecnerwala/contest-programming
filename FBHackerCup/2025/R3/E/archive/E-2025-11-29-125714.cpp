#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// brute force things of size 6
	std::vector<int> dist(1 << 6, -1);
	std::vector<std::array<std::vector<int>, 2>> nxt(1 << 6);
	std::vector<int> q; q.reserve(1 << 6);
	for (int i = 0; i <= 6; i++) {
		int cur = (1 << 6) - (1 << i);
		dist[cur] = 0;
		q.push_back(cur);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int msk = q[z];
		assert(dist[msk] != -1);
		for (int as = 0; as < 1 << 6; as++) {
			if (__builtin_popcount(as) != 3) continue;
			std::vector<int> S(6);
			for (int i = 0; i < 6; i++) S[i] = (msk >> i) & 1;
			std::vector<int> A; A.reserve(3);
			std::vector<int> B; B.reserve(3);
			for (int i = 0; i < 6; i++) (((as >> i) & 1) ? A : B).push_back(i);
			for (int i = 0; i < 3; i++) std::swap(S[A[i]], S[B[i]]);
			int nmsk = 0;
			for (int i = 0; i < 6; i++) {
				nmsk |= (S[i] << i);
			}
			if (dist[nmsk] == -1) {
				dist[nmsk] = dist[msk] + 1;
				nxt[nmsk] = {A, B};
				q.push_back(nmsk);
			}
		}
	}
	for (int msk = 0; msk < (1 << 6); msk++) { cerr << dist[msk] << ' '; } cerr << '\n';


	int T; cin >> T;
	//int T = (1 << (2*5));
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::string S; cin >> S;
		std::string S_orig = S;
		//int N = 5;
		//std::string S(2*N, '?');
		//for (int i = 0; i < 2*N; i++) S[i] = (((case_num-1) >> i) & 1) + '0';

		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';
		cerr << S << '\n';
		assert(N >= 3);
		assert(int(S.size()) == 2*N);

		std::vector<std::array<std::vector<int>, 2>> ops; ops.reserve(10*N);
		int done_lo = -1;
		int done_hi = -1;
		while (true) {
			cerr << S << '\n';
			int lo = 0;
			while (lo < 2*N && S[lo] == '0') lo++;
			int hi = 2*N;
			while (hi > 0 && S[hi-1] == '1') hi--;
			if (lo == hi) break;

			if (lo & 1) lo--;
			if (hi & 1) hi++;
			if (done_lo != -1) {
				assert(hi - lo <= 6);
				lo = done_lo;
				hi = done_hi;
			}
			while (hi - lo < 6) {
				if (lo > 0) lo -= 2;
				else if (hi < 2*N) hi += 2;
				else assert(false);
			}

			std::vector<int> A; A.reserve(N);
			std::vector<int> B; B.reserve(N);
			for (int z = 0; z < lo; z += 2) {
				assert(S[z] == '0' && S[z+1] == '0');
				A.push_back(z);
				B.push_back(z+1);
			}
			[&]{
				assert(hi - lo >= 6);
				if (hi - lo == 6) {
					done_lo = lo;
					done_hi = hi;
					int msk = 0;
					for (int i = 0; i < 6; i++) msk |= (S[lo + i] - '0') << i;
					assert(dist[msk] >= 1);
					for (auto v : nxt[msk][0]) A.push_back(lo + v);
					for (auto v : nxt[msk][1]) B.push_back(lo + v);
					return;
				}
				std::array<int, 2> cnt{0, 0};
				for (int i = lo; i < hi; i++) cnt[S[i] - '0']++;
				assert(hi - lo >= 4);
				{
					std::vector<int> tmp; tmp.reserve(2);
					for (int i = lo+2; tmp.size() < 2 && i < hi; i++) {
						if (S[i] == '0') tmp.push_back(i);
					}
					if (tmp.size() == 2 && tmp[0] <= lo + (hi-lo)/2 + 0 && tmp[1] <= lo + (hi-lo)/2 + 1) {
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
						return;
					}
				}
				{
					std::vector<int> tmp; tmp.reserve(2);
					for (int i = hi-3; tmp.size() < 2 && i >= lo; i--) {
						if (S[i] == '1') tmp.push_back(i);
					}
					std::reverse(tmp.begin(), tmp.end());
					if (tmp.size() == 2 && tmp[0] >= lo + (hi-lo)/2 - 2 && tmp[1] >= lo + (hi-lo)/2 - 1) {
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
						return;
					}
				}
				assert(cnt[0] == cnt[1]);
				assert(false);
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
		for (auto [A, B] : ops) {
			assert(int(A.size()) == N);
			assert(int(B.size()) == N);
			assert(std::ranges::is_sorted(A));
			assert(std::ranges::is_sorted(B));
			std::vector<bool> seen(2*N, false);
			for (int i = 0; i < N; i++) {
				assert(!seen[A[i]]);
				seen[A[i]] = true;
				assert(!seen[B[i]]);
				seen[B[i]] = true;
				std::swap(S_orig[A[i]], S_orig[B[i]]);
			}
		}
		assert(std::ranges::is_sorted(S_orig));
	}

	return 0;
}
