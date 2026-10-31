#include "plants.h"
#include <bits/stdc++.h>
using namespace std;
const int n_bits=18;
const int inf = 1e9;
const int n_max = 2e5+5;
int arr[1<<(n_bits+1)];
int lazyadd[1<<(n_bits+1)];
int p_lt[n_max];
int p_rg[n_max];
int stash[n_max]; // a manual queue
int tallest[n_max];
int shortest[n_max];

struct segtree {
    int node = 1;
    int lazy = 0;
    segtree(){}
    void build(vector<int> &v) {
        assert(v.size()<(1<<n_bits));
        for(int i=0; i<(1<<n_bits); i++) {
            arr[i+(1<<n_bits)] = (i<(int)v.size() ? v[i] : inf);
        }
        for(int i=(1<<n_bits)-1; i>=1; i--) {
            arr[i] = min(arr[2*i], arr[2*i+1]);
        }
        for(int i=0; i<(1<<(n_bits+1)); i++) {
            lazyadd[i] = 0;
        }
    }

    void prefix_update(int x, int change) {
        if(x==-1) return;
        x += (1<<n_bits);
        lazyadd[x] += change;
        for(int i=0; i<n_bits; i++) {
            if(x&1) {
                lazyadd[x-1] += change;
            }
            arr[x/2] = min(arr[x]+lazyadd[x], arr[x^1]+lazyadd[x^1]);
            x=x/2;
        }
    }

    void decr(int left, int right) {
        prefix_update(left-1, 1);
        prefix_update(right,-1);
    }

    int find_and_remove_zero() {
        node = 1;
        lazy = 0;
        for(int i=0; i<n_bits; i++) {
            lazy += lazyadd[node];
            node *= 2;
            if(lazy + lazyadd[node] + arr[node]) node++;
        }
        int ans = node - (1<<n_bits);
        lazyadd[node] = 1e9;
        for(int i=0; i<n_bits; i++) {
            arr[node/2] = min(arr[node]+lazyadd[node], arr[node^1]+lazyadd[node^1]);
            node = node/2;
        }
        return ans;
    }
};

void lexi_smallest(int k, vector<int> &r, int* ptr, int* ord) {
    segtree s;
    s.build(r);
    int n = r.size();
    stash[0] = n;
    ord[n] = -1;
    int* fr = stash+1;
    int* bk = stash+1;
    for(int i=0; i<(int)r.size(); i++) {
        while(lazyadd[1] + arr[1]) {
            s.decr((*fr++)-k+1+n, n-1);
        }
        int p = s.find_and_remove_zero();
        ord[p] = i;
        if(p<k-1) {
            s.prefix_update(p, -1);
            *bk++ = p;
        } else {
            s.decr(p-k+1, p);
        }
        ptr[p] = ord[*(fr-1)];
    }
}
void init(int k, std::vector<int> r) {
    lexi_smallest(k,r,p_lt, tallest);
    for(int &i: r) i = k-1-i;
    lexi_smallest(k,r,p_rg, shortest);
}

int compare_plants(int x, int y) {
    if(x>y) return -compare_plants(y,x);
    if(tallest[x]>tallest[y] || p_rg[y]>=shortest[x]) return -1;
    if(shortest[x]>shortest[y] || p_lt[y]>=tallest[x]) return 1;
	return 0;
}
