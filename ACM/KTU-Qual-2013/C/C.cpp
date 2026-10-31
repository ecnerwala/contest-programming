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

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T;cin >> T;
	for (int t = 1; t <= T; t++) {
		cout << "Case #" << t << ":" << '\n';
		string Str; cin >> Str;
		int N = int(Str.size());
		int S = 1; while (S < N) S *= 2;
		vector<vector<int>> seg(2*S, vector<int>(26));
		vector<int> lazy(2*S, -1);
		for (int i = 0; i < N; i++) {
			lazy[S+i] = Str[i] - 'A';
		}
		auto updateNode = [&](int a) {
			if (lazy[a] == -1) {
				fill(seg[a].begin(), seg[a].end(), 0);
				seg[a][lazy[a]] = 1 << (__builtin_clz(a) - __builtin_clz(S));
			} else {
				assert(a < S);
				for (int v = 0; v < 26; v++) {
					seg[a][v] = seg[2*a][v] + seg[2*a+1][v];
				}
			}
		};
		auto propogateNode = [&](int a) {
			if (a < S && lazy[a] != -1) {
				lazy[2*a] = lazy[2*a+1] = lazy[a];
				updateNode(2*a);
				updateNode(2*a+1);
				lazy[a] = -1;
			}
		};
		auto propogateAll = std::y_combinator([&](auto self, int a) -> void {
			if (!a) return;
			self(a/2);
			propogateNode(a);
		});
	}

	return 0;
}
