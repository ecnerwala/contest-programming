#include <bits/stdc++.h>

// Minimize max_i | B_i / M - A_i / N |
// Minimize max_i | B_i * N - M * A_i |

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; cin >> K;
	int64_t N, M; cin >> N >> M;
	vector<int64_t> A(K); for (auto& a : A) cin >> a;
	vector<int64_t> B(K);

	vector<pair<int64_t, int>> incrs; incrs.reserve(K);
	vector<pair<int64_t, int>> decrs; decrs.reserve(K);

	for (int i = 0; i < K; i++) {
		B[i] = M * A[i] / N;
		int64_t d1 = M * A[i] - B[i] * N;
		int64_t d2 = (B[i] + 1) * N - M * A[i];
		if (d1 <= d2) {
			incrs.push_back({d2, i});
		} else {
			B[i]++;
			decrs.push_back({d1, i});
		}
	}

	int64_t cur_sum = std::accumulate(B.begin(), B.end(), int64_t(0));
	if (cur_sum > M) {
		// decrement it cur_sum - M times
		auto mid = decrs.begin() + (cur_sum - M);
		std::nth_element(decrs.begin(), mid, decrs.end());
		for (auto it = decrs.begin(); it != mid; ++it) {
			B[it->second]--;
		}
	} else {
		// increment it M - cur_sum
		auto mid = incrs.begin() + (M - cur_sum);
		std::nth_element(incrs.begin(), mid, incrs.end());
		for (auto it = incrs.begin(); it != mid; ++it) {
			B[it->second]++;
		}
	}

	for (int i = 0; i < K; i++) {
		cout << B[i] << " \n"[i+1==K];
	}

	return 0;
}
