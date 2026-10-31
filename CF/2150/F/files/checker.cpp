/*
This is a sample checker in C++ for Codeforces Task Cycle Closing.
Feel free to modify it as desired. It is guaranteed to work on at least C++17 and above.

You will first need to run your solution and get the output and store it to a file.

To run this program, you compile it as 
g++ checker.cpp -o checker

and then run it with the command 
./checker <input file> <output file>
where you replace <input file>, <output file> with the name of your files
For example, ./checker in out 

The checker is ONLY GUARANTEED to work if you adhere to the input restrictions and the output formats.
Additionally, the checker also prints the exact input test where you get a wrong answer for your convenience.
*/

#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Invalid Template Arguments: " << argv[0] << " <input.txt> <output.txt>\n";
        return 0;
    }

    ifstream fin(argv[1]); 
    if (!fin) { cout << "Cannot open input file\n"; return 0; }
    ifstream fout(argv[2]); 
    if (!fout) { cout << "Cannot open output file\n"; return 0; }
    
    int t; 
    fin >> t;
    
    for (int id = 1; id <= t; id++){
        int n, m;
        vector <pair<int, int>> e;
        fin >> n >> m;

        vector<vector<int>> adj(n + 1, vector<int>(n + 1));

        for (int i = 1; i <= m; i++){
            int u, v; 
            fin >> u >> v;
            
            adj[u][v] = adj[v][u] = 1;
            e.push_back(make_pair(u, v));
        }

        auto print_input = [&](){
            cout << "The input for the test case is as follows:\n";
            cout << n << " " << m << "\n";
            for (int i = 0; i < e.size(); i++){
                cout << e[i].first << " " << e[i].second << "\n";
            }
        };  

        auto readInt = [&](int l, int r, string s){
            int x;
            if (fout >> x){
                if (l <= x && x <= r){
                    return x;
                } else {
                    cout << "Wrong answer on test case " << id << "\n";
                    cout << "Out of range, Expected integer input for variable " << s << " in range [" << l << ", " <<  r << "], found " << x << "\n";
                    cout << "\n";
                    print_input();
                    exit(0);
                    return 0;
                }
            } else {
                cout << "Wrong answer on test case " << id << "\n";
                cout << "Wrong output format, expected integer input for variable " << s << ", but not found " << "\n";
                cout << "\n";
                print_input();
                exit(0);
                return 0;
            }
        };  
    
        int op = readInt(0, 2, "op");

        for (int i = 1; i <= op; i++){
            int k = readInt(1, n, "k"); 
            int s = readInt(0, n * (n - 1) / 2, "s");

            vector <pair<int, int>> add;

            for (int j = 1; j <= s; j++){
                vector <int> path(k);
                for (int l = 0; l < k; l++){
                    path[l] = readInt(1, n, "v_i");
                }

                vector <bool> seen(n + 1, false);
                for (auto x : path){
                    if (seen[x]){
                        cout << "Wrong answer on test case " << id << "\n";
                        cout << "Operation " << i << ", path " << j << ", vertex " << x << " repeats\n";
                        cout << "\n";
                        print_input();
                        return 0;
                    }
                    seen[x] = true;
                }

                for (int l = 1; l < k; l++){
                    int u = path[l - 1];
                    int v = path[l];
                    if (adj[u][v] == 0){
                        cout << "Wrong answer on test case " << id << "\n";
                        cout << "Operation " << i << ", path " << j << ", edge (" << u << ", " << v << ") does not exist in the graph\n";
                        cout << "\n";
                        print_input();
                        return 0; 
                    }
                }

                int u = path[0];
                int v = path[k - 1];
                if (u == v){
                    cout << "Wrong answer on test case " << id << "\n";
                    cout << "Operation " << i << ", path " << j << ", tries to add a self loop on vertex " << u << "\n";
                    cout << "\n";
                    print_input();
                    return 0; 
                }

                add.push_back({u, v});
            }

            for (auto [u, v] : add){
                if (adj[u][v] == 1){
                    cout << "Wrong answer on test case " << id << "\n";
                    cout << "Operation " << i << " tries to add multiple edge (" << u << " " << v << ")\n";
                    cout << "\n";
                    print_input();
                    return 0; 
                }
                adj[u][v] = adj[v][u] = 1;
            }
        }

        for (int u = 1; u <= n; u++){
            for (int v = u + 1; v <= n; v++) if (adj[u][v] == 0){
                cout << "Wrong answer on test case " << id << "\n";
                cout << "Graph not complete, missing edge (" << u << ", " << v << ")\n";
                cout << "\n";
                print_input();
                return 0;
            }
        }
    }

    int extra;
    if (fout >> extra){
        cout << "Wrong Answer\n";
        cout << "Extra information present in output file\n";
        return 0;
    }

    cout << "AC! You are the best!\n";
}
