#include <bits/stdc++.h>

using std::cerr;

std::pair<int64_t, int64_t> solve_slow(std::vector<int> A, int W) {
	int N = int(A.size());
	std::set<std::vector<int>> seen; 
	std::vector<std::pair<std::vector<int>, int>> q;
	seen.insert(A);
	q.push_back({A, 0});
	std::pair<int64_t, int64_t> ans{0, 0};
	for (int z = 0; z < int(q.size()); z++) {
		const std::vector<int> cur = q[z].first;
		int d = q[z].second;
		ans = std::max(ans, std::pair<int64_t, int64_t>{std::accumulate(cur.begin(), cur.end(), int64_t(0)), -d});
		for (int i = 0; i+1 < N; i++) {
			if (cur[i] == cur[i+1]) {
				for (int x = 1; x <= W; x++) {
					for (int y = 1; y <= W; y++) {
						if (x == y) continue;
						std::vector<int> nxt = cur;
						nxt[i] = x;
						nxt[i+1] = y;
						if (seen.insert(nxt).second) {
							q.push_back({nxt, d+1});
						}
					}
				}
			}
		}
	}
	ans.second = -ans.second;
	return ans;
}

std::pair<int64_t, int64_t> solve(std::vector<int> A, int W) {
	assert(W >= 2);
	int N = int(A.size());
	bool has_move = false;
	for (int i = 0; i+1 < N; i++) {
		if (A[i] == A[i+1]) has_move = true;
	}
	if (!has_move) {
		return {std::accumulate(A.begin(), A.end(), int64_t(0)), 0};
	}
	assert(has_move);
	assert(N >= 2);
	if (W == 2) {
		assert(W == 2);
		// do the xor trick; then, take any two different ones and set one equal to the other
		int num_same = 0;
		int num_good_same = 0;
		num_same += (2 == A[0]);
		num_good_same += (A[0] == 2);
		for (int i = 0; i+1 < N; i++) {
			if (A[i] == A[i+1]) {
				num_same++;
				if (A[i] == 2) num_good_same++;
			}
		}
		int64_t ans = 0;
		for (int z = 0; z < num_same; z++) {
			ans += 2;
		}
		for (int z = num_same; z < N; z++) {
			ans += (z - num_same) % 2 == 0 ? 1 : 2;
		}

		return {ans, num_same - num_good_same};
	} else if (W >= 3) {
		int64_t cur_tot = std::accumulate(A.begin(), A.end(), int64_t(0));
		if (cur_tot >= int64_t(W) * N - 1) {
			return {cur_tot, 0};
		}
		return {int64_t(W) * N - 1, 1};
	} else assert(false);
}

void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 10000; z++) {
		int N = std::uniform_int_distribution(1, 6)(mt);
		int W = std::uniform_int_distribution(2, 2)(mt);
		std::vector<int> A(N);
		for (auto& a : A) a = std::uniform_int_distribution(1, W)(mt);
		cerr << "test " << z << ' ' << N << ' ' << W << '\n';
		auto ans = solve_slow(A, W);
		auto mine = solve(A, W);
		if (ans != mine) {
			cerr << N << ' ' << W << '\n';
			for (auto a : A) { cerr << a << ' '; } cerr << '\n';
			cerr << "ans " << ans.first << ' ' << ans.second << '\n';
			cerr << "mine " << mine.first << ' ' << mine.second << '\n';
		}
		assert(ans == mine);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	int T; cin >> T;
	while (T--) {
		int N, W; cin >> N >> W;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		auto [v, m] = solve(A, W);
		cout << v << ' ' << m << '\n';
	}

	return 0;
}
