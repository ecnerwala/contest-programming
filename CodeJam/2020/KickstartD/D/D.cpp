#include<bits/stdc++.h>

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
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, Q; cin >> N >> Q;
		vector<int> D(N-1); for (int& d : D) cin >> d;
		vector<int> answers(Q);
		vector<vector<pair<int, int>>> queries(N);
		for (int q = 0; q < Q; q++) {
			int s, k; cin >> s >> k; s--, k--;
			queries[s].push_back({k, q});
		}

		// build the cartesian tree
		vector<pair<int, int>> seg(2*(N-1));
		for (int i = 0; i < N-1; i++) seg[N-1+i] = {D[i], i};
		for (int i = N-2; i; i--) seg[i] = max(seg[2*i], seg[2*i+1]);
		auto query_seg = [&](int l, int r) -> pair<int, int> {
			assert(l < r);
			pair<int, int> ans{-1, -1};
			for (int a = N-1+l, b = N-1+r; a < b; a /= 2, b /= 2) {
				if (a & 1) ans = max(ans, seg[a++]);
				if (b & 1) ans = max(ans, seg[--b]);
			}
			return ans;
		};

		using pq = priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>;
		std::y_combinator([&](auto self, int l, int r) -> pq {
			assert(l < r);
			int len = r - l;
			if (len == 1) {
				pq resp(queries[l].begin(), queries[l].end());
				while (!resp.empty() && resp.top().first == 0) {
					answers[resp.top().second] = l;
					resp.pop();
				}
				return std::move(resp);
			}

			int m = query_seg(l, r-1).second+1; assert(l < m && m < r);
			pq l_resp = self(l, m);
			while (!l_resp.empty() && l_resp.top().first < len) {
				answers[l_resp.top().second] = l+l_resp.top().first;
				l_resp.pop();
			}

			pq r_resp = self(m, r);
			while (!r_resp.empty() && r_resp.top().first < len) {
				answers[r_resp.top().second] = r-1-r_resp.top().first;
				r_resp.pop();
			}

			if (l_resp.size() > r_resp.size()) swap(l_resp, r_resp);
			while (!l_resp.empty()) {
				r_resp.push(l_resp.top());
				l_resp.pop();
			}
			return std::move(r_resp);
		})(0, N);

		cout << "Case #" << case_num << ":";
		for (int q = 0; q < Q; q++) cout << ' ' << answers[q]+1;
		cout << '\n';
	}

	return 0;
}
