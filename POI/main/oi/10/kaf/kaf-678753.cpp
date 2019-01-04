#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int BASE = 1e9;
const int L = 9;
const int MAXN = 65;
const int N = 60;

struct num {
	ll v[MAXN];

	num() {
		memset(v, 0, sizeof(v));
	}

	num(string s) {
		memset(v, 0, sizeof(v));
		int sz = int(s.size());
		for(int i = sz - 1; i >= 0; i--) {
			v[i / L] *= 10;
			v[i / L] += s[sz - 1 - i] - '0';
		}
	}

	num& operator += (const num& b) {
		for(int i = 0; i < N; i++) {
			v[i] += b.v[i];
			if(v[i] >= BASE) {
				v[i] -= BASE;
				v[i + 1] ++;
			}
		}
		return *this;
	}

	num& operator -= (const num& b) {
		for(int i = 0; i < N; i++) {
			v[i] -= b.v[i];
			if(v[i] < 0) {
				v[i] += BASE;
				v[i + 1] --;
			}
		}
		return *this;
	}
	
};


bool nz(const num& a) {
	for(int i = 0; i < N; i++) {
		if(a.v[i]) return true;
	}
	return false;
}
bool operator == (const num &a, const num &b) {
	for(int i = 0; i < N; i++) {
		if(a.v[i] != b.v[i]) return false;
	}
	return true;
}

bool operator < (const num &a, const num &b) {
	for(int i = N - 1; i >= 0; i--) {
		if(a.v[i] < b.v[i]) return true;
	}
	return false;
}

bool operator > (const num &a, const num &b) {
	return b < a;
}

bool operator >= (const num &a, const num &b) {
	return !(a < b);
}

bool operator <= (const num &a, const num &b) {
	return !(a > b);
}

num& operator %= (num &a, const num& b) {
	while(a >= b) {
		a -= b;
	}
	return a;
}


num n, l, k, tmp;
void go() {
	while(true) {
		if(l == k) {
			return;
		}
		if(l > k) {
			swap(l, k);
		}
		// very low cycles
		if((tmp = k, tmp += k) <= n) {
			while(nz(l)) {
				k %= l;
				swap(k, l);
			}
			swap(l, k);
			return;
		} else if((tmp = l, tmp += l) > n) {
			l += k;
			l -= n;
			return;
		} else {
			n -= k;
			if(n >= l) {
				while(nz(l)) {
					k %= l;
					swap(k, l);
				}
				swap(l, k);
				return;
			} else {
				k %= l;
				if(!nz(k)) {
					return;
				}
				if(k >= n) {
					l -= n;
					return;
				}
				tmp = l;
				tmp -= k;
				if(n <= tmp) {
					l -= n;
					return;
				}
				l = tmp;
				//return go(n, l, k);
			}
		}
	}
}

string print(const num &a) {
	if(!nz(a)) {
		return "0";
	}
	vector<char> s;
	for(int i = 0; i < N; i++) {
		int v = a.v[i];
		for(int j = 0; j < L; j++) {
			s.push_back(char('0' + v % 10));
			v /= 10;
		}
	}
	while(!s.empty() && s.back() == '0') {
		s.pop_back();
	}
	return string(s.rbegin(), s.rend());
}

int main() {
	string a, b, c; cin >> a >> b >> c;
	n = num(a);
	l = num(b);
	k = num(c);
	go();
	cout << print(l) << '\n';
	return 0;
}

/*
int par[2000];
inline int getpar(int a) { return par[a] == -1 ? a : (par[a] = getpar(par[a])); }
inline void merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if(a != b) par[a] = b;
}

int slow() {
	memset(par, -1, sizeof(par));
	for(int i = 0; i + l < n; i++) {
		merge(i, i + l);
	}
	for(int i = 0; i + k < n; i++) {
		merge(i, i + k);
	}
	int res = 0;
	for(int i = 0; i < n; i++) {
		if(par[i] == -1) {
			res++;
		}
	}
	return res;
}

void test() {
	for(int a = 1; a <= 140; a++) {
		for(int b = 1; b <= a; b++) {
			for(int c = 1; c <= a; c++) {
				n = a, l = b, k = c;
				int s = slow();
				go();
				int g = l;
				if(g != s) {
					cerr << n << ' ' << l << ' ' << k << '\n';
					cerr << g << ' ' << s << '\n';
					assert(false);
				}
			}
		}
	}
	cerr << "all good\n";
}
*/

