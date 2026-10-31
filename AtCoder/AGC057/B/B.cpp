#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int64_t X; cin >> X;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	const int64_t INF = 2e18;

	std::sort(A.begin(), A.end());
	// upper then lower bound
	min_pq<std::pair<int64_t, int64_t>> pq;
	std::multiset<int64_t> lower_bounds;
	for (int i = 0; i < N; i++) {
		int64_t lo = A[i], hi = A[i];
		while (hi * 2 + X <= A[N-1]) {
			lo *= 2;
			lo = std::min(lo, INF);
			hi *= 2, hi += X;
			hi = std::min(hi, INF);
		}
		pq.push({hi, lo});
		//cerr << "insert " << lo << ' ' << hi << '\n';
		lower_bounds.insert(lo);
	}

	//cerr << "val " <<  *lower_bounds.rbegin() - pq.top().first << '\n';
	int64_t ans = *lower_bounds.rbegin() - pq.top().first;

	for (int z = 0; z < 4*N; z++) {
		auto [hi, lo] = pq.top(); pq.pop();
		lower_bounds.erase(lower_bounds.find(lo));
		//cerr << "delete " << lo << ' ' << hi << '\n';

		lo *= 2;
		lo = std::min(lo, INF);
		hi *= 2, hi += X;
		hi = std::min(hi, INF);

		pq.push({hi, lo});
		lower_bounds.insert(lo);
		//cerr << "insert " << lo << ' ' << hi << '\n';
		//cerr << "val " <<  *lower_bounds.rbegin() - pq.top().first << '\n';
		ans = std::min(ans, *lower_bounds.rbegin() - pq.top().first);
	}

	if (ans < X) ans = 0;
	cout << ans << '\n';

	return 0;
}
