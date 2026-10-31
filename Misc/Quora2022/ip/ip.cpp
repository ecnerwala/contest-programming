#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> ch;
	ch.reserve(32*N);
	std::vector<std::pair<uint32_t, int>> val;
	val.reserve(32*N);
	std::vector<bool> has_all;
	has_all.reserve(32*N);

	ch.push_back({-1, -1});
	val.push_back({0, 0});
	has_all.push_back(false);

	for (int i = 0; i < N; i++) {
		uint32_t v; int e;
		{
			char filler;
			uint32_t a, b, c, d; cin >> a >> filler >> b >> filler >> c >> filler >> d >> filler >> e;
			v = a << 24 | b << 16 | c << 8 | d;
		}

		int cur = 0;
		for (int z = 0; z < e; z++) {
			bool d = (v >> (31-z)) & 1;
			if (ch[cur][d] == -1) {
				ch[cur][d] = int(ch.size());
				val.push_back({v >> (31-z) << (31-z), z+1});
				ch.push_back({-1, -1});
				has_all.push_back(false);
			}
			cur = ch[cur][d];
		}
		has_all[cur] = true;
	}

	for (int it = 0; it < int(ch.size()); it++) {
		if (has_all[it]) {
			for (int d = 0; d < 2; d++) {
				if (ch[it][d] != -1) {
					has_all[ch[it][d]] = true;
				}
			}
		}
	}
	for (int it = int(ch.size())-1; it >= 0; it--) {
		if (ch[it][0] != -1 && ch[it][1] != -1 && has_all[ch[it][0]] && has_all[ch[it][1]]) {
			has_all[it] = true;
		}
		if (has_all[it]) {
			for (int d = 0; d < 2; d++) {
				if (ch[it][d] != -1) {
					has_all[ch[it][d]] = false;
				}
			}
		}
	}

	for (int i = 0; i < int(ch.size()); i++) {
		if (has_all[i]) {
			uint32_t v = val[i].first;
			cout << ((v >> 24) & 255) << '.' << ((v >> 16) & 255) << '.' << ((v >> 8) & 255) << '.' << (v & 255) << '/' << val[i].second << '\n';
		}
	}

	return 0;
}
