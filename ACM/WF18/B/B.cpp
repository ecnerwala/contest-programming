#include<bits/stdc++.h>
using namespace std;

string L;

const int MAXW = 1e6;
int W;
string words[MAXW];
unordered_map<string, int> mp;

bool succ[MAXW];
bool prec[MAXW];
vector<int> nxt[MAXW];
vector<int> prv[MAXW];

vector<int> seeds;

vector<vector<int>> sentences;

void parse() {
	sentences.emplace_back();
	string cur = "";
	for (char c : L) {
		if ('a' <= c && c <= 'z') {
			cur.push_back(c);
		} else {
			int ind;
			if (!cur.empty()) {
				if (mp.count(cur)) {
					ind = mp[cur];
				} else {
					ind = W++;
					words[ind] = cur;
					mp[cur] = ind;
				}
				sentences.back().emplace_back(ind);
				cur.clear();
			} else {
				ind = -1;
			}
			if (c == ',') {
				assert(ind != -1);
				seeds.push_back(ind);
			} else if (c == '.') {
				assert(ind != -1);
				sentences.emplace_back();
			} else if (c == ' ') {
				// pass
			} else assert(false);
		}
	}
	assert(sentences.back().empty());
	sentences.pop_back();
	for (auto &it : sentences) {
		for (size_t i = 0; i + 1 < it.size(); i++) {
			nxt[it[i]].push_back(it[i+1]);
			prv[it[i+1]].push_back(it[i]);
		}
	}
}

void dfs_prec(int cur);

void dfs_succ(int cur) {
	if (succ[cur]) return;
	succ[cur] = true;
	for (int n : nxt[cur]) {
		dfs_prec(n);
	}
}

void dfs_prec(int cur) {
	if (prec[cur]) return;
	prec[cur] = true;
	for (int n : prv[cur]) {
		dfs_succ(n);
	}
}

void print() {
	string res = "";
	for (auto &it : sentences) {
		assert(!it.empty());
		res += words[it[0]];
		for (size_t i = 1; i < it.size(); i++) {
			if (prec[it[i]]) {
				res.push_back(',');
			}
			res.push_back(' ');
			res += words[it[i]];
		}
		res.push_back('.');
		res.push_back(' ');
	}
	assert(res.back() == ' ');
	res.pop_back();
	cout << res << '\n';
}

int main() {
	ios_base::sync_with_stdio(0);
	std::getline(cin, L);

	parse();

	for (int s : seeds) {
		dfs_succ(s);
	}

	print();

	return 0;
}
