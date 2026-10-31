#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P; cin >> P;
	int N = 4;
	cout << fixed << setprecision(20);
	cout << N << '\n';
	for (int i = 0; i < P; i++) {
		double theta = 2 * M_PI / P * i + 2 * M_PI / P / 3;
		double x = cos(theta), y = sin(theta);
		cout << x << ' ' << -x << ' ' << y << ' ' << -y << '\n';
	}

	for (int i = 0; i < P; i++) {
		double x = abs(cos(2 * M_PI / P * i + 2 * M_PI / P / 3) + cos(2 * M_PI / P / 3));
		double y = abs(sin(2 * M_PI / P * i + 2 * M_PI / P / 3) + sin(2 * M_PI / P / 3));
		double norm = std::sqrt(x * x + y * y);
		x /= norm, y /= norm;
		cout << x << ' ' << x << ' ' << y << ' ' << y << '\n';
	}

	return 0;
}
