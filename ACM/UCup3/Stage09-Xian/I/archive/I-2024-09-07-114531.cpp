#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q; cin >> N >> Q;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	const int V = int(1e6) + 10;
	std::vector<std::vector<int>> facs(V+1);
	for (int i = 1; i <= V; i++) {
		for (int j = i; j <= V; j += i) {
			facs[j].push_back(i);
		}
	}

	std::vector<std::vector<int>> inds(V+1);
	for (int i = 0; i < N; i++) {
		for (int v : facs[A[i]]) {
			inds[v].push_back(i);
		}
	}

	std::vector<std::vector<std::pair<int, int>>> evts(N);
	for (int v = 1; v <= V; v++) {
		{
			const auto& a = inds[v];
			for (int i = int(a.size())-3, j = int(a.size()); i >= 0; i--) {
				if (a[j-1] >= a[i+1] + (a[i+1] - a[i])) {
					while (a[j-1] >= a[i+1] + (a[i+1] - a[i])) {
						j--;
					}
					evts[a[i]].push_back({a[j], v});
				}
			}
		}
		inds[v] = {};
	}

	std::vector<std::vector<std::pair<int, int>>> queries(N);
	std::vector<int> ans(Q, -1);
	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r; l--, r--;
		queries[l].push_back({r, q});
	}
	const int BUCKET_SIZE = 150;
	std::vector<int> buckets((N-1) / BUCKET_SIZE + 1);
	std::vector<int> vals(N);
	for (int l = N-1; l >= 0; l--) {
		for (auto [r, v] : evts[l]) {
			setmax(buckets[r / BUCKET_SIZE], v);
			setmax(vals[r], v);
		}
		for (auto [r, q] : queries[l]) {
			int b = (r+1) / BUCKET_SIZE;
			int res = 0;
			for (int i = l / BUCKET_SIZE; i < b; i++) {
				setmax(res, buckets[i]);
			}
			for (int i = std::max(l, b * BUCKET_SIZE); i <= r; i++) {
				setmax(res, vals[i]);
			}
			ans[q] = res;
		}
	}
	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
