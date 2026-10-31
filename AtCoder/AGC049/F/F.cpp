#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<pair<int64_t, int64_t>> A(N);
	vector<int64_t> B(N);
	for (auto& a : A) cin >> a.first;
	for (auto& b : B) cin >> b;
	for (auto& a : A) cin >> a.second;

	sort(A.begin(), A.end());
	sort(B.begin(), B.end());

	using pt_t = pair<int64_t, int64_t>;
	vector<pt_t> pts(N+1);
	pts[0] = {0,0};
	for (int i = 0; i < N; i++) {
		pts[i+1] = pts[i];
		assert(60 % A[i].second == 0);
		pts[i+1].first += 60 / A[i].second;

		pts[i+1].second += B[i] - A[i].first;
	}

	vector<int> hull; hull.reserve(N+1);
	for (int i = 0; i <= N; i++) {
		while (int(hull.size()) >= 2) {
			int j = hull.back(); hull.pop_back();
			int k = hull.back();
			pt_t d0 = {pts[j].first - pts[k].first, pts[j].second - pts[k].second};
			pt_t d1 = {pts[i].first - pts[j].first, pts[i].second - pts[j].second};

			if (d1.second * d0.first > d0.second * d1.first) {
				continue;
			}
			hull.push_back(j); break;
		}
		hull.push_back(i);
	}

	int64_t ans = 0;
	for (int z = 0; z+1 < int(hull.size()); z++) {
		int lo = hull[z], hi = hull[z+1];
		//cerr << "Solve range " << lo << '-' << hi << '\n';
		int64_t k_num = pts[hi].second - pts[lo].second;
		int64_t k_den = pts[hi].first - pts[lo].first;

		int64_t tot_deficit = 0;
		vector<pair<int64_t, int64_t>> costs; costs.reserve(hi - lo);
		// now, we need to knapsack to find the right rounding
		for (int i = lo; i < hi; i++) {
			int64_t cur_num = k_num * (60 / A[i].second);
			int64_t d_lo = cur_num / k_den;
			int64_t def = cur_num % k_den;
			if (def < 0) def += k_den, d_lo--;
			assert(0 <= def && def < k_den);
			tot_deficit += def;

			ans += A[i].second * d_lo * d_lo;
			costs.push_back({A[i].first + d_lo, A[i].second * (2 * d_lo + 1)});
		}
		assert(tot_deficit % k_den == 0);
		tot_deficit /= k_den;

		priority_queue<int64_t, vector<int64_t>, greater<int64_t>> pq;
		int64_t cur_deficit = 0;
		for (int i = lo; i < hi; i++) {
			cur_deficit += costs[i-lo].first;
			pq.push(costs[i-lo].second);
			cur_deficit -= B[i];
			while (cur_deficit < 0) {
				assert(!pq.empty());
				ans += pq.top(); pq.pop();
				cur_deficit++;
			}
		}
		assert(cur_deficit == 0);
	}
	cout << ans << '\n';

	return 0;
}

// Claim: necessary and sufficient for
//   sum(biggest k A's) - sum(smallest N-k A's) <= sum(biggest k B's) - sum(smallest N-k B's)
// OR
//   sum(any k A's) - sum(any N-k A's) <= sum(biggest k B's) - sum(smallest N-k B's)
// This is a convex polytope/LP problem
// This is the convex hull of the permutations of B?
