#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; int64_t C; cin >> N >> C;

		//cerr << "Case #" << case_num << ": " << N << ' ' << C << '\n';

		std::vector<std::pair<int, bool>> balls(N);
		for (auto& [x, s] : balls) {
			cin >> x >> s;
		}
		std::sort(balls.begin(), balls.end());

		int64_t ans = 0;

		for (int dir = 0; dir < 2; dir++) {
			//cerr << "dir " << dir << '\n';
			int64_t lo_off = 0;
			max_pq<int64_t> lo_slopes;

			std::optional<int64_t> mid_slope;

			int64_t hi_off = 0;
			min_pq<int64_t> hi_slopes;

			int64_t v0 = 0;

			auto dump_slopes = [&]() {
				std::vector<int64_t> slopes(lo_slopes.size() + hi_slopes.size() + bool(mid_slope));
				for (int i = int(lo_slopes.size())-1; i >= 0; i--) {
					slopes[i] = lo_slopes.top() + lo_off;
					lo_slopes.pop();
				}
				assert(lo_slopes.empty());
				if (mid_slope) {
					slopes[int(slopes.size()) / 2] = *mid_slope;
				}
				for (int i = int(slopes.size()) - int(hi_slopes.size()); i < int(slopes.size()); i++) {
					slopes[i] = hi_slopes.top() + hi_off;
					hi_slopes.pop();
				}
				assert(hi_slopes.empty());
				cerr << "slopes" << '\n';
				for (auto i : slopes) {
					cerr << i << ' ';
				}
				cerr << '\n';
				assert(std::is_sorted(slopes.begin(), slopes.end()));
				int64_t v = v0;
				cerr << v << ' ';
				for (auto i : slopes) {
					v += i;
					cerr << v << ' ';
				}
				cerr << '\n';
				for (int i = 0; i < int(slopes.size())/2; i++) {
					lo_slopes.push(slopes[i] - lo_off);
					hi_slopes.push(slopes[int(slopes.size()) - 1 - i] - hi_off);
				}
			};

			for (int i = 0; i < N && balls[i].first < 0; i++) {
				assert(lo_slopes.size() == hi_slopes.size());

				//dump_slopes();

				// process balls[i]
				int64_t cur_slope = balls[i].second ? C : -C;
				v0 += balls[i].second ? 0 : C;
				if (mid_slope) {
					if (cur_slope > *mid_slope) {
						hi_slopes.push(cur_slope - hi_off);
						lo_slopes.push(*mid_slope - lo_off);
						mid_slope = std::nullopt;
					} else {
						lo_slopes.push(cur_slope - lo_off);
						hi_slopes.push(*mid_slope - hi_off);
						mid_slope = std::nullopt;
					}
				} else if (i == 0) {
					mid_slope = cur_slope;
				} else {
					if (cur_slope >= hi_slopes.top() + hi_off) {
						hi_slopes.push(cur_slope - hi_off);
						mid_slope = hi_slopes.top() + hi_off;
						hi_slopes.pop();
					} else {
						lo_slopes.push(cur_slope - lo_off);
						mid_slope = lo_slopes.top() + lo_off;
						lo_slopes.pop();
					}
				}

				//dump_slopes();

				// advance by dist
				int64_t dist = (i + 1 == N ? 0 : std::min(0, balls[i+1].first)) - balls[i].first;
				// 2 trips
				dist *= 2;

				v0 += dist * (i+1);
				lo_off -= dist;
				hi_off += dist;

				//dump_slopes();
			}

			std::vector<int64_t> slopes(lo_slopes.size() + hi_slopes.size() + bool(mid_slope));
			for (int i = int(lo_slopes.size())-1; i >= 0; i--) {
				slopes[i] = lo_slopes.top() + lo_off;
				lo_slopes.pop();
			}
			if (mid_slope) {
				slopes[int(slopes.size()) / 2] = *mid_slope;
			}
			for (int i = int(slopes.size()) - int(hi_slopes.size()); i < int(slopes.size()); i++) {
				slopes[i] = hi_slopes.top() + hi_off;
				hi_slopes.pop();
			}

			int64_t cnd = v0;
			for (int64_t slope : slopes) {
				v0 += slope;
				cnd = std::min(cnd, v0);
			}
			ans += cnd;

			for (auto& [x, s] : balls) x = -x;
			std::reverse(balls.begin(), balls.end());
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
