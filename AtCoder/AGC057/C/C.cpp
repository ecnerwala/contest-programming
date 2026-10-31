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

void slow() {
	using std::cerr;
	std::array<int, 16> st; std::iota(st.begin(), st.end(), 0);
	//std::swap(st[0], st[8]);
	//std::swap(st[4], st[8+4]);

	std::array<int, 16> en; std::iota(en.begin(), en.end(), 0);

	std::map<std::array<int, 16>, int> prv;
	std::vector<std::array<int, 16>> q;
	q.push_back(st);
	prv[st] = -2;

	for (int z = 0; z < int(q.size()); z++) {
		auto cur = q[z];
		bool good = true;
		bool nontrivial = false;
		for (int i = 0; i < 16; i++) {
			good = good && !((cur[i] - i) & 7);
		}
		for (int i = 0; i < 4; i++) {
			if ((cur[i] - i) != (cur[i+4] - i - 4)) {
				nontrivial = true;
			}
		}
		if (good && nontrivial) {
			std::cerr << "found " << '\n';
			std::vector<int> ops;
			for (int v : cur) { cerr << v << ' '; } cerr << '\n';
			while (cur != st) {
				int x = prv[cur];
				ops.push_back(x);
				std::cerr << x << '\n';
				if (x == -1) {
					for (auto& v : cur) {
						v --;
						v &= 15;
					}
				} else if (x >= 0) {
					for (auto& v : cur) {
						v ^= x;
					}
				} else assert(false);
				//for (int v : cur) { cerr << v << ' '; } cerr << '\n';
			}
			//break;
		}
		for (int x = 0; x < 16; x++) {
			auto nxt = cur;
			for (auto& v : nxt) v ^= x;
			if (prv.count(nxt)) continue;
			prv[nxt] = x;
			q.push_back(nxt);
		}
		{
			auto nxt = cur;
			for (auto& v : nxt) {
				v++;
				v &= 15;
			}
			if (prv.count(nxt)) continue;
			prv[nxt] = -1;
			q.push_back(nxt);
		}
	}
	std::cerr << "not found" << '\n';
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//slow();

	int N; cin >> N;
	std::vector<int> A(1 << N);
	for (auto& a : A) cin >> a;
	auto a_orig = A;

	std::vector<int> ops; ops.reserve(4 * (1 << N));
	if (std::y_combinator([&](auto self, int n) -> bool {
		if (n == 0) return true;
		n--;
		int h = (1 << n); // size of a half
		for (int i = 0; i < h; i++) {
			if (((A[i] ^ A[i+h]) & (2*h-1)) != h) {
				return false;
			}
			A[i+h] -= A[i];
		}
		if (!self(n)) return false;
		for (int i = 0; i < h; i++) {
			A[i+h] += A[i];
		}
		// now we have to pick which ones to flip
		if (n == 0) {
			if (A[0] & 1) {
				ops.push_back(1);
				A[0] ^= 1;
				A[1] ^= 1;
			}
			return true;
		}
		assert(n >= 1);
		int q = 1 << (n-1); // quarter of a group
		if ((A[0] & h) != (A[q] & h)) {
			// flip the entire first or second quarter
			int cnt = 0;
			for (int c = 0; c < q; c++) {
				if ((A[0] & h)) cnt++;
			}
			if (cnt > q - cnt) {
				// Flip A[0..q]
				ops.push_back(q);
				for (int z = 0; z < q; z++) ops.push_back(-1);
				for (int i = 0; i < 2*h; i++) {
					A[i] ^= q;
					A[i] += q;
				}
			} else {
				for (int z = 0; z < q; z++) ops.push_back(-1);
				ops.push_back(q);
				for (int i = 0; i < 2*h; i++) {
					A[i] += q;
					A[i] ^= q;
				}
			}
		}
		for (int i = 0; i < h; i++) {
			assert(!((A[i] ^ i) & (h-1)));
		}
		for (int i = 0; i < q; i++) {
			if ((A[i] & h) != (A[i+q] & h)) {
				assert(i != 0);
				return false;
			}
			if (A[i] & h) {
				assert(A[i+q] & h);
				assert(!(A[i+h] & h));
				assert(!(A[i+q+h] & h));
				// flip these two
				ops.push_back(q - 1 - i);
				ops.push_back(-1);
				ops.push_back(q-1);
				ops.push_back(-1);
				ops.push_back(i);
				A[i] += h;
				A[i+q] += h;
				A[i+h] += h;
				A[i+q+h] += h;
			}
		}
		return true;
	})(N)) {
		cout << "Yes" << '\n';
		cout << ops.size() << '\n';
		for (int i = 0; i < int(ops.size()); i++) {
			cout << ops[i] << " \n"[i+1==int(ops.size())];
		}

		if (false) {
			for (auto o : ops) {
				if (o == -1) {
					for (auto& v : a_orig) {
						v ++;
						v &= (1 << N) - 1;
					}
				} else {
					for (auto& v : a_orig) {
						v ^= o;
					}
				}
			}
			for (int i = 0; i < (1 << N); i++) { cerr << a_orig[i] << ' '; } cerr << '\n';
			for (int i = 0; i < (1 << N); i++) assert(a_orig[i] == i);
		}

	} else {
		cout << "No" << '\n';
	}

	return 0;
}
