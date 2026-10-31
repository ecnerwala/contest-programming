#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cout << 12 << '\n';
	cout << 2 << ' ' << 3;
	vector<int> p7(10); p7[0] = 1; for (int i = 1; i < int(p7.size()); i++) p7[i] = p7[i-1] * 7;
	for (int i = 0; i <= 9; i++) cout << ' ' << p7[i];
	cout << '\n';

	int Q; cin >> Q;
	while (Q--) {
		vector<int> base7(10);
		int A; cin >> A;
		for (int i = 0; i <= 9; i++) {
			int d = A % 7;
			while (d > 3) d -= 7;
			while (d < -3) d += 7;
			base7[i] = d;
			A -= d;
			assert(A % 7 == 0);
			A /= 7;
		}
		assert(!A);
		vector<string> digs(4);
		for (int i = 9; i >= 0; i--) {
			if (base7[i]) {
				if (abs(base7[i]) > 1 && !digs[abs(base7[i])].empty() && digs[abs(base7[i])][0] == '-') base7[i] *= -1;
				digs[abs(base7[i])] += (base7[i] > 0 ? "+" : "-") + to_string(p7[i]);
			}
		}
		string pos;
		string neg;
		for (int v = 1; v <= 3; v++) {
			if (digs[v].empty()) continue;
			if (v > 1) {
				digs[v] = digs[v].substr(0, 1) + to_string(v) + "*(" + digs[v].substr(1) + ")";
			}
			if (digs[v][0] == '+') pos += digs[v];
			else if (digs[v][0] == '-') neg += digs[v];
			else assert(false);
		}
		assert(!pos.empty());
		cout << pos.substr(1) + neg << '\n';
	}
}
