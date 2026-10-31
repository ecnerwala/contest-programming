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
		int N, Q; cin >> N >> Q;
		std::vector<std::pair<int64_t, int>> A(N);
		for (auto& a : A) cin >> a.first;
		for (int i = 0; i < N; i++) A[i].second = i;
		std::sort(A.begin(), A.end());

		// coordinate compress them
		std::vector<int> V(N);
		for (int i = 0; i < N; i++) {
			V[A[i].second] = i;
		}

		struct seg_node {
			std::array<int, 2> c{-1,-1};
			int cnt = 0;
		};
		std::vector<seg_node> nodes; nodes.reserve(N * 20);
		std::vector<int> roots(N+1);

		auto build = std::y_combinator([&](auto self, int lo, int hi) -> int {
			int x = int(nodes.size());
			nodes.emplace_back();
			nodes[x].cnt = 0;
			if (hi - lo == 1) {
				return x;
			}
			int md = (lo + hi) / 2;
			int c0 = self(lo, md);
			int c1 = self(md, hi);
			nodes[x].c = {c0, c1};
			return x;
		});
		roots[0] = build(0, N);
		for (int i = 0; i < N; i++) {
			roots[i+1] = int(nodes.size());
			nodes.push_back(nodes[roots[i]]);

			int lo = 0, hi = N;
			int cur = roots[i+1];
			while (hi - lo > 1) {
				nodes[cur].cnt++;
				int md = (lo + hi) / 2;
				int d = V[i] >= md;
				int nxt = int(nodes.size());
				nodes.push_back(nodes[nodes[cur].c[d]]);
				nodes[cur].c[d] = nxt;
				cur = nxt;
				if (d) lo = md;
				else hi = md;
			}
			nodes[cur].cnt++;
			assert(nodes[roots[i+1]].cnt == i+1);
		}

		while (Q--) {
			int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2;
			l1--, l2--;
			auto get_kth = [&](int k) -> int {
				int a1 = roots[l1], b1 = roots[r1], a2 = roots[l2], b2 = roots[r2];
				int lo = 0, hi = N;
				while (hi - lo > 1) {
					int md = (lo + hi) / 2;
					int lcnt = nodes[nodes[b1].c[0]].cnt - nodes[nodes[a1].c[0]].cnt;
					lcnt += nodes[nodes[b2].c[0]].cnt - nodes[nodes[a2].c[0]].cnt;
					int d;
					if (k < lcnt) {
						d = 0;
						hi = md;
					} else {
						d = 1;
						k -= lcnt;
						lo = md;
					}
					a1 = nodes[a1].c[d];
					b1 = nodes[b1].c[d];
					a2 = nodes[a2].c[d];
					b2 = nodes[b2].c[d];
				}
				return lo;
			};

			int len = (r1 - l1) + (r2 - l2);
			if (len & 1) {
				int64_t ans = A[get_kth(len / 2)].first;
				cout << ans << '\n';
			} else {
				int64_t ans = A[get_kth(len/2-1)].first + A[get_kth(len/2)].first;
				cout << ans/2; // rounds towards 0
				if (ans & 1) cout << ".5";
				cout << '\n';
			}
		}
	}

	return 0;
}
