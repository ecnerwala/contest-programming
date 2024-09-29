#include <bits/stdc++.h>

constexpr int64_t floor_sqrt(int64_t N) {
	assert(N >= 0);
	if (N == 0) return 0;
	int64_t a = N;
	while (true) {
		int64_t b = N/a;
		assert(a >= b);
		if (a-b <= 1) return b;
		a = (a+b+1)>>1;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t K; cin >> K;
		int64_t r = floor_sqrt(K);
		int64_t v = K + r;
		if (v >= (r+1) * (r+1)) v++;
		assert(v <= (r+2) * (r+2));
		cout << v << '\n';
	}

	return 0;
}
