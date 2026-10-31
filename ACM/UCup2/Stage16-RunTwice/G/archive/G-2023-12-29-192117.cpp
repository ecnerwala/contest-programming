#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

template <typename K, typename V>
using hash_map = __gnu_pbds::gp_hash_table<K, V>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1e6);
	std::vector<uint64_t> vals(V);
	std::mt19937_64 mt(484848968734);
	for (auto& v : vals) { v = mt(); }

	hash_map<uint64_t, int> mp;
	for (int i = 0; i < V; i++) {
		mp[vals[i]] = i;
	}

	std::string op; cin >> op;
	uint64_t cur;
	if (op == "start") {
		cur = 0;
	} else if (op == "resume") {
		cin >> cur;
	} else assert(false);

	int N; cin >> N;
	for (int z = 0; z < N; z++) {
		int v; cin >> v; v--;
		cur ^= vals[v];
		if (cur == 0) {
			cout << 0 << '\n';
		} else {
			auto it = mp.find(cur);
			if (it != mp.end()) cout << it->second+1 << '\n';
			else cout << -1 << '\n';
		}
		cout << flush;
	}

	if (op == "start") {
		cout << cur << '\n';
	} else if (op == "resume") {
		// do nothing
	} else assert(false);

	return 0;
}
