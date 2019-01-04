#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>
#include<cassert>

using namespace std;

#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;

const int MAXN = 600000;
const int MAXM = 600000;
const int MAXK = 600000;

int N, M, K;

int O[MAXM];
vector<int> S[MAXN];
ll P[MAXN];

int L[MAXK], R[MAXK];
ll V[MAXK];

int T;
int W[MAXN];

bool inRange(int l, int r, int p) {
	if(l <= r) return l <= p && p <= r;
	else return p <= r || l <= p;
}

void go() {
	for(int i = 0; i < N; i++) W[i] = -1;

	for(T = 0; T * T < K; T++);
	
	for(int a = 0; a * T < K; a++) {
		//cerr << a << '\n';
		vector<pair<int, ll> > evts;
		for(int b = 0; b < min(T, K - a * T); b++) {
			int l = L[a * T + b];
			int r = R[a * T + b];
			ll v = V[a * T + b];

			if(l <= r) {
				evts.push_back(make_pair((l),		+v));
				evts.push_back(make_pair((r + 1),	-v));
			} else {
				evts.push_back(make_pair((0),		+v));
				evts.push_back(make_pair((r + 1),	-v));
				evts.push_back(make_pair((l),		+v));
				evts.push_back(make_pair((M),		-v));
			}
			
		}

		sort(evts.begin(), evts.end());

		vector<int> proc;
		ll num = 0;
		int ind = 0;
		for(int i = 0; i < M; i++) {
			while(ind < evts.size() && evts[ind].A <= i) {
				//cerr << i << ' ' << evts.top().B << '\n';
				num += evts[ind].B;
				ind++;
			}

			assert(num >= 0);

			if(W[O[i]] == -1) {
				if(P[O[i]] > 0 && P[O[i]] <= num) proc.push_back(O[i]);
				P[O[i]] -= num;
			}
		}

		for(int i = 0; i < proc.size(); i++) {
			int n = proc[i];//nation

			assert(W[n] == -1);

			//cerr << "processing " << n << " with a = " << a << " and P = " << P[n] << '\n';

			for(int b = min(T, K - a * T) - 1; b >= 0; b--) {
				int l = L[a * T + b];
				int r = R[a * T + b];
				ll v = V[a * T + b];
				
				for(int j = 0; j < S[n].size(); j++) {
					int s = S[n][j];// sector

					if((l <= r && (l <= s && s <= r)) || (l > r && (s <= r || l <= s))) {
						P[n] += v;
					}

				}

				//cerr << "processing " << n << " with a = " << a << " b = " << b << " and P = " << P[n] << '\n';

				if(P[n] > 0) {
					W[n] = a * T + b;
					break;
				}
			}

			assert(P[n] > 0);

			P[n] = 0;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M;

	for(int i = 0; i < M; i++) {
		cin >> O[i];
		O[i] --;
		S[O[i]].push_back(i);
	}

	for(int i = 0; i < N; i++) {
		cin >> P[i];
	}

	cin >> K;

	for(int i = 0; i < K; i++) {
		cin >> L[i] >> R[i] >> V[i];
		L[i]--, R[i]--;
	}

	go();

	for(int i = 0; i < N; i++) {
		if(W[i] == -1) cout << "NIE" << '\n';
		else cout << (W[i] + 1) << '\n';
	}
}
