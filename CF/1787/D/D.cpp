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
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) { cin >> A[i]; A[i] += i; }

		std::vector<int8_t> can_escape(N, false);
		std::vector<int8_t> vis(N, false);
		for (int st = 0; st < N; st++) {
			if (vis[st]) continue;
			int cur = st;
			while (0 <= cur && cur < N && !vis[cur]) {
				vis[cur] = true;
				can_escape[cur] = false;
				cur = A[cur];
			}
			if (0 <= cur && cur < N && !can_escape[cur]) {
				// just leave it
			} else {
				// we can escape, so set everything
				cur = st;
				while (0 <= cur && cur < N && !can_escape[cur]) {
					can_escape[cur] = true;
					cur = A[cur];
				}
			}
		}

		if (can_escape[0]) {
			int path_0 = 0;
			int cur = 0;
			int last_pt = 0;
			while (0 <= cur && cur < N) {
				last_pt = cur;
				cur = A[cur];
				path_0 ++;
			}

			int num_bad = 0;
			for (int i = 0; i < N; i++) if (!can_escape[i]) num_bad++;
			int64_t ans = int64_t(N) * int64_t(2*N+1) - int64_t(path_0) * int64_t(num_bad);

			// Now, we're good unless we put ourselves in a cycle
			std::vector<std::vector<int>> ch(N);
			for (int i = 0; i < N; i++) {
				if (0 <= A[i] && A[i] < N) ch[A[i]].push_back(i);
			}
			std::vector<int> num_anc(N, -1);

			std::y_combinator([&](auto self, int cur) -> int {
				int v = 1;
				for (int nxt : ch[cur]) {
					v += self(nxt);
				}
				return num_anc[cur] = v;
			})(last_pt);
			{
				cur = 0;
				while (0 <= cur && cur < N) {
					ans -= num_anc[cur];
					cur = A[cur];
				}
			}

			cout << ans << '\n';
		} else {
			int path_0 = 0;
			int cur = 0;
			std::vector<int8_t> seen(N, false);
			while (!seen[cur]) {
				seen[cur] = true;
				path_0 ++;
				cur = A[cur];
			}

			int num_good = 0;
			for (int i = 0; i < N; i++) if (can_escape[i]) num_good++;

			int64_t ans = int64_t(path_0) * int64_t(num_good + N + 1);
			cout << ans << '\n';
		}
	}

	return 0;
}
