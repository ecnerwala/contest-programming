#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;
const int MAXM = 10;

int N, M;

string S;
int par[MAXN];

vector<int> ch[MAXN];

string cool[MAXM];
int kmp[MAXM][30][30];

int state[MAXM];
double res[MAXM];

void make_kmp(string s, int arr[30][30]) {
	memset(arr[0], 0, sizeof(arr[0]));
	for(int i = 0; i < int(s.size()); i++) {
		int c = s[i] - 'A';
		int t = arr[i][c];
		arr[i][c] = i + 1;
		memcpy(arr[i + 1], arr[t], sizeof(arr[0]));
	}
	for(int i = 0; i < 26; i++) {
		arr[s.size()][i] = int(s.size());
	}
}

string shuffle(string a, string b) {
	string ret;
	while(!a.empty() || !b.empty()) {
		if(rand() % (a.size() + b.size()) < a.size()) {
			ret.push_back(a.back());
			a.pop_back();
		} else {
			ret.push_back(b.back());
			b.pop_back();
		}
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

string gen(int a = 0) {
	string s = "";
	for(int c : ch[a]) {
		s = shuffle(s, gen(c));
	}
	if(a) s += S[a - 1];
	else reverse(s.begin(), s.end());
	return s;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	srand(7);
	for(int case_num = 1; case_num <= T; case_num ++) {

		cin >> N;
		for(int i = 0; i <= N; i++) {
			ch[i].clear();
		}
		for(int i = 1; i <= N; i++) {
			cin >> par[i];
			ch[par[i]].push_back(i);
		}
		cin >> S;
		cin >> M;
		for(int i = 0; i < M; i++) {
			cin >> cool[i];
			make_kmp(cool[i], kmp[i]);
		}

		memset(res, 0, sizeof(res));
		int runs = 1e4;

		unordered_map<string, int> mp;
		for(int z = 0; z < runs; z++) {

			memset(state, 0, sizeof(state));
			vector<int> vals = ch[0];

			string cur = gen();
			//cerr << cur << '\n';
			//mp[cur] ++;

			for(char c : cur) {
				for(int i = 0; i < M; i++) state[i] = kmp[i][state[i]][c - 'A'];
			}

			for(int i = 0; i < M; i++) res[i] += (state[i] == int(cool[i].size()));
		}

		//for(auto it : mp) cerr << it.first << ' ' << double(it.second) / runs << '\n';

		for(int i = 0; i < M; i++) res[i] /= runs;

		cout << "Case #" << case_num << ":";
		for(int i = 0; i < M; i++) {
			cout << ' ' << fixed << setprecision(2) << res[i];
		}
		cout << '\n';
	}

	return 0;
}
