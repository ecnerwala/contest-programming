#include <bits/stdc++.h>
#include <numeric>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N);
	for (auto& a : A) cin >> a, a--;

	vector<int> B(N);
	for (auto& b : B) cin >> b, b--;

	vector<int> invA(N);
	vector<int> invB(N);
	for (int i = 0; i < N; i++) {
		invA[A[i]] = i;
		invB[B[i]] = i;
	}

	vector<int64_t> slope(N+1);
	int64_t v0 = 0;
	for (int i = 0; i < N; i++) {
		// abs((invB[i] + x) % N - invA[i]);
		v0 += abs(invA[i] - invB[i]);
		int x_last = N - invB[i] - 1;
		int x_first = (N - invB[i]) % N;
		int x_mid = (invA[i] + N - invB[i]) % N;

		// x_first .. x_mid .. x_last
		if (x_first <= x_mid) {
			slope[x_first]--;
			slope[x_mid]++;
		} else {
			slope[x_first]--;
			slope[N]++;
			slope[0]--;
			slope[x_mid]++;
		}

		if (x_mid <= x_last) {
			slope[x_mid]++;
			slope[x_last]--;
		} else {
			slope[x_mid]++;
			slope[N]--;
			slope[0]++;
			slope[x_last]--;
		}

		int sk = (invA[i]) - (N-1 - invA[i]);
		slope[x_last] += sk;
		slope[x_last+1] -= sk;
	}

	for (int i = 0; i < N; i++) {
		slope[i+1] += slope[i];
	}
	assert(std::accumulate(slope.begin(), slope.end(), int64_t(0)) == 0);
	int64_t ans = v0;
	for (int i = 0; i < N; i++) {
		v0 += slope[i];
		ans = min(ans, v0);
	}
	cout << ans << '\n';

	return 0;
}
