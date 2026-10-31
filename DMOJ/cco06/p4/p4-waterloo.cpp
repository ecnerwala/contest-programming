#define DEBUG 0

#include <bits/stdc++.h>
using namespace std;

#if DEBUG
// basic debugging macros
int __i__,__j__;
#define printLine(l) for(__i__=0;__i__<l;__i__++){cout<<"-";}cout<<endl
#define printLine2(l,c) for(__i__=0;__i__<l;__i__++){cout<<c;}cout<<endl
#define printVar(n) cout<<#n<<": "<<n<<endl
#define printArr(a,l) cout<<#a<<": ";for(__i__=0;__i__<l;__i__++){cout<<a[__i__]<<" ";}cout<<endl
#define print2dArr(a,r,c) cout<<#a<<":\n";for(__i__=0;__i__<r;__i__++){for(__j__=0;__j__<c;__j__++){cout<<a[__i__][__j__]<<" ";}cout<<endl;}
#define print2dArr2(a,r,c,l) cout<<#a<<":\n";for(__i__=0;__i__<r;__i__++){for(__j__=0;__j__<c;__j__++){cout<<setw(l)<<setfill(' ')<<a[__i__][__j__]<<" ";}cout<<endl;}

// advanced debugging class
// debug 1,2,'A',"test";
class _Debug {
    public:
        template<typename T>
        _Debug& operator,(T val) {
            cout << val << endl;
            return *this;
        }
};
#define debug _Debug(),
#else
#define printLine(l)
#define printLine2(l,c)
#define printVar(n)
#define printArr(a,l)
#define print2dArr(a,r,c)
#define print2dArr2(a,r,c,l)
#define debug
#endif

// define
#define MAX_VAL 999999999
#define MAX_VAL_2 999999999999999999LL
#define EPS 1e-6
#define mp make_pair
#define pb push_back

// typedef
typedef unsigned int UI;
typedef long long int LLI;
typedef unsigned long long int ULLI;
typedef unsigned short int US;
typedef pair<int,int> pii;
typedef pair<LLI,LLI> plli;
typedef vector<int> vi;
typedef vector<LLI> vlli;
typedef vector<pii> vpii;
typedef vector<plli> vplli;

// ---------- END OF TEMPLATE ----------
#define MOD 432000

int d[1000];
int dist(int a,int b) {
    return (b-a+MOD) % MOD;
}
int v[1000];
int cc[1000],cc2[1000];
int main() {
    int i;
    int n,c;
    double f;
    scanf("%d",&n);
    for (i = 0; i < n; i++) scanf("%*s %lf",&f),d[i] = round(f*1200);
    scanf("%d",&c),c *= 100;
    sort(d,d+n);
    if (c > 2*MOD) {
        printf("not possible\n");
        return 0;
    }
    if (n == 1) {
        printf("%d\n",c/100);
        return 0;
    }
    else if (n == 2) {
        int ans = 1e9;
        if (dist(d[0],d[1]) >= c) ans = min(ans,dist(d[0],d[1])+c);
        if (dist(d[1],d[0]) >= c) ans = min(ans,dist(d[1],d[0])+c);
        if (dist(d[0],d[1])+MOD >= c) ans = min(ans,dist(d[0],d[1])+MOD+c);
        if (dist(d[1],d[0])+MOD >= c) ans = min(ans,dist(d[1],d[0])+MOD+c);
        if (ans > 2*MOD) printf("not possible\n");
        else printf("%d\n",(ans+99)/100);
        return 0;
    }

    int j;
    int ans = 1e9;
    for (i = 0; i < n; i++) {
        if (dist(d[i],d[(i+2) % n]) < c) {
            printf("not possible\n");
            return 0;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 1; j < n; j++) {
            if (dist(d[(i+j-1) % n],d[(i+j) % n]) < c) break;
        }
        if (j == n) ans = min(ans,dist(d[i],d[(i+n-1) % n])+c);
    }
    for (i = 0; i < n; i++) {
        if (dist(d[i],d[(i+1) % n]) >= c) break;
    }
    if (i == n) {
        for (i = 0; i < n; i++) {
            int u = i,s = 0;
            fill(v,v+n,0),v[u] = 1;
            for (j = 0; j < n-1; j++) {
                int p = u;
                while (v[u] || (dist(d[p],d[u]) < c)) u++,u %= n;
                v[u] = 1,s += dist(d[p],d[u]);
            }
            ans = min(ans,s+c);
        }
    }
    else {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist(d[(i+j) % n],d[(i+j+1) % n]) >= c) break;
            }
            cc[i] = j & 1,cc2[i] = (i+j) % n;
        }
        for (i = 0; i < n; i++) {
            int u = i,s = 0;
            fill(v,v+n,0),v[u] = 1;
            for (j = 0; j < n-1; j++) {
                int p = u;
                while (v[u] || (dist(d[p],d[u]) < c)) u++,u %= n;
                if (cc[u] && !v[(u+1) % n] && (cc2[(u+1) % n] == cc2[u]) && (cc2[u] != cc2[p])) u++,u %= n;
                v[u] = 1,s += dist(d[p],d[u]);
            }
            ans = min(ans,s+c);
        }
    }
    if (ans > 2*MOD) printf("not possible\n");
    else printf("%d\n",(ans+99)/100);

    return 0;
}
