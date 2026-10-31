#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int tot_kernels = 0;
	for (int i = 0; i < 5; i++) {
		int a, l; cin >> a >> l;
		tot_kernels += a * l;
	}
	int avg_kernels = tot_kernels / 5;

	int n, kwf; cin >> n >> kwf;
	int ans = avg_kernels * n / kwf;
	cout << ans << '\n';

	return 0;
}
