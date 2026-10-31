#include <bits/stdc++.h>

template <typename T> T sq(T a) { return a * a; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, F, B; cin >> N >> F >> B;
	vector<vector<double>> V(N, vector<double>(F));
	for (int i = 0; i < N; i++) {
		for (int f = 0; f < F; f++) {
			cin >> V[i][f];
		}
	}

	vector<bool> flipped(N, false);

	vector<double> means(F);
	vector<double> var(F);
	vector<array<double, 2>> loss(N);
	for (int z = 0; z < 500; z++) {
		fill(means.begin(), means.end(), 0);
		fill(var.begin(), var.end(), 0);
		for (int i = 0; i < N; i++) {
			for (int f = 0; f < F; f++) {
				means[f] += V[i][f];
				var[f] += sq(V[i][f]);
			}
		}

		for (int f = 0; f < F; f++) {
			means[f] /= N;
			var[f] /= N;
			var[f] -= means[f] * means[f];
		}
		for (int i = 0; i < N; i++) {
			double loss_0 = 0;
			double loss_flip = 0;
			for (int f = 0; f < F; f++) {
				loss_0 += sq(V[i][f] - means[f]) / var[f];
				loss_flip += sq(V[i][F-f-1] - means[f]) / var[f];
			}
			if (loss_flip < loss_0) {
				flipped[i] = !flipped[i];
				reverse(V[i].begin(), V[i].end());
				swap(loss_flip, loss_0);
			}
			loss[i] = {loss_0, loss_flip};
		}
	}

	int cnt_flipped = 0;
	for (int i = 0; i < N; i++) {
		if (flipped[i]) {
			cnt_flipped++;
		}
	}
	if (cnt_flipped > N - cnt_flipped) {
		for (int i = 0; i < N; i++) {
			flipped[i] = !flipped[i];
		}
	}

	vector<pair<double, int>> should_flip(N);
	for (int i = 0; i < N; i++) {
		should_flip[i] = {flipped[i] ? loss[i][0] - loss[i][1] : loss[i][1] - loss[i][0], i};
	}

	sort(should_flip.begin(), should_flip.end());
	vector<int> ans(B);
	for (int i = 0; i < B; i++) {
		ans[i] = should_flip[i].second;
	}
	sort(ans.begin(), ans.end());
	for (int v : ans) cout << v+1 << '\n';

	return 0;
}
