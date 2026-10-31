#include<bits/stdc++.h>

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
	assert(0 <= x && x < (1 << pow));
	assert(0 <= y && y < (1 << pow));
	if (pow == 0) {
		return 0;
	}
	int hpow = 1 << (pow-1);
	int seg = (x < hpow) ? (
		(y < hpow) ? 0 : 3
	) : (
		(y < hpow) ? 1 : 2
	);
	seg = (seg + rotate) & 3;
	const int rotateDelta[4] = {3, 0, 0, 1};
	int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
	int nrot = (rotate + rotateDelta[seg]) & 3;
	int64_t subSquareSize = int64_t(1) << (2*pow - 2);
	int64_t ans = seg * subSquareSize;
	int64_t add = hilbertOrder(nx, ny, pow-1, nrot);
	ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<std::string> W(N);
		for (auto& w : W) {
			cin >> w;
			std::reverse(w.begin(), w.end());
		}
		int Q; cin >> Q;
		struct query_t {
			int k, l, r;
		};
		std::vector<query_t> queries(Q);
		for (auto& [k, l, r] : queries) {
			cin >> l >> r >> k; l--;
		}
		std::sort(queries.begin(), queries.end(), [&](auto a, auto b) -> bool {
			return a.k < b.k;
		});
		cerr << "Case #" << case_num << " " << "N = " << N << ' ' << "Q = " << Q << "\n";

		struct str_t {
			int idx;
			int val;
		};
		std::vector<str_t> cur_strings(N);
		int V = 1;
		for (int i = 0; i < N; i++) {
			cur_strings[i] = {i, 0};
		}

		std::vector<std::pair<int64_t, std::array<int, 2>>> cur_queries; cur_queries.reserve(Q);
		int q_it = 0;

		std::vector<int> freq(N, 0);

		// number of things with freq > i
		std::vector<int> freq_freq(N+1, 0);
		// freq_freq[N-1] = 0

		// num pts at distance N+1
		std::vector<int> num_pts(N+1);

		std::vector<int> depth_val(N+1);
		int cur_ans = 0;

		auto insert_pt = [&](int a) -> void {
			//cerr << "insert pt " << a << '\n';
			int depth = freq_freq[freq[a]] + freq[a];
			depth_val[depth]++;
			freq_freq[freq[a]]++;
			freq[a]++;
		};
		auto delete_pt = [&](int a) -> void {
			//cerr << "delete pt " << a << '\n';
			freq[a]--;
			assert(freq[a] >= 0);
			freq_freq[freq[a]]--;
			assert(freq_freq[freq[a]] >= 0);
			int depth = freq_freq[freq[a]] + freq[a];
			assert(depth_val[depth] > 0);
			depth_val[depth]--;
		};
		auto get_ans = [&]() -> int {
			while (cur_ans > 0 && depth_val[cur_ans-1] < cur_ans) {
				cur_ans--;
			}
			while (depth_val[cur_ans] == cur_ans + 1) {
				cur_ans++;
			}
			return cur_ans;
		};

		int64_t tot_ans = 0;


		for (int cur_k = 0; !cur_strings.empty(); cur_k++) {
			//cerr << "cur_k = " << cur_k << '\n';
			std::sort(cur_strings.begin(), cur_strings.end(), [&](auto a, auto b) -> bool {
				return a.idx < b.idx;
			});
			cur_queries.clear();
			{
				int S = log_2(2*int(cur_strings.size())+1);
				auto translate_coord = [&](int c) -> int {
					return int(std::lower_bound(cur_strings.begin(), cur_strings.end(), c, [&](auto a, auto b) -> bool { return a.idx < b; }) - cur_strings.begin());
				};
				while (q_it < int(queries.size()) && queries[q_it].k == cur_k) {
					int l = translate_coord(queries[q_it].l);
					int r = translate_coord(queries[q_it].r);
					cur_queries.push_back({hilbertOrder(l, r, S, 0), {l, r}});
					q_it++;
				}
			}
			std::sort(cur_queries.begin(), cur_queries.end(), [&](auto a, auto b) -> bool {
				return a.first < b.first;
			});
			int l = 0;
			int r = 0;
			for (auto [_, q_bounds] : cur_queries) {
				const auto [ql, qr] = q_bounds;
				while (ql < l) {
					l--;
					insert_pt(cur_strings[l].val);
				}
				while (r < qr) {
					insert_pt(cur_strings[r].val);
					r++;
				}
				while (ql > l) {
					delete_pt(cur_strings[l].val);
					l++;
				}
				while (qr < r) {
					r--;
					delete_pt(cur_strings[r].val);
				}

				assert(l == ql);
				assert(r == qr);
				//cerr << "querying " << ql << ' ' << qr << '\n';
				tot_ans += get_ans();
			}
			while (l < r) {
				delete_pt(cur_strings[l].val);
				l++;
			}

			// expand the strings, time to do some --normal sorting-- or something
			{
				int next_len = 0;
				for (int z = 0; z < int(cur_strings.size()); z++) {
					if (cur_k < int(W[cur_strings[z].idx].size())) {
						cur_strings[next_len++] = {cur_strings[z].idx, cur_strings[z].val * 26 + W[cur_strings[z].idx][cur_k] - 'a'};
					}
				}
				cur_strings.resize(next_len);
				std::sort(cur_strings.begin(), cur_strings.end(), [&](auto a, auto b) -> bool {
					return a.val < b.val;
				});
				V = 0;
				int cur = -1;
				for (int z = 0; z < int(cur_strings.size()); z++) {
					if (cur_strings[z].val != cur) {
						cur = cur_strings[z].val;
						V++;
					}
					cur_strings[z].val = V-1;
				}
			}
		}

		cout << "Case #" << case_num << ": " << tot_ans << '\n';
	}

	return 0;
}
