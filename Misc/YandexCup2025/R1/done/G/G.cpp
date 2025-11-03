#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1.1e7);
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	std::vector<bool> powerful(V+1);
	powerful[1] = true;
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
			powerful[i] = false;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) {
				powerful[i*p] = (powerful[i] || powerful[i/p]);
			} else {
				powerful[i*p] = false;
			}
			if (p == pfac[i]) break;
		}
	}
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::pair<int, int>> v; v.reserve(30);
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int a = A[i];
			if (!powerful[a]) {
				v.clear();
				continue;
			}
			{
				v.push_back({a, i});
				int cur = a;
				for (int z = int(v.size())-1; z >= 0; z--) {
					cur = std::gcd(cur, v[z].first);
					v[z].first = cur;
				}
			}
			{
				int nsz = 1;
				for (int z = 1; z < int(v.size()); z++) {
					if (v[z].first == v[nsz-1].first) continue;
					v[nsz] = v[z];
					nsz++;
				}
				v.resize(nsz);
			}
			for (int z = 0; z < int(v.size()); z++) {
				int cnt = (z+1 == int(v.size()) ? i+1 : v[z+1].second) - v[z].second;
				ans += int64_t(cnt) * int64_t(v[z].first);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
