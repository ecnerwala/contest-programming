#include<iostream>
#include<cstring>
#include<deque>
#include<algorithm>
#include<cassert>

using namespace std;

typedef long long ll;

const int MAXN = 1000000;
const ll INF = 1ll << 60;

template <class T> void setmax(T &a, T b) { if( b > a ) a = b; }

int N;
ll H[MAXN];
ll P[MAXN];

deque<ll> dq;

void insert(ll v) {
	while(!dq.empty() && dq.back() < v) dq.pop_back();
	dq.push_back(v);
}

void remove(ll v) {
	assert(!dq.empty());
	if(dq.front() == v) dq.pop_front();
}

ll getMax() {
	assert(!dq.empty());
	return dq.front();
}

void scan(int s, int t, int v) {
	//range in queue: [s, t]
	for(int i = s; i <= t; i ++) {

		insert(H[i]);
	}

	for(int i = 0; s < N; i++) {
		setmax(P[i], getMax() + v);

		remove(H[s]);
		s++, t++;
		if(t < N) insert(H[t]);
	}
}

void go() {
	memcpy(P, H, sizeof(H));

	for(int z = 0; z < 2; z++) {
		
		int v = 0;
		int s = 0;
		int t = 0;
		while(s < N) {
			v ++;
			s = t + 1;
			t = v * v;
			scan(s, t, v);
		}

		reverse(H, H + N);
		reverse(P, P + N);
	}
	
	for(int i = 0; i < N; i++) P[i] -= H[i];
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N; i++) cin >> H[i];

	go();

	for(int i = 0; i < N; i++) cout << P[i] << '\n';
}
