#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

const int MAXK = 4e6;
const int MAXL = 4e6;

int K;
int X[MAXK];

int L;
int Y[MAXK];

inline int sign(int a) { return a ? (a > 0 ? 1 : -1) : 0; }

struct num {
	//stores a number in "balanced binary" : note: not unique
	//sorted in increasing order of abs
	vector<int> v; // +- i means +- 2^(i-1), in order of i

	num const operator + (const num &n) {
		num res;
		for(vector<int>::const_iterator i = v.begin(), j = n.v.begin(); i != v.end() || j != n.v.end(); ) {
			int val;
			if(i == v.end()) {
				val = *j;
				j++;
			} else if(j == n.v.end()) {
				val = *i;
				i++;
			} else {
				if(abs(*i) < abs(*j)) {
					val = *i;
					i++;
				} else {
					val = *j;
					j++;
				}
			}
			//cerr << val << '\n';
			if(res.v.empty()) {
				res.v.push_back(val);
			} else if(res.v.back() == val) {
				if(val > 0) res.v.back() ++;
				else res.v.back() --;
			} else if(res.v.back() == -val) {
				res.v.pop_back();
			} else {
				if(abs(res.v.back()) > abs(val)) {
					int tmp = res.v.back();
					res.v.back() = val;
					res.v.push_back(tmp);
				} else {
					res.v.push_back(val);
				}
			}
		}
		return res;
	}

	void operator ++() {
		if(v.empty()) {
			v.push_back(1);
		} else if(v.front() == -1) {
			v.erase(v.begin());
		} else if(v.front() == 1) {
			int i = 1;
			while(i <= v.size() && v[i - 1] == i) i++;
			//i >= 2
			if(i > v.size()) {
				v.clear();
				v.push_back(i);
			} else if(v[i - 1] == -i){
				v.erase(v.begin(), v.begin() + i); //cancel things
			} else {
				v[i - 2] = i;
				v.erase(v.begin(), v.begin() + i - 2); //insert things
			}
		} else {
			v.insert(v.begin(), 1);
		}
	}

	void operator --() {
		if(v.empty()) {
			v.push_back(-1);
		} else if(v.front() == 1) {
			v.erase(v.begin());
		} else if(v.front() == -1) {
			int i = 1;
			while(i <= v.size() && v[i - 1] == -i) i++;
			//i >= 2
			if(i > v.size()) {
				v.clear();
				v.push_back(-i);
			} else if(v[i - 1] == i){
				v.erase(v.begin(), v.begin() + i); //cancel things
			} else {
				v[i - 2] = -i;
				v.erase(v.begin(), v.begin() + i - 2); //insert things
			}
		} else {
			v.insert(v.begin(), -1);
		}
	}
	
	//divide by 2^cnt
	void div2(int cnt = 1) {
		int i = 0;
		while(i < v.size() && abs(v[i]) <= cnt) {
			i++;
		}
		bool dec = i > 0 && v[i - 1] < 0;
		v.erase(v.begin(), v.begin() + i);
		for(vector<int>::iterator it = v.begin(); it != v.end(); it++) {
			if(*it > 0) *it -= cnt;
			else *it += cnt;
		}
		if(dec) operator--();
	}

	num operator-() {
		num res = *this;
		for(vector<int>::iterator it = res.v.begin(); it != res.v.end(); it++) {
			*it *= -1;
		}
		return res;
	}

	inline void print() {
		for(int i = 0; i < v.size(); i++) cerr << v[i] << ' '; cerr << '\n';
	}

	void makeAlt() {
		vector<int> res;
		int prv = v.back(); v.pop_back();
		while(true) {
			int val;
			bool bk = false;
			if(v.empty()) {
				val = 1;
				bk = true;
			} else {
				val = v.back(); v.pop_back();
			}
			if(sign(val) == sign(prv)) {
				int p = prv;
				if(p < 0) p--;
				else p++;
				if(!res.empty() && res.back() == -p) {
					res.pop_back();
				} else {
					res.push_back(p);
				}
				res.push_back(-prv);
			} else {
				res.push_back(prv);
			}
			if(bk) break;
			prv = val;
		}
		reverse(res.begin(), res.end());
		v = res;
	}

	num() {}
	
	num(int val) {
		for(int i = 1; val; i++, val /= 2) {
			if(val % 2) v.push_back(i);
		}
	}
};

num N;
num D;
num res;

void go() {
	if(K % 2 == 1) X[K ++] = 0;
	if(K == 2) {
		if(X[0] + X[1] == 1) {
			L = 1;
			Y[0] = 1;
			return;
		} else if(X[0] + X[1] == 2) {
			L = 2;
			Y[0] = 1;
			Y[1] = 1;
			return;
		}
	}
	int sum = 0;
	for(int i = K - 1; i >= 0; i--) {
		sum += X[i];
		if(i % 2) {
			N.v.push_back(-(sum + 1));
		} else {
			N.v.push_back(sum + 1);
		}
	}
	D = N + num(K);
	D.div2(1);
	if(X[0] == 1) {
		//remove the first two terms of 2^a - 2^(a - 1)
		num tmp;
		tmp.v.push_back(1);
		tmp.v.push_back(-(sum - 2 + 1));
		N = N + tmp;
	} else {
		num tmp;
		tmp.v.push_back((sum - 1 + 1));
		N = tmp;
	}
	cerr << N.v.size() << '\n';
	cerr << D.v.size() << '\n';
	res = N + D;
	res = res + (- num(sum));
	cerr << res.v.size() << '\n';
	//res.print();
	res.makeAlt();
	cerr << res.v.size() << '\n';
	//res.print();
	L = res.v.size();
	for(int i = 0; i < L; i ++) {
		Y[i] = abs(res.v[L - 1 - i]);
	}
	Y[L] = 1;
	for(int i = 0; i < L; i++) {
		Y[i] -= Y[i + 1];
	}
	if(Y[L - 1] == 0) L--;
}

int main() {
	if(fopen("jed.in", "r")) {
		freopen("jed.in", "r", stdin);
		freopen("jed.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);
	cin >> K;
	for(int i = 0; i < K; i++) cin >> X[i];

	go() ;

	cout << L << '\n';
	cout << Y[0];
	for(int i = 1; i < L; i++) cout << ' ' << Y[i];
	cout << '\n';
}
