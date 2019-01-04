#include<iostream>
#include<cstring>
#include<deque>
#include<algorithm>
#include<cassert>
#include<cmath>

using namespace std;

typedef long long ll;

const int MAXN = 1000000;
const ll INF = 1ll << 60;

template <class T> void setmax(T &a, T b) { if( b > a ) a = b; }

int N;
ll H[MAXN];
ll P[MAXN];

ll sqrt(ll a) {
	return ceil(sqrt(double(a)));
}

ll sq(ll a) { return a * a; }

//returns ceil(x/y) for x, y positive
ll ceil(ll x, ll y) {
	return x / y + bool(x % y);
}

ll dist(ll a, ll b) {
	assert(a < b);

	ll d = b - a;
	ll h = H[b] - H[a];
	if(h <= 0) return INF;
	if(h * h >= d) return b; 

	return b + ceil(sq(d - h * h), sq(2 * h));
}


void go() {
	memcpy(P, H, sizeof(H));

	for(int z = 0; z < 2; z++) {
		deque<ll> dq; // stores index
		
		for(ll i = 0; i < N; i++) {

			while(dq.size() >= 2) {
				ll b = dq.back(); dq.pop_back();
				ll a = dq.back();

				if(dist(a, i) <= dist(b, i)) {
					//pop off b
				} else {
					dq.push_back(b);
					break;
				}
			}

			dq.push_back(i);

			//pop of used things
			while(dq.size() >= 2) {
				ll a = dq.front(); dq.pop_front();
				ll b = dq.front();
				if(i > dist(a, b)) {
					// pop off a
				} else {
					// undo
					dq.push_front(a);
					break;
				}
			}

			setmax(P[i], sqrt(i - dq.front()) + H[dq.front()]);

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
