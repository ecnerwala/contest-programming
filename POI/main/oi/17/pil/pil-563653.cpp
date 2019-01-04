#include<iostream>
#include<deque>
#include<queue>
#include<cassert>

using namespace std;

const int MAXN = 3e6 + 20;
int N;
int T;
int P[MAXN];

struct dq {
	int v[MAXN];
	int l, r;
	
	bool empty() { return l == r; }
	int back() {
		return v[r - 1];
	}
	void pop_back() {
		r--;
	}
	void push_back(int a) {
		v[r++] = a;
	}
	int front() {
		return v[l];
	}
	void pop_front() {
		l++;
	}
	
	dq() {
		l = 0, r = 0;
	}
} minQ, maxQ;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> T >> N;
	for(int i = 0; i < N; i++)  {
		cin >> P[i];
	}

	int res = 0;
	for(int l = 0, r = 0; r < N; r++) {
		int v = P[r];
		while(!minQ.empty() && minQ.back() > v) minQ.pop_back();
		minQ.push_back(v);
		while(!maxQ.empty() && maxQ.back() < v) maxQ.pop_back();
		maxQ.push_back(v);
		while(maxQ.front() - minQ.front() > T) {
			v = P[l];
			assert(!minQ.empty());
			assert(!maxQ.empty());
			if(minQ.front() == v) minQ.pop_front();
			if(maxQ.front() == v) maxQ.pop_front();
			l++;
		}
		int cnd = r - l + 1;
		if(cnd > res) res = cnd;
	}
	cout << res << '\n';
	return 0;
}
