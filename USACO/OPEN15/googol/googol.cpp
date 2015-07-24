#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef int num;

typedef pair<num, num> pii;
#define A first
#define B second

const ll __N = 46550;

const int MAXS = 2e5;
int S;
string strs[MAXS];
map<string, int> idx;
pii val[MAXS];

pii query(num v) {
	if(val[v] != pii(-1, -1)) {
		return val[v];
	}

	cout << strs[v] << endl;
	string l, r;
	cin >> l >> r;
	if(!idx.count(l)) {
		strs[S] = l;
		idx[l] = S;
		S++;
	}
	if(!idx.count(r)) {
		strs[S] = r;
		idx[r] = S;
		S++;
	}
	return val[v] = pii(idx[l], idx[r]);
}

const ll BASE = 1e17;

ll ans[7];

void mult2() {
	assert(ans[6] == 0);
	for(int i = 0; i < 7; i++) {
		ans[i] *= 2;
	}
	for(int i = 0; i < 6; i++) {
		if(ans[i] >= BASE) {
			ans[i] -= BASE;
			ans[i + 1] ++;
		}
		assert(ans[i] < BASE);
	}
	assert(ans[6] == 0);
}

void inc() {
	for(int i = 0; true; i++) {
		assert(i < 7);
		ans[i] ++;
		if(ans[i] == BASE) {
			ans[i] = 0;
		} else {
			break;
		}
	}
	assert(ans[6] == 0);
}

void print_ans() {
	assert(ans[6] == 0);
	cout << "Answer ";
	bool started = false;
	for(int i = 6; i >= 0; i--) {
		if(!started) {
			if(ans[i] == 0) continue;
			cout << ans[i];
			started = true;
		} else {
			cout << setfill('0') << setw(17) << ans[i];
		}
	}
	assert(started);
	cout << endl;
}

vector<char> vals;
void recurse(int a) {
	assert(a);
	if(query(a).A == 0) {
		inc();
		return;
	}
	recurse(query(a).A);
	int cur = query(a).B;
	for(__typeof(vals.rbegin()) it = vals.rbegin(); it != vals.rend(); it++) {
		assert(cur != 0);
		if(*it) cur = query(cur).B;
		else cur = query(cur).A;
	}
	mult2();
	if(cur != 0) {
		inc();
		vals.push_back(1);
	} else {
		vals.push_back(0);
	}
}

int main() {
	memset(val, -1, sizeof(val));

	strs[0] = "0";
	idx[strs[0]] = 0;

	strs[1] = "1";
	idx[strs[1]] = 1;

	S = 2;

	recurse(1);

	print_ans();
	return 0;
}
