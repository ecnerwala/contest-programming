#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

const int MAXN = 600;
struct grid_t {
	int CX[MAXN];
	int CY[MAXN];
	int W[MAXN][MAXN];
};
std::array<grid_t, 2> grids;
std::array<int, 3> dp[MAXN][MAXN];

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	for (int i = 0; i < N; i++) {
		int x; cin >> x;
		grids[0].CX[i] = x;
		grids[1].CY[i] = x;
	}
	for (int i = 0; i < N; i++) {
		int x; cin >> x;
		grids[0].CY[i] = x;
		grids[1].CX[i] = x;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int x; cin >> x;
			grids[0].W[i][j] = x;
			grids[1].W[j][i] = x;
		}
	}

	std::vector<std::pair<std::array<std::array<int, 2>, 2>, int>> all_queries(Q);
	for (auto& q : all_queries) {
		for (auto& r : q.first) for (auto& x : r) {
			cin >> x; x--;
		}
	}
	for (int q = 0; q < Q; q++) all_queries[q].second = q;

	std::vector<int> ans(Q, -1);

	const int INF = int(1e9) + 20;

	std::y_combinator([&](auto self, bool transpose, std::array<std::array<int, 2>, 2> bounds, std::vector<std::pair<std::array<std::array<int, 2>, 2>, int>> queries) -> void {
		if (queries.empty()) return;

		if (bounds[0][1] - bounds[0][0] < bounds[1][1] - bounds[1][0]) {
			transpose = !transpose;
			std::swap(bounds[0], bounds[1]);
			for (auto& q : queries) std::swap(q.first[0], q.first[1]);
		}
		const auto& gs = grids[transpose];
		assert(bounds[0][1] - bounds[0][0] >= bounds[1][1] - bounds[1][0]);
		if (bounds[0][1] - bounds[0][0] == 0) {
			assert(bounds[1][1] - bounds[1][0] == 0);
			int best = max(0, gs.W[bounds[0][0]][bounds[1][0]] - gs.CX[bounds[0][0]] - gs.CY[bounds[1][0]]);
			for (auto& q : queries) {
				assert(q.first == bounds);
				ans[q.second] = best;
			}
			return;
		}

		int mlo = (bounds[0][0] + bounds[0][1]) / 2;
		int mhi = mlo + 1;
		assert(bounds[0][0] <= mlo && mlo + 1 == mhi && mhi <= bounds[0][1]);

		{
			std::vector<std::pair<std::array<std::array<int, 2>, 2>, int>> lqueries, mqueries, rqueries;
			lqueries.reserve(queries.size());
			mqueries.reserve(queries.size());
			rqueries.reserve(queries.size());
			for (auto q : queries) {
				if (q.first[0][0] >= mhi) rqueries.push_back(q);
				else if (q.first[0][1] <= mlo) lqueries.push_back(q);
				else mqueries.push_back(q);
			}
			queries = std::move(mqueries);
			mqueries = {};
			self(transpose, std::array<std::array<int, 2>, 2>{std::array<int, 2>{bounds[0][0], mlo}, bounds[1]}, std::move(lqueries));
			self(transpose, std::array<std::array<int, 2>, 2>{std::array<int, 2>{mhi, bounds[0][1]}, bounds[1]}, std::move(rqueries));
		}

		if (queries.empty()) return;

		/*
		cerr << "start" << bounds[0][0] << '-' << bounds[0][1] << 'x' << bounds[1][0] << '-' << bounds[1][1] << ' ' << transpose << '\n';
		for (int x = bounds[0][0]; x <= bounds[0][1]; x++) { cerr << gs.CX[x] << ' '; } cerr << '\n';
		for (int y = bounds[1][0]; y <= bounds[1][1]; y++) { cerr << gs.CY[y] << ' '; } cerr << '\n';
		cerr << '\n';
		for (int x = bounds[0][0]; x <= bounds[0][1]; x++) {
			for (int y = bounds[1][0]; y <= bounds[1][1]; y++) { cerr << gs.W[{x,y}] << ' '; } cerr << '\n';
		}
		cerr << '\n';
		*/

		// solve yourself
		for (int y_cur = bounds[1][0]; y_cur <= bounds[1][1]; y_cur++) {
			for (int use = 0; use < 2; use++) {
				/*
				for (int x = bounds[0][0]; x <= bounds[0][1]; x++) {
					for (int y = bounds[1][0]; y <= bounds[1][1]; y++) {
						dp[{x, y}] = {-INF,-INF,-INF};
					}
				}
				*/
				for (int y = y_cur; y <= bounds[1][1]; y++) {
					dp[mhi][y] = {-INF,-INF,-INF};
				}
				if (use) {
					dp[mhi][y_cur] = {0, max(0, gs.W[mhi][y_cur]) - gs.CX[mhi], max(0, gs.W[mhi][y_cur] - gs.CX[mhi])};
				} else {
					dp[mhi][y_cur] = {0, -gs.CX[mhi], -INF};
				}
				for (int x = mhi; x <= bounds[0][1]; x++) {
					for (int y = y_cur; y <= bounds[1][1]; y++) {
						setmax(dp[x][y][0], dp[x][y][1]);
						setmax(dp[x][y][0], dp[x][y][2]);
						if (y+1 <= bounds[1][1]) {
							setmax(dp[x][y+1][0], dp[x][y][0]);
							setmax(dp[x][y+1][1], dp[x][y][1] + max(0, gs.W[x][y+1] - gs.CY[y+1]));
							setmax(dp[x][y+1][2], max(dp[x][y][0], dp[x][y][1] + gs.W[x][y+1]) - gs.CY[y+1]);
						}
						if (x+1 <= bounds[0][1]) {
							dp[x+1][y][0] = dp[x][y][0];
							dp[x+1][y][1] = max(dp[x][y][0], dp[x][y][2] + gs.W[x+1][y]) - gs.CX[x+1];
							dp[x+1][y][2] = dp[x][y][2] + max(0, gs.W[x+1][y] - gs.CX[x+1]);
						}
					}
				}
				for (int y = y_cur; y >= bounds[1][0]; y--) {
					dp[mlo][y] = {-INF,-INF,-INF};
				}
				if (use) {
					dp[mlo][y_cur] = {0, max(0, gs.W[mlo][y_cur]) - gs.CX[mlo], max(0, gs.W[mlo][y_cur] - gs.CX[mlo])};
				} else {
					dp[mlo][y_cur] = {0, -gs.CX[mlo], -INF};
				}
				for (int x = mlo; x >= bounds[0][0]; x--) {
					for (int y = y_cur; y >= bounds[1][0]; y--) {
						setmax(dp[x][y][0], dp[x][y][1]);
						setmax(dp[x][y][0], dp[x][y][2]);
						if (y-1 >= bounds[1][0]) {
							setmax(dp[x][y-1][0], dp[x][y][0]);
							setmax(dp[x][y-1][1], dp[x][y][1] + max(0, gs.W[x][y-1] - gs.CY[y-1]));
							setmax(dp[x][y-1][2], max(dp[x][y][0], dp[x][y][1] + gs.W[x][y-1]) - gs.CY[y-1]);
						}
						if (x-1 >= bounds[0][0]) {
							dp[x-1][y][0] = dp[x][y][0];
							dp[x-1][y][1] = max(dp[x][y][0], dp[x][y][2] + gs.W[x-1][y]) - gs.CX[x-1];
							dp[x-1][y][2] = dp[x][y][2] + max(0, gs.W[x-1][y] - gs.CX[x-1]);
						}
					}
				}
				//cerr << "cross " << mlo << '-' << mhi << " at " << y_cur << ' ' << "use " << use << '\n';
				for (auto q : queries) {
					//cerr << "query " << q.first[0][0] << '-' << q.first[0][1] << 'x' << q.first[1][0] << '-' << q.first[1][1] << ' ' << q.second << '\n';
					if (q.first[1][0] <= y_cur && y_cur <= q.first[1][1]) {
						int cnd = dp[q.first[0][0]][q.first[1][0]][0] + dp[q.first[0][1]][q.first[1][1]][0];
						if (use > 0) {
							cnd -= gs.CY[y_cur];
						}
						//cerr << cnd << '\n';
						setmax(ans[q.second], cnd);
					}
				}
			}
		}
	})(false, std::array<std::array<int, 2>, 2>{std::array<int, 2>{0, N-1}, {0, N-1}}, std::move(all_queries));

	for (int x : ans) {
		cout << x << '\n';
	}

	return 0;
}
