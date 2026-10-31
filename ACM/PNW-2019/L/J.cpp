#include<bits/stdc++.h>
using namespace std;

void FAIL() {
	cout << -1 << '\n';
	exit(0);
}

int rt(int a, int b) {
	//cerr << a << ' ' << b << '\n';
	for (int i = 0; i < b; i++) {
		if (i * i % b == a) return i;
	}
	//cerr << "FAILING" << '\n';
	FAIL();
	return -1;
}

vector<int> rt(vector<int> a, int b) {
	while (!a.empty() && a.back() == 0) {
		a.pop_back();
	}
	if (a.empty()) return {};

	int deg = int(a.size()) - 1;
	if (deg % 2 != 0) FAIL();

	reverse(a.begin(), a.end()); // for convenience

	vector<int> res(deg / 2 + 1);
	if (b == 2) {
		for (int i = 0; i <= deg / 2; i++) {
			res[i] = a[i * 2];
		}
	} else {
		res[0] = rt(a[0], b);
		for (int i = 1; i <= deg / 2; i++) {
			int v = a[i];
			for (int j = 1; j < i; j++) {
				v -= res[j] * res[i - j];
			}
			v %= b;
			if (v < 0) v += b;
			res[i] = 0;
			while (res[i] * 2 * res[0] % b != v) {
				res[i]++;
			}
		}
	}

	//for (int i = 0; i <= deg; i++) { cerr << a[i] << ' '; } cerr << '\n';
	//for (int i = 0; i <= deg / 2; i++) { cerr << res[i] << ' '; } cerr << '\n';

	vector<int> r2(deg+1);
	for (int i = 0; i <= deg / 2; i++) {
		for (int j = 0; j <= deg / 2; j++) {
			r2[i+j] += res[i] * res[j];
			r2[i+j] %= b;
		}
	}
	//cerr << "r2 == a -> " << (r2 == a) << '\n';
	if (r2 != a) FAIL();

	reverse(res.begin(), res.end());
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string s; cin >> s;
	reverse(s.begin(), s.end());
	vector<int> m5;
	vector<int> m2;
	for (int i = 0; i < int(s.size()); i++) {
		int d = s[i] - '0';
		m5.push_back(d % 5);
		m2.push_back(d % 2);
	}
	while (!m5.empty() && m5.back() == 0) {
		m5.pop_back();
	}
	while (!m2.empty() && m2.back() == 0) {
		m2.pop_back();
	}

	auto v2 = rt(m2, 2);
	auto v5 = rt(m5, 5);
	while (v2.size() < v5.size()) v2.push_back(0);
	while (v5.size() < v2.size()) v5.push_back(0);
	int N = int(v2.size());
	string answer;
	for (int z = 0; z < 2; z++) {
		string cur;
		for (int i = 0; i < N; i++) {
			int v = v5[i];
			if (v % 2 != v2[i]) v += 5;
			cur += char('0' + v);
		}
		reverse(cur.begin(), cur.end());
		if (answer.empty() || cur < answer) answer = cur;

		for (auto& it : v5) {
			it = (5 - it) % 5;
		}
	}
	cout << answer << '\n';

	return 0;
}
