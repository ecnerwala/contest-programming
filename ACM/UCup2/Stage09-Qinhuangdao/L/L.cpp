#include <bits/stdc++.h>

using namespace std;
static struct FastInput {
	static constexpr int BUF_SIZE = 1 << 20;
	char buf[BUF_SIZE];
	size_t chars_read = 0;
	size_t buf_pos = 0;
	FILE *in = stdin;
	char cur = 0;

	inline char get_char() {
		if (buf_pos >= chars_read) {
			chars_read = fread(buf, 1, BUF_SIZE, in);
			buf_pos = 0;
			buf[0] = (chars_read == 0 ? -1 : buf[0]);
		}
		return cur = buf[buf_pos++];
	}

	inline void tie(int) {}

	inline explicit operator bool() {
		return cur != -1;
	}

	inline static bool is_blank(char c) {
		return c <= ' ';
	}

	inline bool skip_blanks() {
		while (is_blank(cur) && cur != -1) {
			get_char();
		}
		return cur != -1;
	}

	inline FastInput& operator>>(char& c) {
		skip_blanks();
		c = cur;
		return *this;
	}

	inline FastInput& operator>>(string& s) {
		if (skip_blanks()) {
			s.clear();
			do {
				s += cur;
			} while (!is_blank(get_char()));
		}
		return *this;
	}

	template <typename T>
		inline FastInput& read_integer(T& n) {
			// unsafe, doesn't check that characters are actually digits
			n = 0;
			if (skip_blanks()) {
				int sign = +1;
				if (cur == '-') {
					sign = -1;
					get_char();
				}
				do {
					n += n + (n << 3) + cur - '0';
				} while (!is_blank(get_char()));
				n *= sign;
			}
			return *this;
		}

	template <typename T>
		inline typename enable_if<is_integral<T>::value, FastInput&>::type operator>>(T& n) {
			return read_integer(n);
		}

#if !defined(_WIN32) || defined(_WIN64)
	inline FastInput& operator>>(__int128& n) {
		return read_integer(n);
	}
#endif

	template <typename T>
		inline typename enable_if<is_floating_point<T>::value, FastInput&>::type operator>>(T& n) {
			// not sure if really fast, for compatibility only
			n = 0;
			if (skip_blanks()) {
				string s;
				(*this) >> s;
				sscanf(s.c_str(), "%lf", &n);
			}
			return *this;
		}
} fast_input;

#define cin fast_input

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false);

	int T; cin >> T;
	//std::mt19937 mt(48);
	//int T = 100;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N); for (auto& p : P) { cin >> p; p--; }
		//int N = std::uniform_int_distribution<int>(1, 10)(mt);
		//std::vector<int> P(N); std::iota(P.begin(), P.end(), 0); std::shuffle(P.begin(), P.end(), mt);
		std::vector<int> inv_P(N);
		for (int i = 0; i < N; i++) {
			inv_P[P[i]] = i;
		}

		std::vector<std::pair<int, int>> pairs; pairs.reserve(2*N);

		auto add_point = [&](int x, int y) {
			if (x < 0 || x >= N || y < 0 || y >= N) return;
			assert(x <= y);
			pairs.push_back({x, y});
		};

		int vlo = inv_P[0];
		int vhi = inv_P[0] - 1;
		int cur_v = 0;

		int mi0 = inv_P[0];
		int mi1 = N;
		int ma0 = inv_P[0];
		int ma1 = -1;
		// NOTE: Bug here where sum_idx was initialized to 0 on accident
		int64_t sum_idx = inv_P[0];

		std::vector<int> node_val(N);

		for (int v = 1; v < N; v++) {
			int i = inv_P[v];
			sum_idx += i;
			if (i < mi0) {
				mi1 = mi0;
				mi0 = i;
			} else if (i < mi1) {
				mi1 = i;
			}
			if (i > ma0) {
				ma1 = ma0;
				ma0 = i;
			} else if (i > ma1) {
				ma1 = i;
			}

			int l = v+1;
			// now, try moving each of them
			assert(ma0 - mi0 + 1 >= l);
			if (ma0 - mi0 + 1 == l) {
				// we can also cycle it potentially
				cur_v++;
				while (mi0 < vlo) {
					vlo--;
					node_val[vlo] = cur_v;
				}
				while (ma0 > vhi) {
					vhi++;
					node_val[vhi] = cur_v;
				}

				add_point(mi0, ma0 + 1);
				add_point(mi0 - 1, ma0);
			} else {
				// try moving mi0
				if (ma0 - mi1 + 1 == l-1) {
					add_point(mi0, mi1 - 1);
					add_point(mi0, ma0 + 1);
				} else if (ma0 - mi1 + 1 == l) {
					int missing = int(int64_t(ma0 - mi1 + 1) * int64_t(mi1 + ma0) / 2 - (sum_idx - mi0));
					assert(mi1 <= missing && missing <= ma0 && P[missing] > v);
					add_point(mi0, missing);
				} else {
					// nothing good
				}

				if (ma1 - mi0 + 1 == l-1) {
					add_point(mi0 - 1, ma0);
					add_point(ma1 + 1, ma0);
				} else if (ma1 - mi0 + 1 == l) {
					int missing = int(int64_t(ma1 - mi0 + 1) * int64_t(mi0 + ma1) / 2 - (sum_idx - ma0));
					assert(mi0 <= missing && missing <= ma1 && P[missing] > v);
					add_point(missing, ma0);
				} else {
					// nothing good
				}
			}
		}

		// NOTE: bug here, bad assertion for N = 1
		//assert(vlo == 0 && vhi == N-1);
		std::vector<int> freq(N);
		for (auto [x, y] : pairs) {
			freq[x]++;
		}
		for (int i = 1; i < N; i++) freq[i] += freq[i-1];
		std::vector<std::pair<int, int>> s_pairs(pairs.size());
		for (auto [x, y] : pairs) {
			s_pairs[--freq[x]] = {x, y};
		}
		std::fill(freq.begin(), freq.end(), 0);
		for (auto [x, y] : s_pairs) {
			freq[y]++;
		}
		for (int i = 1; i < N; i++) freq[i] += freq[i-1];
		for (auto [x, y] : s_pairs) {
			pairs[--freq[y]] = {x, y};
		}

		int best_val = 0;
		int best_i = inv_P[0], best_j = inv_P[0];

		int cur_cnt = 0;
		// NOTE: BUG HERE where i looped up to N
		for (int i = 0; i < int(pairs.size()); i++) {
			if (i > 0 && pairs[i] == pairs[i-1]) {
				cur_cnt++;
			} else {
				cur_cnt = 1;
			}
			auto [x, y] = pairs[i];
			int cnd = cur_cnt - std::abs(node_val[x] - node_val[y]);
			if (cnd > best_val) {
				best_val = cnd;
				best_i = x;
				best_j = y;
			}
		}

		cout << best_i + 1 << ' ' << best_j + 1 << '\n';
	}

	return 0;
}
