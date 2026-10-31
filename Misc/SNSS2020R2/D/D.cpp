#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct author {
		int idx;
		int64_t w;
		array<int64_t, 2> b;
	};
	vector<author> A(N);
	for (auto& a : A) {
		cin >> a.b[0] >> a.b[1] >> a.w;
	}
	for (int i = 0; i < N; i++) A[i].idx = i+1;
	sort(A.begin(), A.end(), [](const author& a, const author& b) { return a.w < b.w; });

	int64_t ST, EN; cin >> ST >> EN;
	set<array<int64_t, 2>> intervals({array<int64_t, 2>{ST, EN}});

	const int64_t INF = 1.1e18;
	auto lower_bound = [&](int64_t v) {
		auto it = intervals.lower_bound({v, -INF});
		if (it != intervals.begin()) {
			--it;
			auto o = *it;
			assert(o[0] < v);
			if (o[1] > v) {
				intervals.erase(it);
				intervals.insert({o[0], v});
				it = intervals.insert({v, o[1]}).first;
			} else {
				++it;
			}
		}
		return it;
	};

	assert(!intervals.empty());
	for (int i = 0; i < N; i++) {
		auto b = A[i].b;

		auto lo = lower_bound(b[0]);
		auto hi = lower_bound(b[1]);
		intervals.erase(lo, hi);

		if (intervals.empty()) {
			cout << i+1 << '\n';
			for (int j = 0; j <= i; j++) {
				cout << A[j].idx << " \n"[j==i];
			}
			exit(0);
		}
	}
	cout << -1 << '\n';
	exit(0);

	return 0;
}
