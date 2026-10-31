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

	string S; cin >> S;
	int Q; cin >> Q;
	vector<string> ops(Q); for (auto& o : ops) cin >> o;

	const int INF = 1e9;

	auto CRASH = [&]() { cout << "CRASH" << '\n'; exit(0); };
	auto NO = [&]() { cout << "NO" << '\n'; exit(0); };

	vector<int> len;
	len.push_back(1);
	len.push_back(1);

	vector<array<int, 2>> ch;
	ch.push_back({-1, -1});
	ch.push_back({-1, -1});

	vector<int> stk;
	stk.push_back(0);
	stk.push_back(1);
	int sz = 2;
	for (const auto& op : ops) {
		if (op == "copy") {
			if (sz < 1) CRASH();
			sz++;
			stk.push_back(stk.back());
		} else if (op == "swap") {
			if (sz < 2) CRASH();
			swap(stk.end()[-2], stk.end()[-1]);
		} else if (op == "roll") {
			if (sz < 3) CRASH();
			swap(stk.end()[-3], stk.end()[-2]);
			swap(stk.end()[-2], stk.end()[-1]);
		} else if (op == "fuse") {
			if (sz < 2) CRASH();
			sz--;
			int y = stk.back(); stk.pop_back();
			int x = stk.back(); stk.pop_back();

			int z = int(len.size());
			len.push_back(min(INF, len[x] + len[y]));
			ch.push_back({x, y});

			stk.push_back(z);
		} else assert(false);
	}

	int v = stk.back();

	if (len[v] != int(S.size())) {
		NO();
	}

	string T; T.reserve(S.size());
	std::y_combinator([&](auto self, int cur) -> void {
		if (cur == 0) T += 'a';
		else if (cur == 1) T += 'b';
		else {
			self(ch[cur][0]);
			self(ch[cur][1]);
		}
	})(v);

	if (S != T) {
		NO();
	}
	cout << "YES" << '\n';

	return 0;
}
