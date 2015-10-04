#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4000;

const int MAXD = 2e5;
int M, N;
int D;
int cnt[MAXD];
int pos[MAXN];

bool place(int i, int del) {
	bool bad = false;
	for(int j = 0; j < i; j++) {
		if((cnt[abs(pos[i] - pos[j])] -= del) < 0) {
			bad = true;
		}
	}
	return !bad;
}

bool dfs(int i, int d) {
	if(i == N) return true;

	while(!cnt[d]) d--;
	pos[i] = d;
	if(place(i, 1) && dfs(i + 1, d)) return true;
	place(i, -1);

	pos[i] = D - d;
	if(place(i, 1) && dfs(i + 1, d)) return true;
	place(i, -1);

	return false;
}

void go() {
	pos[0] = 0;
	D = MAXD - 1;
	while(!cnt[D]) D--;
	cnt[D] --;
	pos[1] = D;
	assert(dfs(2, D));
	sort(pos, pos + N);
}

void test() {
	memset(cnt, 0, sizeof(cnt));
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			cnt[abs(pos[i] - pos[j])] ++;
		}
	}
	go();
}

int main2() {
	N = 1000;
	for(int i = 0; i < N; i++) pos[i] = i + i / 3;
	test();
	return 0;
}

int main() {
	cin >> M;
	N = 0;
	while(N * (N - 1) / 2 < M) N++;
	assert(N * (N - 1) / 2 == M);
	for(int i = 0; i < M; i++) {
		int d; cin >> d;
		cnt[d]++;
	}
	go();
	cout << N << '\n';
	for(int i = 0; i < N; i++) {
		cout << pos[i] << '\n';
	}
	return 0;
}
