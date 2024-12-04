#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;
	int64_t a = 0, b = 0;
	while (b * (b+1) < N) b ++;
	if (b * (b+1) == N) {
		cout << 1 << '\n';
		exit(0);
	} else {
		while (b >= 0) {
			int64_t cnd = a * (a+1) + b * (b+1);
			if (cnd == N) {
				cout << 2 << '\n';
				exit(0);
			} else if (cnd > N) {
				b--;
			} else if (cnd < N) {
				a++;
			} else assert(false);
		}
	}
	cout << 3 << '\n';

	return 0;
}
