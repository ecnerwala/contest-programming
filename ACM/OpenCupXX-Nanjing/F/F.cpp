#include<bits/stdc++.h>
using namespace std;

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

void go() {
	int N, M, Q; cin >> N >> M >> Q;
	vector<pair<int, pair<int, int>>> edges(M);
	for (auto& e : edges) {
		int x, y, w; cin >> x >> y >> w; x--, y--;
		e = {w, {x,y}};
	}
	sort(edges.begin(), edges.end());

	//cerr << "doing dsu" << '\n';
	vector<pair<int, int>> par(N, {-1, M});
	vector<vector<pair<int, int>>> ch(N);
	for (int e = 0; e < M; e++) {
		int u, v; tie(u, v) = edges[e].second;
		while (par[u].first >= 0) u = par[u].first;
		while (par[v].first >= 0) v = par[v].first;
		if (u == v) continue;
		if (par[u].first > par[v].first) swap(u, v);
		par[u].first += par[v].first;
		par[v] = {u, e};
		ch[u].emplace_back(e, v);
	}

	vector<int> st(N);
	vector<int> en(N);
	//cerr << "building preorder" << '\n';
	{
		int idx = 0;
		for (int root = 0; root < N; root++) {
			if (par[root].first < 0) {
				std::y_combinator([&](auto self, int cur) -> void {
					//cerr << cur << '\n';
					st[cur] = idx;
					idx++;
					for (auto it : ch[cur]) {
						self(it.second);
					}
					en[cur] = idx;
				})(root);
			}
		}
	}
	//cerr << "built preorder" << '\n';

	// return the range for a with edges < lim
	auto get_range = [&](int a, int lim) -> pair<int, int> {
		//cerr << "get_range" << a << ' ' << lim << '\n';
		assert(0 <= lim && lim <= M);
		while (par[a].second < lim) {
			a = par[a].first;
			assert(a >= 0);
		}
		assert(par[a].second >= lim);
		auto it = lower_bound(ch[a].begin(), ch[a].end(), pair<int, int>(lim, -1));
		if (it == ch[a].end()) {
			return {st[a], en[a]};
		} else {
			return {st[a], st[it->second]};
		}
	};

	vector<pair<int, int>> queries(Q);
	for (auto& q : queries) {
		int x, y; cin >> x >> y; x--, y--;
		q = {x,y};
	}

	vector<int> mi(Q, -1);
	vector<int> ma(Q, M+1);

	vector<vector<int>> edgeInds(N);
	for (int e = 0; e < M; e++) {
		int u, v; tie(u, v) = edges[e].second;
		u = st[u], v = st[v];
		edgeInds[u].push_back(v);
		edgeInds[v].push_back(u);
	}

	int S = 1; while (S < N) S *= 2;
	vector<int> seg(2*S);

	vector<tuple<int, pair<int, int>, int>> evts; evts.reserve(2*Q);

	vector<int> curCnt(Q);

	while (true) {
		//cerr << "start binary search" << '\n';
		evts.clear();
		for (int q = 0; q < Q; q++) {
			if (ma[q] - mi[q] == 1) continue;

			int md = (mi[q] + ma[q]) / 2;
			curCnt[q] = 0;
			int u, v; tie(u, v) = queries[q];
			pair<int, int> ur = get_range(u, md);
			pair<int, int> vr = get_range(v, md);

			evts.emplace_back(ur.first, vr, -1-q);
			evts.emplace_back(ur.second, vr, q);
		}

		if (evts.empty()) break;
		sort(evts.begin(), evts.end());

		fill(seg.begin(), seg.end(), 0);
		int curX = 0;
		for (auto [x, yrange, q] : evts) {
			while (curX < x) {
				for (int y : edgeInds[curX]) {
					for (int a = S+y; a; a /= 2) seg[a]++;
				}
				curX++;
			}
			int curVal = 0;
			for (int a = S+yrange.first, b = S+yrange.second; a < b; a /= 2, b /= 2) {
				if (a & 1) curVal += seg[a++];
				if (b & 1) curVal += seg[--b];
			}
			if (q < 0) {
				curCnt[-1-q] -= curVal;
			} else {
				curCnt[q] += curVal;
			}
		}

		for (int q = 0; q < Q; q++) {
			if (ma[q] - mi[q] == 1) continue;

			int md = (mi[q] + ma[q]) / 2;
			if (curCnt[q]) {
				ma[q] = md;
			} else {
				mi[q] = md;
			}
		}
		//cerr << "stop binary search" << '\n';
	}

	for (int q = 0; q < Q; q++) {
		if (mi[q] == -1) {
			cout << 0 << '\n';
		} else if (mi[q] == M) {
			cout << -1 << '\n';
		} else {
			cout << edges[mi[q]].first << '\n';
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		//cerr << "start case\n";
		go();
		//cerr << "end case\n";
	}

	return 0;
}
