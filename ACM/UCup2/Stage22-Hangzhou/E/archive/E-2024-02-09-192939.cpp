#include <bits/stdc++.h>

using freq_t = std::valarray<int>;
struct container_t {
	int L;
	freq_t f;
	friend bool operator < (const container_t& a, const container_t& b) {
		return a.L < b.L;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<container_t> strings(N);
		for (int i = 0; i < N; i++) {
			std::string s; cin >> s;
			strings[i].L = int(s.size());
			strings[i].f = freq_t(0, 26);
			for (char c : s) strings[i].f[c - 'a']++;
		}
		[&]() -> void {
			std::priority_queue<container_t> pq;
			for (int i = N-1; i >= 0; i--) {
				while (!pq.empty() && pq.top().L >= strings[i].L) {
					auto v = pq.top(); pq.pop();
					int k = v.L / strings[i].L;
					v.L -= k * strings[i].L;
					v.f -= k * strings[i].f;
					if (v.f.min() < 0) {
						cout << "NO" << '\n';
						return;
					}
					pq.emplace(std::move(v));
				}
				pq.push(strings[i]);
			}
			pq.push(container_t{0, freq_t(0, 26)});

			std::string S0(strings[0].L, '?');
			int idx = strings[0].L;
			container_t cur = strings[0];
			while (!pq.empty()) {
				auto nxt = pq.top(); pq.pop();
				assert(nxt.L <= cur.L);
				freq_t diff = (cur.f - nxt.f);
				if (diff.min() < 0) {
					cout << "NO" << '\n';
					return;
				}
				for (int z = 0; z < 26; z++) {
					for (int a = 0; a < diff[z]; a++) {
						S0[--idx] = char('a' + z);
					}
				}
				cur = std::move(nxt);
			}
			assert(idx == 0);
			cout << "YES" << '\n';
			cout << S0 << '\n';
			for (int i = 1; i < N; i++) {
				std::string nxt(strings[i].L, '?');
				for (int z = 0; z < strings[i].L; z++) {
					nxt[z] = S0[z % int(S0.size())];
				}
				S0 = std::move(nxt);
				cout << S0 << '\n';
			}
		}();
	}

	return 0;
}
