#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

long long ans = 0;

long long MOD = 998244353;


long long dfs(int ind, vector<vector<int>>& graph, int parent){
    long long buildup = 1;
    for (int item: graph[ind]){
        if (item == parent) continue; 
        buildup *= dfs(item, graph, ind);
        buildup %= MOD;
    }
    ans += buildup;
    ans = ans % MOD;
    return (buildup + 1)%MOD;
}

void solve(){
    int n; std::cin>> n;
    vector<int> visited (n+1);
    std::vector<vector<int>> graph(n+1);
    for (int i = 0; i < n-1; ++i){
        int u, v; std::cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    dfs(1, graph, 0);
    cout << (ans + 1)%MOD << endl;
}

int main(){
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while(t--){
        ans = 0;
        solve();
    }
}