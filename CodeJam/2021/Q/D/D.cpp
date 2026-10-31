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

	int T, N, Q; cin >> T >> N >> Q;
	for (int case_num = 1; case_num <= T; case_num ++) {

		vector<int> ans; ans.reserve(N);
		ans.push_back(1);
		ans.push_back(2);
		for (int c = 3; c <= N; c++) {
			int mi = -1, ma = int(ans.size());
			while (ma - mi > 1) {
				int md1 = mi + (ma - mi + 2) / 3;
				int md2 = mi + (ma - mi) * 2 / 3;
				assert(mi < md1 && md1 <= md2 && md2 < ma);
				if (md1 == md2) {
					if (md1 != 0) md1--;
					else if (md2 != int(ans.size())-1) md2++;
					else assert(false);
				}
				cout << ans[md1] << ' ' << ans[md2] << ' ' << c << '\n' << flush;
				int resp; cin >> resp;
				if (resp == -1) exit(0);
				if (resp == ans[md1]) {
					ma = md1;
				} else if (resp == ans[md2]) {
					mi = md2;
				} else if (resp == c) {
					mi = md1;
					ma = md2;
				} else assert(false);
			}
			ans.insert(ans.begin()+ma, c);
		}
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
		cout << flush;
		int resp; cin >> resp;
		if (resp == -1) exit(0);
		if (resp == 1) {
			// correct
		} else assert(false);
	}

	return 0;
}
