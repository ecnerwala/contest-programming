#include<iostream>
#include<deque>
#include<queue>

using namespace std;

const int MAXN = 3e6 + 20;
int N;
int T;
int P[MAXN];

deque<int> minQ, maxQ;

int main() {
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
			if(!minQ.empty() && minQ.front() == v) minQ.pop_front();
			if(!maxQ.empty() && maxQ.front() == v) maxQ.pop_front();
			l++;
		}
		int cnd = r - l + 1;
		if(cnd > res) res = cnd;
	}
	cout << res << '\n';
	return 0;
}
