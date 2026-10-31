#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int INF = 1e9;

	int N; cin >> N;
	vector<int> C(N); for (auto& c : C) cin >> c;

	const int MAXV = 5010;
	vector<int> dp(MAXV, INF);
	dp[0] = 0;
	vector<int> ndp(MAXV);
	for (int i = 0; i < N; i++) {
		int a, b; cin >> a >> b; int c = C[i];
		fill(ndp.begin(), ndp.end(), INF);

		for (int v = 0; v < MAXV; v++) {
			int num_need = (max(0, c-v) + a-1) / a;
			setmin(ndp[min(MAXV-1, num_need * b)], dp[v] + num_need);
		}

		for (int v = 0; v < MAXV; v++) {
			setmin(ndp[min(MAXV-1, v + b)], ndp[v] + 1);
		}

		swap(dp, ndp);
	}
	cout << *min_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
