#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int W, H, N; cin >> W >> H >> N; N++;

	double m = W * W / double(W*W - H*H);
	//double r1 = m + sqrt(m * m - 1);
	double r2 = m - sqrt(m * m - 1);
	// r1*r2 = 1

	vector<double> X(N+1);
	X[0] = 0;
	for (int k = 1; k <= N; k++) {
		X[k] = (1 - pow(r2, 2*k)) / (1 - pow(r2, 2*N)) * pow(r2, N-k) * W;
	}

	double ans = 0;
	for (int k = 0; k < N; k++) {
		double l = (X[k] + X[k+1]) / W * H + (X[k+1] - X[k]);
		ans += l * l / 4;
		ans -= (X[k] / W * H) * (X[k] / W * H) / 2;
	}

	cout << fixed << setprecision(6) << ans << '\n';
	for (int k = 1; k <= 10 && k < N; k++) {
		cout << fixed << setprecision(6) << X[k] << '\n';
	}

	return 0;
}
