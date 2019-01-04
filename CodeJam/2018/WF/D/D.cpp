#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1100;
const int MAXP = 1100;
int N, P;
const int ME = 0;
bool attacks[MAXN][MAXP];
bool defends[MAXN][MAXP];

const int MAXV = MAXN + MAXP;
int V;
bool adj[MAXV][MAXV];
int scc[MAXV];

int next_ind = 0;
int ind[MAXV];
bool instack[MAXV];
int mindepth[MAXV];
stack<int> stk;
void dfs(int i, int d = 0) {
	assert(ind[i] == -1);
	ind[i] = (next_ind++);
	mindepth[i] = ind[i];
	stk.push(i);
	instack[i] = true;
	for (int j = 0; j < V; j++) {
		if (adj[i][j]) {
			if (ind[j] == -1) {
				dfs(j, d+1);
				mindepth[i] = min(mindepth[i], mindepth[j]);
			} else if (instack[j]) {
				mindepth[i] = min(mindepth[i], mindepth[j]);
			}
		}
	}
	if (mindepth[i] == ind[i]) {
		assert(instack[i]);
		vector<int> v;
		while (true) {
			assert(!stk.empty());
			int j = stk.top(); stk.pop();
			assert(instack[j]);
			instack[j] = false;
			v.push_back(j);
			scc[j] = i;
			if (j == i) break;
		}
	}
}

void do_scc() {
	next_ind = 0;
	for (int i = 0; i < V; i++) {
		instack[i] = false;
		ind[i] = -1;
		mindepth[i] = -1;
		scc[i] = -1;
	}
	for (int i = 0; i < V; i++) {
		if (ind[i] == -1) dfs(i);
		assert(stk.empty());
		assert(scc[i] != -1);
	}
}

int cnt[MAXN];
bool has[MAXP];
bool go() {
	bool res = true;
	// First, is there a stalemate where I can't defend fully against anyone?
	// Look for this final subset who I can't defend fully against (using greedy)
	{
		queue<int> peopleq;
		queue<int> defenseq;

		peopleq.push(ME);

		for (int i = 1; i < N; i++) {
			cnt[i] = 0;
			for (int p = 0; p < P; p++) {
				if (attacks[i][p]) cnt[i] ++;
			}
			if (cnt[i] == 0) peopleq.push(i);
		}
		for (int p = 0; p < P; p++) {
			has[p] = false;
		}

		while (true) {
			if (!peopleq.empty()) {
				int i = peopleq.front(); peopleq.pop();
				assert(i == ME || cnt[i] == 0);
				for (int p = 0; p < P; p++) {
					if (defends[i][p] && !has[p]) {
						has[p] = true;
						defenseq.push(p);
					}
				}
			} else if (!defenseq.empty()) {
				int p = defenseq.front(); defenseq.pop();
				assert(has[p]);
				for (int i = 1; i < N; i++) {
					if (attacks[i][p]) {
						assert(cnt[i] > 0);
						cnt[i] --;
						if (cnt[i] == 0) peopleq.push(i);
					}
				}
			} else {
				break;
			}
		}

		// if anyone can still attack me, I lose
		for (int i = 1; i < N; i++) {
			if (cnt[i] > 0) {
				return false;
				//res = false;
				//cerr << "Fail: attack by " << i << '\n';
			}
		}
	}

	// A stalemate where i can defend someone fully:
	// Then, he has to defend me fully, so I have (learn) all defenses for my attacks
	// If there's someone who can't defend me fully, he has to attack me
	// I learn that attack, so I can defend, so I would win against him
	// Thus, everyone can defend me fully
	// We need to find a subset such that I can't attack anyone with something new
	// Everyone also has to have an attack that I have
	// If I know an attack, everyone succeptible is mine (transitively)
	// Everyone in the survivors has to have an attack which we learn (and thus they can't be succeptible to)
	{
		// if p "survives", then all i with attacks[i][p] must survive
		// if i "survives", then all p with !defends[i][p] must survive
		V = N + P;
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				adj[i][j] = false;
			}
		}
		for (int i = 0; i < N; i++) {
			for (int p = 0; p < P; p++) {
				if (attacks[i][p]) {
					adj[p+N][i] = true;
				}
				if (!defends[i][p]) {
					adj[i][p+N] = true;
				}
			}
		}

		do_scc();

		/*
		cerr << N << ' ' << P << '\n';
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				cerr << adj[i][j] << ' ';
			}
			cerr << '\n';
		}
		cerr << '\n';
		for (int i = 0; i < V; i++) {
			cerr << scc[i] << ' ';
		}
		cerr << '\n';
		*/

		// We only care about scc's with people in it or that point to people
		// Only scc's that don't point to people are attacks which no one knows
		for (int p = 0; p < P; p++) {
			int outdeg = 0;
			for (int i = 0; i < N; i++) {
				if (adj[p+N][i]) outdeg ++;
			}
			if (outdeg == 0) {
				// we'll just set the SCC to -1, as a marker
				scc[p+N] = -1;
			}
		}

		for (int i = 0; i < V; i++) {
			if (scc[i] != i) continue;
			//for (int j = 0; j < V; j++) { if (scc[j] == i) cerr << j << ' '; } cerr << '\n';
			bool isLeaf = true;
			for (int j = 0; j < V; j++) {
				if (scc[j] == i) {
					for (int k = 0; k < V; k++) {
						if (adj[j][k] && scc[k] != i && scc[k] != -1) {
							isLeaf = false;
							break;
						}
					}
					if (!isLeaf) break;
				}
			}
			if (!isLeaf) continue;

			bool hasPerson = false;
			for (int j = 0; j < N; j++) {
				if (scc[j] == i) hasPerson = true;
			}
			// all leafs should have people
			assert(hasPerson);

			if (scc[ME] == i) {
				// this scc implies I must survive (my attacks don't get learned), so not good
				continue;
			}

			bool goodSet = true;
			for (int j = 0; j < N; j++) {
				if (scc[j] != i) continue;
				bool canTeach = false;
				for (int p = 0; p < P; p++) {
					if (attacks[j][p] && scc[p+N] != i) {
						// node j can survive without forcing p to survive, so they can teach us p
						canTeach = true;
						break;
					}
				}
				if (!canTeach) {
					goodSet = false;
					break;
				}
			}
			if (goodSet) {
				return false;
				//res = false;
			}
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> P;
		memset(attacks, false, sizeof(attacks));
		memset(defends, false, sizeof(defends));
		for (int i = 0; i < N; i++) {
			int a, d; cin >> a >> d;
			for (int z = 0; z < a; z++) {
				int p; cin >> p; p--;
				assert(0 <= p && p < P);
				attacks[i][p] = true;
			}
			for (int z = 0; z < d; z++) {
				int p; cin >> p; p--;
				assert(0 <= p && p < P);
				defends[i][p] = true;
			}
		}

		//cerr << "Case #" << case_num << ":" << '\n';
		bool res = go();
		cout << "Case #" << case_num << ": " << (res ? "YES" : "NO") << '\n';
	}

	return 0;
}
