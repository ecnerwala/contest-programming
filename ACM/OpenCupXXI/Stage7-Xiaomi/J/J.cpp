#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> L(N); for (auto& a : L) cin >> a;
	vector<int> W(N); for (auto& a : W) cin >> a;

	vector<int> tot_weight(1<<N);
	for (int i = 0; i < N; i++) {
		for (int m = 0; m < (1 << i); m++) {
			tot_weight[(1<<i)+m] = tot_weight[m] + W[i];
		}
	}

	vector<double> max_overhang(1<<N);
	for (int m = 0; m < (1 << N); m++) {
		for (int i = 0; i < N; i++) {
			if (m & (1 << i)) continue;
			setmax(max_overhang[m + (1 << i)], max_overhang[m] + (double(L[i]) * 0.5 * W[i] / (tot_weight[m] + W[i])));
			setmax(max_overhang[m + (1 << i)], double(L[i]) - (double(L[i]) * 0.5 * W[i] / (tot_weight[m] + W[i])));
		}
	}

	cout << fixed << setprecision(11) << max_overhang[(1<<N)-1] << '\n';

	return 0;
}
