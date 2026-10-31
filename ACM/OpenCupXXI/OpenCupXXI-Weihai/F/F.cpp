#include <bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

using namespace std;
typedef long long ll;

struct cube
{
    pair <int, int> block[8]; // val, rot
};

ll enc (cube c)
{
    ll res = 0;
    for (int i = 0; i < 8; i++)
    {
        res = 24 * res + (c.block[i].first * 3 + c.block[i].second);
    }
    return res;
}

cube dec (ll x)
{
    cube res;
    for (int i = 7; i >= 0; i--)
    {
        int v = x % 24;
        res.block[i].second = v % 3;
        res.block[i].first = v / 3;
        x /= 24;
    }
    return res;
}

int swivloc[8] = {0,2,4,6,1,3,5,7};
int swivturn[8] = {1,-1,-1,1,-1,1,1,-1};
int turnloc[8] = {1,3,0,2,5,7,4,6};
int moveloc[8] = {0,1,2,3,5,7,4,6};

cube swivel (cube c)
{
    cube res;
    for (int i = 0; i < 8; i++)
    {
        res.block[i].first = c.block[swivloc[i]].first;
        res.block[i].second = (c.block[swivloc[i]].second + swivturn[i] + 3) % 3;
    }
    return res;
}

cube turn (cube c)
{
    cube res;
    for (int i = 0; i < 8; i++)
        res.block[i] = c.block[turnloc[i]];
    return res;
}

cube move (cube c)
{
    cube res;
    for (int i = 0; i < 8; i++)
        res.block[i] = c.block[moveloc[i]];
    return res;
}

hash_map <ll, int> dist;
vector <ll> bfsq;

void work (ll cenc)
{
    cube c = dec (cenc);
    /*cout << cenc << " " << dist[cenc] << "\n";
    for (int i = 0; i < 8; i++)
        cout << c.block[i].first << " " << c.block[i].second << " ";
    cout << "\n";*/

    cube p = c;
    for (int i = 0; i < 3; i++)
    {
        for (int k = 1; k <= 3; k += 2)
        {
            cube t = p;
            for (int j = 0; j < k; j++)
                t = move (t);
            for (int j = 0; j < (3 - i) % 3; j++)
                t = swivel (t);
            ll v = enc (t);
            if (dist.find(v) == dist.end())
            {
                dist[v] = dist[cenc] + 1;
                bfsq.push_back(v);
            }
        }
        p = swivel (p);
    }
}

void precomp()
{
    bfsq.clear();
    cube c;
    for (int i = 0; i < 8; i++)
    {
        c.block[i].first = i;
        c.block[i].second = 0;
    }

    ll vstart = enc (c);
    bfsq.push_back (vstart);
    dist[vstart] = 0;

    for (int i = 0; i < bfsq.size(); i++)
    {
        //if (i % 10000 == 0) cout << i << endl;
        work (bfsq[i]);
    }

    //cout << bfsq.size() << " " << dist[bfsq.back()] << "\n";
}

string color = "WBOYGR";
vector <int> rv[2];
int score[2][8];
int rot[2][8];

int sloc[24] = {2,3,0,1,2,0,0,1,1,3,3,2,6,4,4,5,5,7,7,6,4,5,6,7};
int srot[24] = {0,0,0,0,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,0,0,0,0};

int get_color (char c)
{
    for (int i = 0; i < 6; i++)
        if (c == color[i]) return i;
    return -1;
}

void gogo()
{
    for (int i = 0; i < 2; i++)
    {
        rv[i].clear();
        for (int j = 0; j < 8; j++)
        {
            score[i][j] = rot[i][j] = 0;
        }
    }

    string s;
    for (int i = 0; i < 7; i++)
    {
        getline (cin, s);
        int np = 0;
        for (int j = 0; j < s.length(); j++)
        {
            char c = s[j];
            if (c == ' ') continue;
            else if (c == '|') np++;
            else
            {
                int cc = get_color (c);
                if (cc == -1) continue;
                rv[np].push_back(cc);
            }
        }
    }

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 24; j++)
        {
            int v = sloc[j];
            int r = srot[j];
            score[i][v] += (1 << rv[i][j]);
            if (rv[i][j] % 3 == 0)
                rot[i][v] = r;
        }

    cube c;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (score[1][i] == score[0][j])
            {
                c.block[i].first = j;
                c.block[i].second = (rot[1][i] - rot[0][j] + 3) % 3;
            }
        }
    }
    while (c.block[0].first + c.block[0].second != 0)
    {
        /*for (int i = 0; i < 8; i++)
        cout << c.block[i].first << " " << c.block[i].second << " ";
        cout << "\n";*/
        if (rand() % 2)
            c = swivel (c);
        else
            c = turn (c);
        //cout << "gogo" << endl;
    }
    cout << dist[enc (c)] << "\n";
}

int main()
{
    ios_base::sync_with_stdio(0);

    precomp();
    //return 0;

    int T; cin >> T;
    string s;
    getline (cin, s);

    for (int t = 0; t < T; t++)
        gogo();
}
