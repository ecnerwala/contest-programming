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

	int W; cin >> W;

	int num_cuts = 0;
	std::y_combinator([&](auto self) -> int {
		int M, num_ch; cin >> M >> num_ch;
		std::vector<int> children(num_ch);
		for (auto& ch : children) {
			ch = self();
		}
		std::sort(children.begin(), children.end());
		int cur_ch = 0;
		for (int i = 0; i < int(children.size()); i++) {
			if (cur_ch + children[i] > W) {
				num_cuts++;
			} else {
				cur_ch += children[i];
			}
		}
		assert(0 <= cur_ch && cur_ch <= W);
		M += cur_ch;
		if (M > 0) {
			num_cuts += (M - 1) / W;
			M = (M - 1) % W + 1;
		}
		return M;
	})();

	cout << num_cuts + 1 << '\n';

	return 0;
}
