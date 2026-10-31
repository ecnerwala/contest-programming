#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int K, P, Q; cin >> K >> P >> Q; P--, Q--;
	std::vector<int> cnt(K); for (int& c : cnt) cin >> c;
	int N = std::accumulate(cnt.begin(), cnt.end(), 0);

	std::vector<int> ans(N, -1);
	auto go = [&]() -> bool {
		if (cnt[P] < 1) return false;
		if (cnt[Q] < 1) return false;

		if (N == 1) {
			if (P == Q) {
				ans[0] = P;
				return true;
			} else {
				return false;
			}
		}

		if (P == Q && cnt[P] < 2) return false;

		if (N % 2 == 0) {
			int idx = -2;
			auto get_idx = [&]() -> int {
				idx += 2;
				if (idx == N) idx = 1;
				assert(idx != N+1);
				return idx;
			};
			auto add_cnt = [&](int v, int c) {
				assert(c >= 0);
				while (c--) ans[get_idx()] = v;
			};
			if (P == Q) {
				add_cnt(P, cnt[P]-1);
				for (int k = 0; k < K; k++) {
					if (k == P) continue;
					add_cnt(k, cnt[k]);
				}
				add_cnt(P, 1);
			} else {
				add_cnt(P, cnt[P]);
				for (int k = 0; k < K; k++) {
					if (k == P || k == Q) continue;
					add_cnt(k, cnt[k]);
				}
				add_cnt(Q, cnt[Q]);
			}
			assert(idx == N-1);
		} else {
			ans.front() = P, ans.back() = Q;

			int idx = -1;
			auto get_idx = [&]() -> int {
				idx += 2;
				if (idx == N) idx = 2;
				//assert(idx != N-1);
				return idx;
			};
			auto add_cnt = [&](int v, int c) {
				assert(c >= 0);
				while (c--) ans[get_idx()] = v;
			};

			int big_one = -1;
			for (int k = 0; k < K; k++) {
				if (k == P || k == Q) continue;
				if (cnt[k] >= (N - 1) / 2) {
					// this is the biggest guy
					big_one = k;
					break;
				}
			}

			if (big_one != -1) {
				add_cnt(big_one, cnt[big_one]);
			}
			for (int k = 0; k < K; k++) {
				if (k == P || k == Q || k == big_one) continue;
				add_cnt(k, cnt[k]);
			}
			if (P == Q) {
				add_cnt(P, cnt[P] - 2);
			} else {
				add_cnt(P, cnt[P] - 1);
				add_cnt(Q, cnt[Q] - 1);
			}

			assert(idx == (N == 3 ? 1 : N-3));
		}

		if (ans.front() != P && ans.back() != Q) return false;
		for (int v : ans) {
			if (v == -1) return false;
			--cnt[v];
		}
		for (int c : cnt) if (c != 0) return false;
		for (int i = 0; i+1 < N; i++) if (ans[i] == ans[i+1]) return false;
		return true;
	};

	if (go()) {
		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i+1==N];
		}
	} else {
		cout << 0 << '\n';
	}

	return 0;
}
