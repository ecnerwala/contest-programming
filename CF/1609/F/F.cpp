#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int64_t, int>> A(2*N);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first;
		A[i].second = i;
		A[N+i] = A[i];
	}
	const int MAX_CNT = 60;
	std::vector<int> prv(2*N);
	std::vector<int> nxt(2*N);
	struct list_t {
		int front;
		int back;
	};
	struct range_t {
		int lo;
		bool is_max;
		std::pair<int64_t, int> whole_val;
		list_t part_vals;
		int cnt;
		bool good;
	};

	int64_t ans = 0;
	std::vector<range_t> stk; stk.reserve(N);
	for (int b = 0; b < MAX_CNT; b++) {
		stk.clear();
		for (int i = 0; i < 2*N; i++) {
			prv[i] = -1;
			nxt[i] = -1;
		}

		auto val = [&](const range_t& r) -> int {
			return r.good ? r.cnt : 0;
		};

		int cur_cnt = 0;
		for (int i = 0; i < N; i++) {
			range_t r;
			r.lo = i;
			r.whole_val = A[i];
			r.is_max = i > 0 && A[i] > A[i-1];
			r.part_vals.front = r.part_vals.back = i;
			r.good = (__builtin_popcountll(A[i].first) == b);
			r.cnt = r.good;

			while (!stk.empty()) {
				if (r.is_max == stk.back().is_max) {
					if (r.is_max == (r.whole_val > stk.back().whole_val)) {
						// we need to eat the whole thing
						//
						cur_cnt -= val(stk.back());

						r.lo = stk.back().lo;
						r.cnt += stk.back().cnt;
						// meld stk.back().part_vals and r.part_vals
						{
							int x = stk.back().part_vals.back;
							int y = r.part_vals.front;
							nxt[x] = y;
							prv[y] = x;
							r.part_vals.front = stk.back().part_vals.front;
						}
						stk.pop_back();
					} else {
						// we don't get to eat anything
						break;
					}
				} else {
					if (r.is_max == (r.whole_val > A[stk.back().part_vals.front])) {
						// eat the whole thing
						cur_cnt -= val(stk.back());

						//r.part_vals.push_front(stk.back().whole_val);
						{
							int x = stk.back().whole_val.second;
							prv[x] = -1;
							nxt[x] = r.part_vals.front;
							prv[r.part_vals.front] = x;
							r.part_vals.front = x;
						}
						r.cnt += stk.back().good * (r.lo - stk.back().lo);
						r.lo = stk.back().lo;

						stk.pop_back();
					} else {
						cur_cnt -= val(stk.back());

						// just try to eat end guys on the stk
						int cur = stk.back().whole_val.second;
						while (r.is_max == (A[i] > A[stk.back().part_vals.back])) {
							int v = __builtin_popcountll(A[stk.back().part_vals.back].first);
							// pop_back
							{
								stk.back().part_vals.back = prv[stk.back().part_vals.back];
								nxt[stk.back().part_vals.back] = -1;
							}
							int ncur = A[stk.back().part_vals.back].second;
							stk.back().cnt -= (v == b) * (cur - ncur);
							cur = ncur;
						}
						{
							// push back i+N
							int& x = stk.back().part_vals.back;
							int y = i+N;
							nxt[x] = y;
							prv[y] = x;
							x = y;
						}
						stk.back().cnt += r.good * (stk.back().whole_val.second - cur);

						cur_cnt += val(stk.back());
						break;
					}
				}
			}

			cur_cnt += val(r);
			stk.push_back(std::move(r));
			ans += cur_cnt;
		}
	}
	cout << ans << '\n';

	return 0;
}
