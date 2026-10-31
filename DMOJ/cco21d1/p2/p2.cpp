#include <bits/extc++.h>

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
struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K, Q, D, M; cin >> K >> Q >> D >> M;

	auto modK = [&](int64_t a) -> int {
		a %= K;
		return int(a < 0 ? a + K : a);
	};

	vector<vector<int>> digs(K);
	while (D--) {
		int a; cin >> a;
		digs[modK(a)].push_back(a);
	}

	while (Q--) {
		hash_set<int64_t> vis;
		vector<int> resp;
		auto dfs = std::y_combinator([&](auto self, int64_t a) -> bool {
			if (!vis.insert(a).second) {
				return false;
			}
			for (int d : digs[modK(a)]) {
				int64_t b = a-d;
				assert(b % K == 0);
				b /= K;
				if (!b || self(b)) {
					resp.push_back(d);
					vis.erase(a);
					return true;
				}
			}
			return false;
		});

		resp.clear();
		int64_t n; cin >> n;
		if (dfs(n)) {
			for (int i = 0; i < int(resp.size()); i++) {
				cout << resp[i] << " \n"[i+1==int(resp.size())];
			}
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
