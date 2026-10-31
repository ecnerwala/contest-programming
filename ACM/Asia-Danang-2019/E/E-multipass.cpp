#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

		vector<pair<int, int>> V(N);
		for (int i = 0; i < N; i++) {
			cin >> V[i].first;
			V[i].second = i;
		}
		std::sort(V.begin(), V.end());

		vector<int> A(N);
		for (int i = 0; i < N; i++) A[V[i].second] = i;

		struct query_t {
			int l, r;
			int u, v;
			int idx;
		};
		vector<query_t> queries(Q);

		struct bsearch_query_t {
			int l, r;
			int u;
			int* res;
		};
		vector<bsearch_query_t> bsearch_queries; bsearch_queries.reserve(2*Q);

		for (int q = 0; q < Q; q++) {
			int l, r, u, v; cin >> l >> r >> u >> v; l--, u--;
			queries[q].l = l;
			queries[q].r = r;
			queries[q].idx = q;
			bsearch_queries.push_back({l, r, u, &queries[q].u});
			if (v < r-l) {
				bsearch_queries.push_back({l, r, v, &queries[q].v});
			} else {
				queries[q].v = N;
			}
		}

		std::y_combinator([&](auto self, int mi, int ma, std::vector<int> curA, std::vector<bsearch_query_t> curQ) -> void {
			if (ma - mi == 1) {
				for (auto& q : curQ) *q.res = mi;
				return;
			}
			int md = mi + (ma - mi) / 2;

			std::vector<int> loA, hiA;
			std::vector<bsearch_query_t> loQ, hiQ;

			{
				std::vector<int> pref(curA.size()+1);
				for (int i = 0; i < int(curA.size()); i++) pref[i+1] = pref[i] + (curA[i] < md);

				loA.reserve(pref.back());
				hiA.reserve(int(curA.size()) - pref.back());
				for (int i = 0; i < int(curA.size()); i++) (curA[i] < md ? loA : hiA).push_back(curA[i]);
				curA = {};

				loQ.reserve(curQ.size());
				hiQ.reserve(curQ.size());

				for (auto q : curQ) {
					int pl = pref[q.l];
					int pr = pref[q.r];
					int v = pr - pl;
					if (v <= q.u) {
						q.u -= v;
						q.l -= pl;
						q.r -= pr;
						hiQ.push_back(q);
					} else {
						q.l = pl;
						q.r = pr;
						loQ.push_back(q);
					}
				}

				curQ = {};
				loQ.shrink_to_fit();
				hiQ.shrink_to_fit();
			}

			self(mi, md, std::move(loA), std::move(loQ));
			self(md, ma, std::move(hiA), std::move(hiQ));
		})(0, N, A, std::move(bsearch_queries));

		std::vector<int64_t> ans(Q, 0);
		std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.r < b.r; });

		std::vector<int> seg(2*N);
		for (int c = 0; c < 3; c++) {
			for (int b = 0; b < 30; b++) {
				std::fill(seg.begin(), seg.end(), -1);
				int cur_r = 0;
				for (auto q : queries) {
					if (q.v - q.u <= c) continue;

					for (; cur_r < q.r; cur_r++) {
						int v = A[cur_r];
						assert(V[v].second == cur_r);
						int m = V[v].first;
						if (m & (1 << b)) {
							int p = v >= c && V[v-c].first == m ? V[v-c].second : -1;
							for (int a = N+v; a; a >>= 1) {
								setmax(seg[a], p);
							}
						}
					}

					bool has = false;
					for (int x = N+q.u+c, y = N+q.v; x < y; x >>= 1, y >>= 1) {
						if (x & 1) if (seg[x++] >= q.l) { has = true; break; }
						if (y & 1) if (seg[--y] >= q.l) { has = true; break; }
					}

					ans[q.idx] += has << b;
				}
			}
		}

		for (auto v : ans) {
			cout << v << '\n';
		}
	}

	return 0;
}
