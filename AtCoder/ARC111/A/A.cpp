#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, M; cin >> N >> M;
	int64_t r = 1;
	int64_t a = 10;
	while (N) {
		if (N & 1) r = r * a % (M*M);
		a = a * a % (M*M);
		N >>= 1;
	}
	cout << r/M << '\n';
	return 0;
}
