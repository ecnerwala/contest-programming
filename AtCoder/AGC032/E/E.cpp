#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
ll M;
ll A[MAXN];

ll Z;

bool isGood() {
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	N *= 2;
	for (int i = 0; i < N; i++) cin >> A[i];
	sort(A, A + N);

	return 0;
}
