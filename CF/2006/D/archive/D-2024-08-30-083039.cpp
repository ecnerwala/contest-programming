#include <bits/stdc++.h>

inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
	if (pow == 0) {
		return 0;
	}
	int hpow = 1 << (pow-1);
	int seg = (x < hpow) ? (
		(y < hpow) ? 0 : 3
	) : (
		(y < hpow) ? 1 : 2
	);
	seg = (seg + rotate) & 3;
	const int rotateDelta[4] = {3, 0, 0, 1};
	int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
	int nrot = (rotate + rotateDelta[seg]) & 3;
	int64_t subSquareSize = int64_t(1) << (2*pow - 2);
	int64_t ans = seg * subSquareSize;
	int64_t add = hilbertOrder(nx, ny, pow-1, nrot);
	ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q, K; cin >> N >> Q >> K;
		int RT = 0;
		while ((RT+1) * (RT+1) <= K) RT++;
		std::vector<int> B(N);
		for (auto& b : B) {
			cin >> b;
			if (b > RT) {
				// negative numbers are needs
				b = -(K / b);
			}
		}

		struct query_t {
			int64_t H;
			int l, r;
			int q;
		};
		std::vector<query_t> Qs(Q);
		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--;
			Qs[q] = {hilbertOrder(l, r, 20, 0), l, r, q};
		}
		std::sort(Qs.begin(), Qs.end(), [&](auto a, auto b) { return a.H < b.H; });

		std::vector<int> answers(Q);

		std::vector<int> freq_small(RT+1);
		std::vector<int> freq_big(RT+1);
		freq_small[0]++;
		int l = 0;
		int r = 0;
		for (auto query : Qs) {
			while (l > query.l) {
				l--;
				if (B[l] > 0) freq_small[B[l]]++;
				else freq_big[-B[l]]++;
			}
			while (r < query.r) {
				if (B[r] > 0) freq_small[B[r]]++;
				else freq_big[-B[r]]++;
				r++;
			}
			while (l < query.l) {
				if (B[l] > 0) freq_small[B[l]]--;
				else freq_big[-B[l]]--;
				l++;
			}
			while (r > query.r) {
				r--;
				if (B[r] > 0) freq_small[B[r]]--;
				else freq_big[-B[r]]--;
			}

			// now answer the query
			{
				cerr << "query " << query.q << " l=" << query.l << " r=" << query.r << '\n';
				for (int i = 1; i <= RT; i++) { cerr << ' ' << freq_big[i]; } cerr << '\n';
				for (int i = 1; i <= RT; i++) { cerr << ' ' << freq_small[i]; } cerr << '\n';
				// we need to match all the big to all the small
				int ans = 0;

				int tot_need = 0;
				for (int i = 1; i <= RT; i++) {
					tot_need += freq_big[i];
				}
				int need = 0;
				int have = 0;
				for (int i = 1; i <= RT; i++) {
					need += freq_big[i];
					have += freq_small[i];
					ans = std::max(ans, (need - have + (tot_need > need) + 1) / 2);
				}
				answers[query.q] = ans;
			}
		}
		for (int q = 0; q < Q; q++) {
			cout << answers[q] << " \n"[q+1==Q];
		}
	}

	return 0;
}
