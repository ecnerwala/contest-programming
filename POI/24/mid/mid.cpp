#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<array<int, 2>> ch(N);
	for (auto& a : ch) for (auto& v : a) { cin >> v; v--; }

	vector<int> val(N, -1);

	vector<int> q; q.reserve(N);
	vector<pair<int, int>> inds; inds.reserve(N);
	q.push_back(0);
	while (ch[q.back()][0] != -1) {
		q.push_back(ch[q.back()][0]);
	}
	inds.push_back({0, int(q.size())});
	for (int v = 0; v < int(inds.size()); v++) {
		auto [st, en] = inds[v];

		for (int a = st; a < en; a++) val[q[a]] = v;

		for (int d = 0; d < 2; d++) {
			if (v == 0 && d == 0) continue;
			int nst = int(q.size());
			for (int a = st; a < en; a++) {
				int b = ch[q[a]][d];
				if (b != -1) q.push_back(b);
			}
			if (int(q.size()) - nst > 0) {
				inds.push_back({nst, int(q.size())});
			}
		}
	}

	int Q; cin >> Q;
	while (Q--) {
		int x, y; cin >> x >> y; x--, y--;
		cout << (val[x] >= val[y] ? "TAK" : "NIE") << '\n';
	}

	return 0;
}
