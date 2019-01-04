#include<iostream>
#include<stack>

using namespace std;

const int MAXN = 2e5;
int N;
int A[MAXN];
int res[MAXN];

bool go() {
	int num = 0;
	stack<int> L, R;
	for(int i = 0; i < N; i++) {
		if(L.empty() || L.top() > A[i]) {
			L.push(A[i]);
			res[i] = 1;
		} else if(R.empty() || R.top() > A[i]) {
			R.push(A[i]);
			res[i] = 2;
		} else {
			return false;
		}

		while((!L.empty() && L.top() == num) || (!R.empty() && R.top() == num)) {
			if(!L.empty() && L.top() == num) L.pop();
			else R.pop();
			num++;
		}
	}
	return num == N;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> A[i];
		A[i] --;
	}
	if(go()) {
		cout << "TAK" << '\n';
		cout << res[0];
		for(int i = 1; i < N; i++) {
			cout << ' ' << res[i];
		}
		cout << '\n';
	} else {
		cout << "NIE" << '\n';
	}
	return 0;
}
