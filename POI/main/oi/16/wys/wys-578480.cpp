#include<bits/stdc++.h>

using namespace std;

const int MAXL = 30;


// efficient mod for small values
inline int mod(int a, int m) { return a >= m ? a - m : a; }

int f[MAXL];  // failure array
string lexM(string s) {
	int n = s.size();
	s += s;

	int res = 0;
	memset(f, 0, sizeof(f));
	for(int cnd = 0, pos = 1; pos < 2 * n; ) {
		if(s[pos] == s[res + cnd]) {
			cnd++;
			pos++;
			f[pos - res] = cnd;
		} else {
			if(s[pos] < s[res + cnd]) {
				res = pos - cnd;
			}
			if(cnd == 0) {
				pos++;
				cnd = f[pos - res] = 0;
			} else {
				cnd = f[cnd];
			}
		}
	}

	return s.substr(res, n);
}

string normalize(string s) {
	string res = lexM(s);
	reverse(s.begin(), s.end());
	return min(res, lexM(s));
}

void filter(vector<string> &v) {
	for(int i = 0; i < int(v.size()); i++) v[i] = normalize(v[i]);
	sort(v.begin(), v.end());
	v.resize(unique(v.begin(), v.end()) - v.begin());
}

void append(vector<string> &res, const string &s) {
	// assume s normalized
	// s does not end with a's (not all a's)
	int sz = s.size();
	for(int i = 0; i < sz; i++) {
		string n = s;
		if(n[i] == 'a') continue;
		if(i + 1 == sz && n[0] == 'a' && n[1] == 'a') {
			//wee, we have 2.4
			int l = n[i] - 'c';
			int r = n[2] - 'c';
			n[i] = char((l + r - 4) + 'c');
			n = n.substr(3); // chop off first 2 a's & c
		} else if(n[mod(i + 1, sz)] == 'a'){
			n[i] --;
			n[mod(i + 2, sz)] --;
			n.erase(n.begin() + mod(i + 1, sz));
		} else {
			n[i] --;
			n[mod(i + 1, sz)] --;
			n.insert(n.begin() + i + 1, 'e');
		}
		res.push_back(n);
	}
}

void print(const vector<string> &res) {
	cout << res.size() << '\n';
	cout << res[0];
	for(int i = 1; i < int(res.size()); i++) {
		cout << ' ' << res[i];
	}
	cout << '\n';
}

vector<string> bfs[12];

int main() {
	bfs[1].push_back("eee");
	for(int i = 1; i < 10; i++) {
		for(int j = 0; j < int(bfs[i].size()); j++) {
			append(bfs[i + 1], bfs[i][j]);
		}
		filter(bfs[i + 1]);
	}
	//cerr << 1 << ' '; print(bfs[1]);
	//cerr << 2 << ' '; print(bfs[2]);
	//cerr << 3 << ' '; print(bfs[3]);

	int T; cin >> T;
	for(int t = 0; t < T; t++) {
		char type;
		cin >> type;
		//cerr << "hi\n";
		if(type == 'K') {
			string s; cin >> s;
			s = normalize(s);
			vector<string> res;
			append(res, s);
			filter(res);
			print(res);
		} else {
			int d; cin >> d;
			print(bfs[d]);
		}
	}
	return 0;
}
