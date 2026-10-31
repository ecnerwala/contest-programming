#include<bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		std::vector<std::pair<int64_t, int64_t>> A; A.reserve(N);
		int num_ground = 0;
		int64_t max_ground = 0;
		for (int i = 0; i < N; i++) {
			int64_t a, b; cin >> a >> b;
			if (a == 0) {
				num_ground++;
				max_ground = std::max(max_ground, b * (N+1) + i + 1);
			} else {
				A.push_back({a * (N+1), b * (N+1) + i + 1});
			}
		}

		int num_segs = num_ground;
		int num_jumps = 0;

		int64_t X = max_ground, Y = max_ground;

		std::sort(A.begin(), A.end());
		auto it = A.begin();
		min_pq<int64_t> pq;
		while (true) {
			assert(X <= Y);
			int64_t h = (!pq.empty() && pq.top() < X) ? pq.top() : X;
			if (it != A.end() && it->first <= h) {
				pq.push(it->second);
				++it;
				continue;
			}

			if (!pq.empty()) {
				X = pq.top(); pq.pop();
				num_segs++;
				num_jumps++;
				if (X > Y) std::swap(X, Y);
			} else {
				if (X == Y) {
					break;
				} else {
					X = Y;
					num_jumps++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << num_segs << ' ' << num_jumps << '\n';
	}

	return 0;
}
