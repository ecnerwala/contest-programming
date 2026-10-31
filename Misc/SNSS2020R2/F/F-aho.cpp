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

template <typename T> struct bit {
private:
	std::vector<T> dat;
public:
	bit() {}
	explicit bit(size_t N) : dat(N) {}
	bit(size_t N, const T& t) : dat(N, t) {}

	template <typename F> void for_prefix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_prefix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_suffix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}

	template <typename F> void for_suffix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	int N = int(S.size());
	int Q; cin >> Q;

	vector<int> ans(Q);
	vector<vector<pair<int, int>>> evts(N+1);

	int V = 1;
	vector<array<int, 26>> ch(1);
	ch[0].fill(0);
	ch.reserve(int(2e6) + 100);

	for (int q = 0; q < Q; q++) {
		int l, r; string M; cin >> l >> r >> M; r++;

		l += int(M.size());

		int cur = 0;
		for (char c : M) {
			if (!ch[cur][c-'a']) {
				ch[cur][c-'a'] = V;
				ch.emplace_back();
				ch.back().fill(0);
				V++;
			}

			cur = ch[cur][c-'a'];
		}

		// endpoint is [l, r]
		evts[l-1].push_back({~q, cur});
		evts[r].push_back({q, cur});
	}

	vector<int> fail(V, -1);
	{
		vector<int> q; q.reserve(V);
		q.push_back(0);
		for (int z = 0; z < V; z++) {
			int cur = q[z];
			for (int c = 0; c < 26; c++) {
				int nxt = ch[cur][c];
				if (nxt) {
					q.push_back(nxt);
					fail[nxt] = z ? ch[fail[cur]][c] : 0;
				} else {
					ch[cur][c] = z ? ch[fail[cur]][c] : 0;
				}
			}
		}
		assert(int(q.size()) == V);
	}

	vector<vector<int>> failCh(V);
	for (int i = 1; i < V; i++) {
		failCh[fail[i]].push_back(i);
	}
	vector<array<int, 2>> bounds(V);
	{
		int idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			bounds[cur][0] = idx;
			idx++;
			for (auto nxt : failCh[cur]) {
				self(nxt);
			}
			bounds[cur][1] = idx;
		})(0);
		assert(idx == V);
	}

	bit<int> bounds_bit(V, 0);
	int state = 0;
	for (int i = 0; i <= N; i++) {
		bounds_bit.for_prefix(bounds[state][0]+1, [&](int& v) { v++; });
		for (auto [q, q_state] : evts[i]) {
			int cnt = 0;
			bounds_bit.for_suffix(bounds[q_state][0], [&](int v) { cnt += v; });
			bounds_bit.for_suffix(bounds[q_state][1], [&](int v) { cnt -= v; });

			if (q >= 0) {
				ans[q] += cnt;
			} else {
				ans[~q] -= cnt;
			}
		}

		if (i < N) {
			state = ch[state][S[i]-'a'];
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
