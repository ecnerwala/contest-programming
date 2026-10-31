#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t C; cin >> N >> C;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		if (C >= 0) {
			std::sort(A.begin(), A.end());
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			continue;
		}
		assert(C < 0);
		std::sort(A.begin(), A.end(), std::greater<>());
		std::vector<int> prv(N);
		std::vector<int> nxt(N);
		for (int i = 0; i < N; i++) {
			prv[i] = i-1;
			nxt[i] = i+1;
		}
		std::set<std::pair<int64_t, int>> bests;
		auto is_good = [&](int i) -> bool {
			return prv[i] != -1 && nxt[i] != N && A[prv[i]] + C <= A[nxt[i]];
		};
		for (int i = 1; i+1 < N; i++) {
			// i is good
			if (is_good(i)) bests.insert({A[i], i});
		}

		std::vector<int64_t> B; B.reserve(N);
		int head = 0;
		for (int i = 0; i < N; i++) {
			assert(prv[head] == -1);
			int to_erase = head;
			if (!B.empty()) {
				auto it = bests.lower_bound({B.back() + C, -1});
				if (it != bests.end()) {
					to_erase = it->second;
				}
			}
			B.push_back(A[to_erase]);
			bests.erase({A[to_erase], to_erase});
			int p = prv[to_erase];
			int n = nxt[to_erase];
			if (p != -1) {
				bests.erase({A[p], p});
				nxt[p] = n;
				if (is_good(p)) bests.insert({A[p], p});
			}
			if (n != N) {
				bests.erase({A[n], n});
				prv[n] = p;
				if (is_good(n)) bests.insert({A[n], n});
			}

			if (to_erase == head) {
				head = n;
			}
		}

		for (int i = 0; i < N; i++) {
			cout << B[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
