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

const char* IMPOSSIBLE = "Impossible";
const char* POSSIBLE = "Possible";

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<bool> A(1<<N);
	for (int m = 0; m < (1<<N); m++) {
		char c; cin >> c;
		assert(c == '0' || c == '1');
		A[(1<<N)-1-m] = (c == '1');
	}

	for (int m = 0; m < (1<<N); m++) {
		for (int i = 0; i < N; i++) {
			// m2 is a superset
			if (A[m] > A[m | (1<<i)]) {
				cout << IMPOSSIBLE << '\n';
				exit(0);
			}
		}
	}
	cout << POSSIBLE << '\n';
	mt19937 mt(48);
	vector<int> maxExclude;
	for (int m = 0; m < (1<<N); m++) {
		if (A[m]) continue;
		bool isMaximal = true;
		for (int i = 0; i < N; i++) {
			if (!(m & (1<<i)) && !A[m | (1<<i)]) {
				isMaximal = false;
				break;
			}
		}
		if (isMaximal) {
			maxExclude.push_back(m);
		}
	}
	vector<int> pattern;
	for (int m : maxExclude) {
		vector<int> pts;
		for (int i = 0; i < N; i++) {
			if (m & (1<<i)) continue;
			pts.push_back(i+1);
		}
		assert(!pts.empty());
		//shuffle(pts.begin(), pts.end(), mt);
		queue<vector<int>> q;
		for (auto i : pts) {
			q.push(vector<int>({i}));
		}
		while (q.size() > 1) {
			vector<int> a = std::move(q.front()); q.pop();
			vector<int> b = std::move(q.front()); q.pop();
			vector<int> r; r.reserve(a.size() * 2 + b.size() * 2);
			for (int i : a) r.push_back(i);
			for (int i : b) r.push_back(i);
			reverse(a.begin(), a.end());
			reverse(b.begin(), b.end());
			for (int i : a) r.push_back(-i);
			for (int i : b) r.push_back(-i);
			q.push(std::move(r));
		}
		pattern.insert(pattern.end(), q.front().begin(), q.front().end());
	}
	/*
	vector<int> pattern = std::y_combinator([&](auto self, int n, int l, int r) -> vector<int> {
		assert(A[r-1]);
		if (n == 0) {
			return {};
		}
		vector<int> a = self(n-1, l + (1<<(n-1)), r);
		vector<int> res = a;
		res.push_back(n);
		if (A[l+(1<<(n-1))-1]) {
			vector<int> b = self(n-1, l, l + (1<<(n-1)));
			for (int i : b) res.push_back(i);
			res.push_back(-n);
			reverse(b.begin(), b.end());
			for (int i : b) res.push_back(-i);
		}
		return res;
	})(N, 0, (1<<N));
	*/

	vector<bool> patA(1<<N);
	for (int m = 0; m < (1<<N); m++) {
		stack<int> st;
		for (int i : pattern) {
			if (m & (1<<(abs(i)-1))) {
				continue;
			}
			if (!st.empty() && st.top() == -i) {
				st.pop();
			} else {
				st.push(i);
			}
		}
		patA[m] = st.empty();
	}

	for (int m = 0; m < (1<<N); m++) {
		assert(patA[m] == A[m]);
	}

	vector<pair<int, int>> pos;
	pos.emplace_back(0,1);
	auto goToPt = [&](int x) {
		assert(pos.back().second == 1);
		if (pos.back() == pair<int, int>(x,1)) {
			return;
		}
		pos.emplace_back(pos.back().first, 0);
		while (pos.back().first < x) {
			pos.emplace_back(pos.back().first+1, 0);
		}
		while (pos.back().first > x) {
			pos.emplace_back(pos.back().first-1, 0);
		}
		pos.emplace_back(x,1);
	};
	for (auto i : pattern) {
		assert(i != 0);
		if (i > 0) {
			goToPt(i-1);
			pos.emplace_back(i, 1);
		} else {
			goToPt(-i);
			pos.emplace_back(-i-1, 1);
		}
	}

	goToPt(0);

	if (int(pos.size()) - 1 > 250000) {
		while (true);
	}
	cout << pos.size()-1 << '\n';
	for (auto it : pos) {
		cout << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
