#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) {
	if (b > a) a = b;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		std::vector<int> left_parity(N);
		std::vector<int> has_mx(N);
		int cnt_good = 0;
		int tot_val = 0;
		auto update_inter = [&](int a, int w) -> void {
			assert(w == +1 || w == -1);
			assert(par[a] < 0);
			int l = -par[a];
			tot_val += w * (l + 1) / 2;
			if ((l & 1) ? (has_mx[a] & (1 << left_parity[a])) : has_mx[a]) {
				cnt_good += w;
			}
		};
		auto get_ans = [&]() -> int {
			return tot_val - !cnt_good;
		};
		auto merge = [&](int a, int b) -> void {
			a = get_par(a), b = get_par(b);
			assert(a != b);
			update_inter(a, -1);
			update_inter(b, -1);

			assert(a < b);
			left_parity[b] = left_parity[a];
			if (par[a] > par[b]) std::swap(a, b);
			has_mx[a] |= has_mx[b];
			par[a] += par[b];
			par[b] = a;
			update_inter(a, +1);
		};
		std::vector<std::pair<int, int>> evts; evts.reserve(N);
		for (int i = 0; i < N; i++) {
			evts.push_back({A[i], i});
		}
		std::sort(evts.begin(), evts.end(), std::greater<>());
		std::vector<bool> alive(N);
		int mx = *std::max_element(A.begin(), A.end());
		int64_t ans = 0;
		for (auto [v, i] : evts) {
			alive[i] = true;
			left_parity[i] = i & 1;
			if (v == mx) has_mx[i] = 1 << (i & 1);
			update_inter(i, +1);
			if (i-1 >= 0 && alive[i-1]) {
				merge(i-1, i);
			}
			if (i+1 < N && alive[i+1]) {
				merge(i, i+1);
			}
			int max_cnt = tot_val - !cnt_good;
			//cerr << v << ' ' << i << ' ' << max_cnt << '\n';
			setmax(ans, int64_t(mx) + int64_t(v) + int64_t(max_cnt));
		}
		cout << ans << '\n';
	}

	return 0;
}
