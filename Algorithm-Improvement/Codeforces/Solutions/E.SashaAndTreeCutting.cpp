#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;

#define ll long long
ll ans = 1e9;

int seen[(1 << 20) + 3][21];

int tc = 0;
ll m;

void dp(long long mask, const vector<ll>& edges, int ind, ll taken){
    // cout << "dp: " << mask << " " << taken << endl;   
    if (seen[mask][taken] == tc) return;
    if (__builtin_popcountll(mask) == m){
        ans = min(ans, taken);
        seen[mask][taken] = tc;
        return;
    }
    if (ind == edges.size()){
        return;
    }
    //either take an edge or do not
    // cout << mask << " " << taken << endl;
    dp(mask, edges, ind+1, taken);
    dp(mask | edges[ind], edges, ind+1, taken + 1);
}

vector<pair<int, int>> dfs(int parent, int node, const map<int, vector<int>> &graph, int goal) {
    if (node == goal)
        return {{min(parent, node), max(parent, node)}};

    for (int next : graph.at(node)) {
        if (next == parent)
            continue;

        auto path = dfs(node, next, graph, goal);
        if (!path.empty()){
            path.push_back({min(parent, node), max(parent, node)});
            return path;
        }
    }

    return {};
}


void solve(){
    ans = 1e9;
    ll n; cin >> n;
    map <int, vector<int>> a;
    int j, k;
    for (int i = 0; i < n-1; ++i){
        cin >> j >> k;
        a[j].push_back(k);
        a[k].push_back(j);
    }

    cin >> m;

    map <pair<int, int>, ll> occs;
    for (ll i{0}, u, v; i < m; ++i) {
        cin >> u >> v;
        for (const auto &edge : dfs(-1, u, a, v)) {
            if (edge.first == -1) continue;
            occs[edge] |= 1LL << i;
        }
    }

    set<ll> maskoccs;

    for(auto [edge, mask] : occs){
        maskoccs.insert(mask);
        // cout << edge.first << " " << edge.second << " " << mask << endl;
    }
    vector<ll> edgeset (maskoccs.begin(), maskoccs.end()); 

    dp(0, edgeset, 0, 0);
    cout << ans << endl;
}

int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while(t--) {
        tc = t + 1;
        solve();
    }
    return 0;
}