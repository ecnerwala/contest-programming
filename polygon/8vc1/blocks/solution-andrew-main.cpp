#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	int X, Y;
	cin >> X >> Y;
	vector<int> m2;
	vector<int> m3;
	vector<int> m6;
	for(int i = 2; m2.size() < X; i += 2) {
		if(i % 3 == 0) continue;
		m2.push_back(i);
	}
	for(int i = 3; m3.size() < Y; i += 3) {
		if(i % 2 == 0) continue;
		m3.push_back(i);
	}
	for(int i = 6; (!m2.empty() && m2.back() > i) || (!m3.empty() && m3.back() > i); i += 6) {
		m6.push_back(i);
		if(m2.empty()) {
			m3.pop_back();
		} else if(m3.empty()) {
			m2.pop_back();
		} else if(m2.back() > m3.back()) {
			m2.pop_back();
		} else {
			m3.pop_back();
		}
	}
	ll res = 0;
	for(int i : m2) {
		res += i;
	}
	for(int i : m3) {
		res += i;
	}
	for(int i : m6) {
		res += i;
	}
	cout << res << '\n';
	return 0;
}
