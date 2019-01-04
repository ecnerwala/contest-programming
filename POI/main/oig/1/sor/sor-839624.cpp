#include<bits/stdc++.h>
using namespace std;

typedef pair<size_t, string> pis;

const int MAXN = 200;
int N;
pis S[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> S[i].second;
		S[i].first = S[i].second.size();
	}
	sort(S, S + N);
	for (int i = 0; i < N; i++) {
		cout << S[i].second << '\n';
	}

	return 0;
}
