#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<deque>
#include<set>
#include<map>
#include<list>
#include<string>
#include<cstring>
#include<cassert>
#include<cmath>
#include<ctime>

using namespace std;

#define ITER(i, v) for(__typeof(v.begin()) i = v.begin(); i != v.end(); i++) 
#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;

template <class T> T min(T a, T b, T c) { return min(a, min(b, c)); }
template <class T> T max(T a, T b, T c) { return max(a, max(b, c)); }

template <class T> void setmin(T &a, T b) { if(b < a) a = b; }
template <class T> void setmax(T &a, T b) { if(b > a) a = b; }

const int MAXN = 10000;

int N;
bool E[MAXN][MAXN];

const char BAD = -1;

char vis[MAXN];

inline void printVis() { 
	for(int i = 0; i < N; i++) 
	cout << char(
		vis[i] == BAD ? '-' : 
		(vis[i] + '0')); 
	cout << '\n'; 
}

inline void printGrid() {
	cerr << ' ';
	for(int i = 0; i < N; i++) cerr << char(vis[i] + '0');
	cerr << '\n';
	for(int i = 0; i < N; i++) {
		cerr << char(vis[i] + '0');
		for(int j = 0; j < N; j++) {
			if(i == j) cerr << char(vis[i] + '0');
			else cerr << E[i][j];
		}
		cerr << '\n';
	}
}

//returns id of error or -1
int dfs(int c, bool e) {
	if(vis[c] == e) return -1;
	else if(vis[c] == !e) return c;
	vis[c] = e;
	for(int i = 0; i < N; i++) {
		if(c == i) continue;
		if(E[c][i] == !e) {
            int v = dfs(i, !e);
			if(v != -1) {
                return v;
            }
		}
	}
	return -1;
}

char vis2[MAXN];

bool solve(int c) {
	memcpy(vis2, vis, sizeof(vis));
    int v = dfs(c, 0);
    if(v == -1) return true;
    else {
        bool b = !vis[v];

		memcpy(vis, vis2, sizeof(vis));

        v = dfs(v, b);

        return v == -1;
    }
}

int num[MAXN];

ll go() {
	int e = 0;
	for(int i = 0; i < N; i++) {
        for(int j = 0; j < i; j++) {
            e += E[i][j];
        }
    }
	if(e == 0 || e == N * (N - 1) / 2) return N;

    memset(vis, -1, sizeof(vis));

    for(int i = 0; i < N; i++) {
        if(vis[i] == -1) {
            if(! solve(i)) return 0;
        }
    }

	//printGrid();

    int ans = 1;
    int v = -1;

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
			//if(i != j && vis[j] == vis[i]) assert(E[i][j] == vis[j]); 
			if(vis[j] != vis[i] && 
                    E[i][j] != vis[j]) {
                num[i]++;
            }
        }
        if(!num[i]) {
			//cerr << i << '\n';
			ans ++; 
			v = i;
		}
    }
    
    if(ans == 2) {
    	for(int j = 0; j < N; j++) {
    		if(vis[j] != vis[v] && num[j] == 1) ans++;
    	}
    }

    return ans;
}

int main() {
	memset(E, false, sizeof(E));

	scanf("%d", &N);

	for(int i = 0; i < N; i++) {
		int k; scanf("%d", &k);
		for(int j = 0; j < k; j++) {
			int a; scanf("%d", &a); a--;
			E[i][a] = true;
		}
	}
	printf("%lld\n", go());
	return 0;
}
