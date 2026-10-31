#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		array<int, 2> A; cin >> A[0] >> A[1];

		vector<array<int, 20>> par(N);
		vector<int> depth(N);
		for (int i = 1; i < N; i++) {
			cin >> par[i][0]; par[i][0]--;
			depth[i] = depth[par[i][0]]+1;
			for (int z = 0; par[i][z]; z++) {
				par[i][z+1] = par[par[i][z]][z];
			}
		}

		double ans = 0;
		vector<array<int, 2>> cnt(N);
		for (int i = N-1; i >= 0; i--) {
			for (int z = 0; z < 2; z++) {
				cnt[i][z]++;
				if (depth[i] < A[z]) continue;
				int cur = i;
				for (int l = 0, v = A[z]; v; v >>= 1, l++) {
					if (v & 1) cur = par[cur][l];
				}
				cnt[cur][z] += cnt[i][z];
			}
			double va = double(cnt[i][0]) / double(N);
			double vb = double(cnt[i][1]) / double(N);
			ans += va + vb - va * vb;
		}

		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(10) << ans << '\n';
	}

	return 0;
}
