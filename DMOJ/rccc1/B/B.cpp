#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int64_t> A(N); for (auto& a : A) cin >> a;

	unordered_map<int64_t, int> single_map;
	int64_t num_eq = 0;
	for (int i = 0; i < N; i++) {
		num_eq += single_map[A[i]]++;
	}

	int64_t ans = 0;
	vector<int64_t> vec; vec.reserve(N*(N-1)/2);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			vec.push_back(A[i]*A[j]);
		}
	}
	sort(vec.begin(), vec.end());
	int cnt = 0;
	int64_t last = -1;
	for (int64_t v : vec) {
		if (v != std::exchange(last, v)) cnt = 0;
		ans += cnt++;
	}

	// now we've counted all distinct pairs, but each pair may share exactly 1 item; we need to count triples of distinct i, {j, k} so that A[j] == A[k]
	ans -= num_eq * (N-2);

	ans *= 8;
	cout << ans << '\n';

	return 0;
}
