#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> pq;
		for (int i = 0; i < N; i++) {
			pq.push(A[i]);
		}
		int64_t offset = 0;
		while (pq.top() - offset > 0) {
			int64_t a = pq.top() - offset; pq.pop();
			int64_t b = pq.top() - offset;
			int64_t d = b / a * a;
			offset += d;
			pq.push(a + offset);
		}
		cout << offset << '\n';
	}

	return 0;
}
