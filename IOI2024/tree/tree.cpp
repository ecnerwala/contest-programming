#include "tree.h"

#include <bits/stdc++.h>

namespace ecnerwala {

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

std::vector<int64_t> ans;

}

void init(std::vector<int> P, std::vector<int> W) {
	int N = int(P.size());
	assert(P[0] == -1);
	std::vector<std::vector<int>> ch(N);
	for (int i = 1; i < N; i++) ch[P[i]].push_back(i);

	int64_t ans_base = 0;

	std::vector<std::pair<int, int>> stk(N+1);
	stk[0] = {0, -1};
	int stk_len = 1;
	std::vector<std::vector<std::pair<int, int>>> ch_weights(N);
	std::vector<int64_t> savings_rate(N+1);
	ecnerwala::y_combinator([&](auto self, int cur) -> int {
		int w = W[cur];
		if (ch[cur].empty()) {
			ans_base += w;
		} else {
			int mi = 0;
			int ma = stk_len;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (stk[md].first <= w) mi = md;
				else ma = md;
			}
			stk[stk_len-1].second = cur;
			std::pair<int, int> old_val = std::exchange(stk[ma], {w, -1});
			int old_len = std::exchange(stk_len, ma+1);

			int own_cnt = int(ch[cur].size()) - 1;
			ans_base += int64_t(w) * int64_t(own_cnt);
			for (int nxt : ch[cur]) {
				own_cnt += self(nxt);
			}
			ch_weights[stk[mi].second].push_back({w, own_cnt});

			stk[ma] = old_val;
			stk_len = old_len;
			stk[stk_len-1].second = -1;

		}

		int cnt = 0;
		int pw = stk[stk_len-1].first;
		std::sort(ch_weights[cur].begin(), ch_weights[cur].end(), std::greater<>());
		for (auto [v, v_cnt] : ch_weights[cur]) {
			assert(v >= pw);
			// save at a rate of (v-w) from cnt .. cnt + v_cnt
			savings_rate[cnt] += v - pw;
			cnt += v_cnt;
			savings_rate[cnt] -= v - pw;
		}
		return cnt;
	})(0);
	std::vector<int64_t> ans(N+1);
	ans[0] = ans_base;
	for (int i = 0; i < N; i++) {
		savings_rate[i+1] += savings_rate[i];
		ans[i+1] = ans[i] - savings_rate[i];
	}
	assert(savings_rate[N] == 0);
	ecnerwala::ans = std::move(ans);
}

long long query(int L, int R) {
	R -= L;
	using ecnerwala::ans;
	int f = R / L;
	assert(f >= 0);
	if (f + 1 >= int(ans.size())) {
		return ans.back() * L;
	}
	return L * ans[f] + (R - L * f) * (ans[f+1] - ans[f]);
}
