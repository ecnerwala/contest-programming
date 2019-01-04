#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
#include<cassert>

using namespace std;

typedef vector<int> vi;

const int MAXN = 400000;

vector<int> kmp_table(string s) {
	vector<int> f(s.size() + 1);
	f[0] = -1;
	f[1] = 0;
	int pos = 2, cnd = 1;
	while(pos <= s.size()) {
		if(cnd == 0) {
			f[pos] = 0;
			pos++;
			cnd++;
		} else if(s[pos - 1] == s[cnd - 1]) {
			f[pos] = cnd;
			pos++;
			cnd++;
		} else {
			cnd = f[cnd - 1] + 1;
		}
	}
	return f;
}

vector<int> periods(string s){
	vector<int> kmp = kmp_table(s);
	
	vector<int> per;
	for(int cur = kmp[s.size()]; cur; cur = kmp[cur]) {
		per.push_back(s.size() - cur);
	}

	return per;
}

string find(int n, vi p) {
	//cerr << n << '\n';
	//for(int i = 0; i < p.size(); i++) cerr << p[i] << ' '; cerr << '\n';

	if(n == 1) return string(1, '0');
	if(p.size() == 0) return string(n-1, '0') + '1';

	int a = p[0];
	if(a == 1) return string(n, '0');

	int m;
	if(a * 2 <= n) {
		m = a + n % a;
	} else {
		m = n - a;
	}

	vi p2;
	if(a < m) p2.push_back(a);
	for(int i = 1; i < p.size(); i++) {
		int b = p[i];
		if(b % a == 0) continue;
		p2.push_back(m - n + b);
	}
	sort(p2.begin(), p2.end());

	string s = find(m, p2);

	//cerr << s << '\n';

	if(a * 2 <= n) {
		string rep = s.substr(0, a);
		while(s.size() < n) s = rep + s;
	} else {
		s = s + string(2 * a - n, '0') + s;
		if(periods(s) != p) s[n - a] = '1';
	}
	
	return s;
}

int main() {
	int k; cin >> k;

	for(int i = 0; i < k; i++) {
		string s; cin >> s;
		cout << find(s.size(), periods(s)) << '\n';
	}

	return 0;
}
