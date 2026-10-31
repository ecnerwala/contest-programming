#define NDEBUG
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

struct union_find {
	std::vector<int> par;
	union_find() {}
	union_find(int N) : par(N, -1) {}
	int get_par(int a) {
		return par[a] >= 0 ? (par[a] = get_par(par[a])) : a;
	}
	bool merge(int a, int b) {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	}
	bool merge(std::array<int, 2> a) { return merge(a[0], a[1]); }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, K; cin >> B >> K;
	string inp_string; inp_string.reserve(B);
	auto read_string = [&]() -> int64_t {
		int64_t r = 0;
		cin >> inp_string;
		assert(int(inp_string.size()) == B);
		for (int i = 0; i < B; i++) {
			assert(inp_string[i] == '0' || inp_string[i] == '1');
			r |= int64_t(inp_string[i] - '0') << i;
		}
		return r;
	};

	int64_t X = read_string();
	int64_t Y = read_string();

	std::vector<int64_t> bad(K);
	for (auto& v : bad) v = read_string();
	std::sort(bad.begin(), bad.end());
	bad.resize(std::unique(bad.begin(), bad.end()) - bad.begin());
	assert(int(bad.size()) == K);

	auto p2 = [](int i) -> int64_t { return int64_t(1) << i; };

	std::vector<int64_t> boundaries; boundaries.reserve(3*K+5);
	std::vector<char> is_live; is_live.reserve(3*K+4);
	boundaries.push_back(0);
	is_live.push_back(true);
	for (int64_t v : bad) {
		if (boundaries.back() == v) {
			// do nothing
			is_live.back() = false;
		} else {
			int64_t l = boundaries.back(), r = v;
			assert(l < r);
			int64_t m = r & -p2(63 - __builtin_clzll(l ^ r));
			assert(l < m && m <= r);
			if (m < r) {
				boundaries.push_back(m);
				is_live.push_back(true);
			}
			boundaries.push_back(r);
			is_live.push_back(false);
		}
		boundaries.push_back(v+1);
		is_live.push_back(true);
	}
	if (boundaries.back() != p2(B)) {
		boundaries.push_back(p2(B));
	} else {
		is_live.pop_back();
	}

	union_find uf(int(boundaries.size())-1);

	std::y_combinator([&](auto self, int b, int lo, int hi) -> void {
		if (hi - lo == 1) {
			return;
		}
		assert(hi - lo >= 2);
		assert(b >= 0);
		int64_t blo = boundaries[lo];
		int64_t bhi = boundaries[hi];
		assert(!(blo & (p2(b+1)-1)));
		assert(!(bhi & (p2(b+1)-1)));
		assert(bhi - blo == p2(b+1));
		int64_t bmi = blo + p2(b);
		if (boundaries[lo+1] > bmi) {
			assert(is_live[lo]);
			for (int i = lo+1; i < hi; i++) {
				if (is_live[i]) {
					uf.merge(lo, i);
				}
			}
		} else if (boundaries[hi-1] < bmi) {
			assert(is_live[hi-1]);
			for (int i = lo; i < hi-1; i++) {
				if (is_live[i]) {
					uf.merge(hi-1, i);
				}
			}
		} else {
			int mi = lo+1;
			while (mi < hi && boundaries[mi] != bmi) mi++;
			assert(mi != hi);

			self(b-1, lo, mi);
			self(b-1, mi, hi);

			for (int i = lo, j = mi; i < mi && j < hi; ) {
				assert(max(boundaries[i], boundaries[j]-p2(b)) < min(boundaries[i+1], boundaries[j+1]-p2(b)));
				if (is_live[i] && is_live[j]) {
					uf.merge(i, j);
				}
				if (boundaries[i+1] == boundaries[j+1]-p2(b)) {
					i++, j++;
				} else if (boundaries[i+1] < boundaries[j+1]-p2(b)) {
					i++;
				} else if (boundaries[i+1] > boundaries[j+1]-p2(b)) {
					j++;
				} else assert(false);
			}
		}
	})(B-1, 0, int(boundaries.size())-1);

	int bx = int(std::upper_bound(boundaries.begin(), boundaries.end(), X) - boundaries.begin()) - 1;
	int by = int(std::upper_bound(boundaries.begin(), boundaries.end(), Y) - boundaries.begin()) - 1;

	cout << (uf.get_par(bx) == uf.get_par(by) ? "TAK" : "NIE") << '\n';
	return 0;
}
