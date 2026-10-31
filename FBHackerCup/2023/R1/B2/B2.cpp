#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<std::vector<std::vector<int>>> parts(42);
	parts[0].push_back({});
	for (int i = 1; i <= 41; i++) {
		for (int v = 0; i+v <= 41; v++) {
			for (auto a : parts[v]) {
				a.push_back(i);
				parts[v+i].push_back(a);
			}
		}
	}

	std::map<int, std::vector<int>> m;
	for (const auto& v : parts[41]) {
		int prod = 1;
		for (auto a : v) prod *= a;
		if (!m.count(prod)) {
			m[prod] = v;
		} else {
			auto& o = m[prod];
			if (v.size() < o.size()) {
				o = v;
			}
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int P; cin >> P;
		cout << "Case #" << case_num << ": ";
		if (m.count(P)) {
			cout << m[P].size();
			for (int a : m[P]) {
				cout << " " << a;
			}
			cout << '\n';
		} else {
			cout << -1 << '\n';
		}

	}

	return 0;
}
