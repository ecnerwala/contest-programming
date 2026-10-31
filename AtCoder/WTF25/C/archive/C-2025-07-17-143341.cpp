#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int64_t> pref_A(N+1);
		pref_A[0] = 0;
		for (int i = 0; i < N; i++) {
			pref_A[i+1] = pref_A[i] + A[i];
		}
		int64_t S = pref_A[N];
		assert(S % 2 == 0);
		int64_t tgt = S / 2;

		auto is_good = [&](int64_t d) -> bool {
			std::vector<int64_t> min_pos(N);
			std::vector<int64_t> max_pos(N);
			for (int i = 0; i < N; i++) {
				min_pos[i] = 0;
				max_pos[i] = A[i];
			}

			// just put some edge cases here
			min_pos[0] = A[0];
			max_pos[N-1] = 0;

			std::vector<int64_t> min_diff(N-1);
			for (int i = 0; i+1 < N; i++) {
				min_diff[i] = A[i+1] - d;
			}
			for (int i = 0; i+1 < N; i++) {
				setmax(min_pos[i+1], min_pos[i] + min_diff[i]);
			}
			for (int i = N-2; i >= 0; i--) {
				setmin(max_pos[i], max_pos[i+1] - min_diff[i]);
			}
			for (int i = 0; i < N; i++) {
				if (min_pos[i] > max_pos[i]) return false;
			}
			int64_t min_sum = std::accumulate(min_pos.begin(), min_pos.end(), int64_t(0));
			int64_t max_sum = std::accumulate(max_pos.begin(), max_pos.end(), int64_t(0));
			if (min_sum > tgt || max_sum < tgt) return false;
			assert(min_sum <= tgt && tgt <= max_sum);

			std::vector<int64_t> pos = min_pos;
			int64_t extra = tgt - min_sum;

			int64_t pref = 0;
			for (int i = 0; i < N; i++) {
				if (!(0 <= pos[i] && pos[i] <= A[i])) return false;
				if (i > 0 && pos[i-1] + (A[i] - pos[i]) > d) return false;
				pref -= A[i] - pos[i];
				if (pref < 0) return false;
				pref += pos[i];
			}
			if (pref != 0) return false;
			return true;
		};

		int64_t mi = -1;
		int64_t ma = 0;
		while (!is_good(ma)) {
			std::tie(mi, ma) = std::make_pair(ma, ma + (ma - mi) * 2);
		}
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
