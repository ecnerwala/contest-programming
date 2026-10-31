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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<std::pair<int, int>>> adj(2*N);
		for (int i = 0; i < 2*N; i++) {
			int x, y; cin >> x >> y; x--, y--;
			y += N;
			adj[x].push_back({y, i});
			adj[y].push_back({x, i});
		}
		// find any cycle
		std::vector<std::pair<std::array<int, 2>, int>> stk; stk.reserve(2*N);
		std::vector<int> in_stack(2*N, -1);
		bool found_cycle = std::y_combinator([&](auto self, int cur, int p) -> bool {
			assert(in_stack[cur] == -1);
			int best_stack = -1;
			for (auto [nxt, c] : adj[cur]) {
				if (c == p) continue;
				best_stack = std::max(best_stack, in_stack[nxt]);
			}
			if (best_stack != -1) {
				for (auto [nxt, c] : adj[cur]) {
					if (c == p) continue;
					if (in_stack[nxt] == best_stack) {
						stk.push_back({{cur, nxt}, c});
						stk.erase(stk.begin(), stk.begin() + in_stack[nxt]);
						return true;
					}
				}
			}
			for (auto [nxt, c] : adj[cur]) {
				if (c == p) continue;
				in_stack[cur] = int(stk.size());
				stk.push_back({{cur, nxt}, c});
				if (self(nxt, c)) {
					return true;
				}
				in_stack[cur] = -1;
				stk.pop_back();
			}
			// should only be the case if we're a leaf, so we'll never revisit anything
			return false;
		})(0, -1);
		assert(found_cycle);

		assert(stk.size() >= 4);
		while (stk.size() > 4) {
			int l = int(stk.size()) / 2;
			if (!(l & 1)) l--;
			assert(l >= 3 && int(stk.size()) - l >= 3);
			int a = stk[0].first[0];
			int b = stk[l].first[0];
			assert((a >= N) != (b >= N));
			cout << '?' << ' ' << std::min(a,b)+1 << ' ' << std::max(a,b)-N+1 << '\n' << flush;
			int c; cin >> c; c--;
			bool use_first = true;
			for (int i = 0; i < l; i++) {
				use_first = use_first && stk[i].second != c;
			}
			if (use_first) {
				stk.erase(stk.begin()+l, stk.end());
				stk.push_back({{b, a}, c});
			} else {
				stk.erase(stk.begin(), stk.begin()+l);
				stk.push_back({{a, b}, c});
			}
		}
		assert(stk.size() == 4);
		std::array<int, 4> ans;
		for (int i = 0; i < 4; i++) {
			ans[i] = stk[i].first[0];
		}
		std::sort(ans.begin(), ans.end());
		assert(ans[1] < N && ans[2] >= N);
		cout << '!' << ' ' << ans[0]+1 << ' ' << ans[1]+1 << ' ' << ans[2]-N+1 << ' ' << ans[3]-N+1 << '\n' << flush;
		std::string res; cin >> res;
		if (res == "ERROR") exit(0);
		else if (res == "OK") continue;
		else assert(false);
	}

	return 0;
}
