#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXT = 30;
const int MAXR = 30;

int T, R;

int S[MAXT];
int BP[MAXT];
vector<int> ops[MAXT]; // positive is lock, negative is unlock, 0 is compute

int CP[MAXT];

int PC[MAXR];
int owner[MAXR];

int CT;

int finish[MAXT];
int num_finished;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> T >> R;
	memset(PC, 0, sizeof(PC));
	for(int i = 1; i <= T; i++) {
		cin >> S[i] >> BP[i];
		int a;
		cin >> a;
		while(a--) {
			char c; int n; cin >> c >> n;
			if(c == 'C') {
				while(n--) {
					ops[i].push_back(0);
				}
			} else if(c == 'L') {
				ops[i].push_back(n);
				PC[n] = max(PC[n], BP[i]);
			} else if(c == 'U'){
				ops[i].push_back(-n);
			} else assert(false);
		}
		reverse(ops[i].begin(), ops[i].end());
	}

	CT = 0;
	while(num_finished < T) {
		priority_queue<pii> pq; // curr priority, task
		for(int i = 1; i <= T; i++) {
			if(S[i] <= CT && !ops[i].empty()) {
				CP[i] = BP[i];
				pq.emplace(CP[i], i);
			}
		}
		while(true) {
			if(pq.empty()) {
				CT++;
				break;
			}
			int cur = pq.top().second;
			if(pq.top().first < CP[cur]) continue;
			pq.pop();
			// see if any other task blocks it
			if(ops[cur].back() <= 0) {
				if(ops[cur].back() == 0) {
					CT++;
				} else {
					owner[-ops[cur].back()] = 0;
				}
				ops[cur].pop_back();
				if(ops[cur].empty()) finish[cur] = CT, num_finished++;
				break;
			} else {
				int k = ops[cur].back();
				bool blocked = false;
				for(int l = 1; l <= R; l++) {
					if(owner[l] && ((l == k) || (owner[l] != cur && PC[l] >= CP[cur]))) {
						blocked = true;
						int nxt = owner[l];
						if(CP[nxt] < CP[cur]) {
							CP[nxt] = CP[cur];
							pq.emplace(CP[nxt], nxt);
						}
					}
				}
				if(!blocked) {
					owner[k] = cur;
					ops[cur].pop_back();
					assert(!ops[cur].empty());
					break;
				}
			}
		}
	}

	for(int i = 1; i <= T; i++) {
		assert(finish[i]);
		cout << finish[i] << '\n';
	}

}
