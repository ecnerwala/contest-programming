#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	const int MAX_SZ = int(2.01e6);
	std::vector<std::array<int, 3>> ch; ch.reserve(MAX_SZ);
	std::vector<int> cnt; cnt.reserve(MAX_SZ);
	std::vector<int> subtree_cnt; subtree_cnt.reserve(MAX_SZ);
	ch.push_back({-1, -1, -1});
	cnt.push_back(0);
	subtree_cnt.push_back(0);

	for (int i = 0; i < N; i++) {
		std::string s; cin >> s;
		s = s + s;
		int cur = 0;
		for (char c : s) {
			int v;
			if (c == 'K') {
				v = 0;
			} else if (c == 'P') {
				v = 1;
			} else if (c == 'N') {
				v = 2;
			} else assert(false);
			if (ch[cur][v] == -1) {
				ch[cur][v] = int(ch.size());
				ch.push_back({-1, -1, -1});
				cnt.push_back(0);
				subtree_cnt.push_back(0);
			}
			subtree_cnt[cur]++;
			cur = ch[cur][v];
		}
		subtree_cnt[cur]++;
		cnt[cur]++;
	}

	auto get_sz = [&](int a) -> int {
		return a == -1 ? 0 : subtree_cnt[a];
	};

	std::vector<int> cur_string; cur_string.reserve(ch.size());
	std::vector<int> freq; freq.reserve(ch.size());
	std::vector<int> fail; fail.reserve(ch.size());
	int64_t ans = 0;
	std::y_combinator([&](auto self, int cur) -> void {
		freq.push_back(cnt[cur]);
		if ((ch[cur][0] != -1) + (ch[cur][1] != -1) + (ch[cur][2] != -1) >= 2) {
			// we're a branch point, so let's look for periods
			fail.clear();
			fail.push_back(-1);
			for (int i = 0; i < int(cur_string.size()); i++) {
				int cnd = fail[i];
				while (cnd != -1 && cur_string[i] != cur_string[cnd]) {
					cnd = fail[cnd];
				}
				fail.push_back(cnd+1);
			}
			int period = int(cur_string.size()) - fail.back();
			int v = cur_string.empty() ? 0 : cur_string[int(cur_string.size()) % period];
			int cnt_periodic = 0;
			for (int i = period; i <= int(cur_string.size()); i += period) {
				cnt_periodic += freq[i];
			}
			cnt_periodic += get_sz(ch[cur][v]);
			ans += int64_t(cnt_periodic) * int64_t(get_sz(ch[cur][(v + 1) % 3])) * int64_t(get_sz(ch[cur][(v + 2) % 3]));
		}
		for (int v = 0; v < 3; v++) {
			if (ch[cur][v] != -1) {
				cur_string.push_back(v);
				self(ch[cur][v]);
				cur_string.pop_back();
			}
		}
		freq.pop_back();
	})(0);
	cout << ans << '\n';

	return 0;
}
