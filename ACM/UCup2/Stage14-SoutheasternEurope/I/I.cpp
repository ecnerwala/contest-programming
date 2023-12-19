#include <bits/stdc++.h>

struct multiset_t {
	int first_nonzero = 0;
	std::array<int, 10> freq{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	friend bool operator == (const multiset_t& a, const multiset_t& b) {
		return a.first_nonzero == b.first_nonzero && a.freq == b.freq;
	}
	friend bool operator < (const multiset_t& a, const multiset_t& b) {
		return a.first_nonzero < b.first_nonzero || (a.first_nonzero == b.first_nonzero && a.freq > b.freq);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	std::vector<int> hall_cnt(1024);
	for (int i = 0; i < N; i++) {
		int msk = 0;
		for (int z = 0; z < 6; z++) {
			int d; cin >> d;
			msk |= (1 << d);
		}
		hall_cnt[1024-1-msk]++;
	}
	for (int d = 0; d < 10; d++) {
		for (int a = 0; a < 1024; a++) {
			if (a & (1 << d)) continue;
			hall_cnt[a] += hall_cnt[a + (1 << d)];
		}
	}
	for (int a = 0; a < 1024; a++) {
		hall_cnt[a] = N - hall_cnt[a];
	}

	// Keep up to K strings
	std::vector<std::vector<multiset_t>> base_multisets(N+5);
	for (int m = 1; m < 1024; m++) {
		if (m == 1) {
			for (int i = 1; i < 10; i++) {
				multiset_t a;
				a.freq[0] = hall_cnt[m] + 1;
				a.freq[i] = 1;
				a.first_nonzero = i;
				base_multisets[hall_cnt[m] + 2].push_back(a);
			}
		} else {
			if (hall_cnt[m] + 1 < __builtin_popcount(m)) {
				// already screwed
				continue;
			}
			multiset_t a;
			for (int i = 0; i < 10; i++) {
				if (m & (1 << i)) a.freq[i]++;
			}
			a.freq[__builtin_ctz(m)] += hall_cnt[m] + 1 - __builtin_popcount(m);
			a.first_nonzero = 1 + __builtin_ctz(m >> 1);
			base_multisets[hall_cnt[m] + 1].push_back(a);
		}
	}

	// We'll keep a bunch of bad multisets
	std::vector<multiset_t> cur_multisets; cur_multisets.reserve(K);
	for (int l = 1; K > 0; l++) {
		{
			// first increase length by 1
			std::set<multiset_t> nxt_multisets;
			if (l < int(base_multisets.size())) {
				nxt_multisets.insert(base_multisets[l].begin(), base_multisets[l].end());
			}
			for (const auto& a : cur_multisets) {
				for (int i = 0; i < 10; i++) {
					auto b = a;
					b.freq[i]++;
					if (i && i < b.first_nonzero) {
						b.first_nonzero = i;
					}
					nxt_multisets.insert(b);
				}
			}
			cur_multisets.clear();
			while (!nxt_multisets.empty() && int(cur_multisets.size()) < K) {
				auto cur = *nxt_multisets.begin();
				nxt_multisets.erase(nxt_multisets.begin());
				cur_multisets.push_back(cur);
				{
					int m = 0;
					for (int i = 0; i < 10; i++) {
						if (cur.freq[i]) m |= (1 << i);
					}
					if (l == hall_cnt[m] + 1) {
						// take the last thing that's not 1
						int last_val = 10;
						for (int i = 9; i >= 0; i--) {
							if (!cur.freq[i]) continue;
							if (last_val == 10) {
								last_val = i;
								continue;
							}
							if (cur.freq[i] >= 2) {
								// we should try decrementing this guy
								auto nxt = cur;
								nxt.freq[i]--;
								int acc = 1;
								acc += nxt.freq[last_val] - 1;
								nxt.freq[last_val] = 1;
								for (int j = i+1; true; j++) {
									assert(j <= last_val);
									if (nxt.freq[j]) {
										nxt.freq[j] += acc;
										acc = 0;
										break;
									}
								}
								nxt_multisets.insert(nxt);
								break;
							}
						}
					}
				}
			}
		}
		std::set<std::string> best_strings;
		for (auto ms : cur_multisets) {
			std::string s; s.reserve(l);
			s += char('0' + ms.first_nonzero);
			for (int i = 0; i < 10; i++) {
				int f = ms.freq[i] - (i == ms.first_nonzero);
				for (int z = 0; z < f; z++) {
					s += char('0' + i);
				}
			}
			assert(int(s.size()) == l);
			best_strings.insert(s);
		}
		while (!best_strings.empty() && K > 0) {
			auto s = *best_strings.begin();
			best_strings.erase(best_strings.begin());
			K--;
			cout << s << " \n"[K==0];
			if (std::next_permutation(s.begin(), s.end())) {
				best_strings.insert(s);
			}
		}
	}

	return 0;
}
