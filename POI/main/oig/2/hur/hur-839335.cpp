#include<bits/stdc++.h>
using namespace std;

int main() {
	map<char, int> mp;
	int N; scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		char c; int v;
		scanf(" %c %d", &c, &v);
		mp[c] += v;
	}
	for (__typeof(mp.begin()) it = mp.begin(); it != mp.end(); it++) {
		printf("%c %d\n", it->first, it->second);
	}

	return 0;
}
