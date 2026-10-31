#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> arr(2*(1 << N));
	for (auto& a : arr) cin >> a;

	int64_t tot_cost = 0;
	std::vector<int> buf(2 * (1 << N));
	for (int b = 0; b < N; b++) {
		for (int i = 0; i < (1 << (N-b-1)); i++) {

			int s0 = (2*i+0) << (b+1);
			int e0 = s0 + (1 << b);
			int s1 = (2*i+1) << (b+1);
			int e1 = s1 + (1 << b);
			if (arr[e0] > arr[e1]) {
				std::swap(s0, s1);
				std::swap(e0, e1);
			}
			assert(arr[e0] <= arr[e1]);
			tot_cost += arr[e1] - arr[e0];
			auto buf_en = std::merge(arr.begin() + s0, arr.begin() + e0 + 1, arr.begin() + s1, arr.begin() + e1, buf.begin());
			assert(buf_en == buf.begin() + (1 << (b+1)) + 1);
			tot_cost -= arr[e1] - *buf_en;
			*buf_en = arr[e1];
			int sout = (2*i+0) << (b+1);
			std::copy(buf.begin(), buf_en, arr.begin() + sout);
		}
	}
	cout << tot_cost << '\n';

	return 0;
}
