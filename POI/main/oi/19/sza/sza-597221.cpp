#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1.2e3;
int N;
pair<pii, int> items[MAXN];

const int MAXV = 1.2e5;
int V = 1e5 + 20;

const int INF = 2e9;

int maxtime[MAXV]; // max time you have to get this

const int MAXM = 1.5e6;
int M;
pair<pii, int> plans[MAXM];

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> items[i].B >> items[i].A.A >> items[i].A.B;
	}

	cin >> M;
	for(int i = 0; i < M; i++) {
		cin >> plans[i].A.A >> plans[i].B >> plans[i].A.B;
		plans[i].A.B += plans[i].A.A;
	}

	sort(items, items + N);
	sort(plans, plans + M);

	maxtime[0] = INF;
	
	for(int i = 0, j = 0; i < M; i++) {
		int t = plans[i].A.A;
		while(j < N && items[j].A.A <= t) {

			int v = items[j].B;
			int s = items[j].A.B;

			for(int i = V - v; i >= 0; i--) {
				maxtime[i + v] = max(maxtime[i + v], min(maxtime[i], s));
			}

			j++;
		}

		bool good = maxtime[plans[i].B] > plans[i].A.B;
		cout << (good ? "TAK" : "NIE") << '\n';
	}

	return 0;
}
