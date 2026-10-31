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

	int N; cin >> N;
	vector<int> A(1 << N);
	for (int s = 0; s < (1 << (N-1)); s++) {
		int a, b; cin >> a >> b;
		A[a] = b;
		A[b] = a;
	}
	vector<int> B(1 << N, -1);

	// Break the loop at 0 and A[0]
	vector<int> match = A;
	match[0] = int(match.size());
	match.push_back(0);
	match[A[0]] = int(match.size());
	match.push_back(A[0]);

	std::y_combinator([&](auto self, int cur, int msk) -> void {
		int base = cur & ~msk;
		assert((match[cur] & ~msk) != base);
		int nxt = -1;
		for (int v = msk; true; v = (v-1) & msk) {
			int cnd = v | base;
			if (cnd != cur && (match[cnd] & ~msk) != base) {
				nxt = cnd;
				break;
			}
		}

		if (!(msk & (msk-1))) {
			// link up cur and nxt
			B[cur] = nxt;
			B[nxt] = cur;
			match[match[nxt]] = match[cur];
			match[match[cur]] = match[nxt];
			return;
		} else {
			int diff = nxt ^ cur;
			diff = diff & -diff; // get the lowest order bit
			self(cur, msk ^ diff);
			self(nxt, msk ^ diff);
		}
	})(0, (1 << N) - 1);

	for (int z = 0, cur = 0; z < (1 << (N-1)); z++, cur = B[A[cur]]) {
		cout << cur << ' ' << A[cur] << " \n"[z+1==(1<<(N-1))];
	}

	return 0;
}
