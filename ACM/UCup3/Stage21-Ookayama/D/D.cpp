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

std::vector<int> sim(int msk, std::vector<int> carries) {
	const int a = (msk >> 0) & 1;
	const int b = (msk >> 1) & 1;
	const int c = (msk >> 2) & 1;
	int cura = a;
	int curb = b;
	std::vector<int> n_carries;
	while (true) {
		for (auto d : carries) {
			int nxt = (cura ^ curb) + c + d;
			n_carries.push_back(nxt >> 1);
			nxt &= 1;
			cura = curb;
			curb = nxt;
		}
		if (cura == a && curb == b) break;
	}
	return n_carries;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::set<std::vector<int>> carries;
	carries.insert(std::vector<int>(1, 0));
	for (int d = 0; d < 3; d++) {
		std::set<std::vector<int>> ncarries;
		for (auto c : carries) {
			for (int msk = 0; msk < 8; msk++) {
				for (auto v : c) { cerr << v; } cerr << " (" << c.size() << ")" << " + " << msk << " => ";
				auto r = sim(msk, c);
				for (auto v : r) { cerr << v; } cerr << " (" << r.size() << ")" << '\n';
				ncarries.insert(r);
			}
		}
		carries = ncarries;
	}

	return 0;
}
