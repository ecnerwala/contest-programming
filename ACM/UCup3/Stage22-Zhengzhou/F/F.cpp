#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, Q; cin >> N >> K >> Q;

	int64_t tot_work = 0;
	struct work_t {
		int64_t a;
		int64_t b;
		int64_t l;
	};
	std::vector<work_t> A(N);
	for (int i = 0; i < N; i++) {
		int a, l; cin >> a >> l; a--;
		int64_t b = tot_work;
		A[i] = {a, b, l};
		tot_work += l;
	}

	std::vector<int64_t> pref_max(N+1);
	pref_max[0] = 0;
	for (int i = 0; i < N; i++) {
		pref_max[i+1] = std::max(pref_max[i], A[i].a - A[i].b);
	}

	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y; x--, y--;
		int64_t end_time = A[y].b + pref_max[y+1] + std::max<int64_t>(K, tot_work) * x + A[y].l - 1;
		int64_t d = end_time / K;
		int64_t h = end_time % K;
		cout << d+1 << ' ' << h+1 << '\n';
	}

	return 0;
}
