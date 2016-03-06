#include<bits/stdc++.h>
typedef long long ll;
#define fast std::ios::sync_with_stdio(false);std::cin.tie(false)
#define endl "\n"
//#define abs(a) a >= 0 ? a : -a
#define ll long long int
#define mod 1000000007
#define Endl endl
using namespace std;
ll powmod(ll a, int b) { ll res = 1; if (a >= mod)a %= mod; for (; b; b >>= 1){ if (b & 1)res = res*a; if (res >= mod)res %= mod; a = a*a; if (a >= mod)a %= mod; }return res; }
ll gcd(ll a, ll b){ if (a < b)std::swap(a, b); return b == 0 ? a : gcd(b, a % b); }
ll lcm(ll a, ll b){ return ((a*b) / gcd(a, b)); }
int l_bound(int *arr, int lo, int hi, int val){ while (lo <= hi){ int mid = (lo + hi) >> 1; if (arr[mid] >= val)hi = mid - 1; else lo = mid + 1; }return lo; }
int u_bound(int *arr, int lo, int hi, int val){ while (lo <= hi){ int mid = (lo + hi) >> 1; if (arr[mid] > val)hi = mid - 1; else lo = mid + 1; }return hi; }
int bsearch(int *arr, int n, int val){ int lo = 0, hi = n - 1; while (hi >= lo){ int mid = (hi + lo) >> 1; if (arr[mid] == val)return mid; if (arr[mid] > val)hi = mid - 1; else lo = mid + 1; }return -1; }
const int N = 10000 + 10;
const int MAX = 1e6 + 10;
const int LN = 14;
vector<int> adj[N], costs[N], ind[N];
int n, sz, arr[N], tree[N], depth[N], pa[N][20], otherEnd[N], subsz[N];
int chainNo, chainHead[N], chainInd[N], posInBase[N];
int st[4 * N];

void build(int node, int l, int r){
	if (l == r){
		st[node] = arr[l];
		return;
	}
	int mid = (l + r) >> 1;
	build(2 * node, l, mid);
	build(2 * node + 1, mid + 1, r);
	st[node] = (st[2 * node] > st[2 * node + 1]) ? st[2 * node] : st[2 * node + 1];
}

void update(int node, int l, int r, int i, int val){
	if (l > r || l > i || r < i)return;
	if (l == i && r == i){
		st[node] = val;
		return;
	}
	int mid = (l + r) >> 1;
	update(2 * node, l, mid, i, val);
	update(2 * node + 1, mid + 1, r, i, val);
	st[node] = (st[2 * node] > st[2 * node + 1]) ? st[2 * node] : st[2 * node + 1];
}

int query_tree(int node, int l, int r, int i, int j){
	if (l > r || l > j || r < i){
		return -1;
	}
	if (l >= i && r <= j){
		return st[node];
	}
	int mid = (l + r) >> 1;
	return max(query_tree(2 * node, l, mid, i, j), query_tree(2 * node + 1, mid + 1, r, i, j));
}

//v is ancestor of u
//query the chain in which 'u' is present till head of that chain, then move to next chain up
//we do that till u and v are in the same chain and then we query from u to v in the same chain :D
int query_up(int u, int v){
	if (u == v)return 0;
	int uchain, vchain = chainInd[v], ans = -1;
	while (1){
		uchain = chainInd[u];
		if (uchain == vchain){
			if (u == v)break;
			ans = max(ans,query_tree(1, 0, sz, posInBase[v] + 1, posInBase[u]));
			break;
		}
		ans = max(ans,query_tree(1, 0, sz, posInBase[chainHead[uchain]], posInBase[u]));
		u = chainHead[uchain];
		u = pa[u][0];//move to parent of u, we are moving a chain up
	}
	return ans;
}

int LCA(int p, int q){
	if (depth[q] > depth[p])swap(p, q);
	int diff = depth[p] - depth[q];
	for (int i = 0; i < LN; i++){
		if ((diff >> i) & 1)
			p = pa[p][i];
	}
	//Now p and  q are at same level
	if (p == q)return p;
	for (int i = LN - 1; i >= 0; i--){
		if (pa[p][i] != -1 && pa[p][i] != pa[q][i]){
			p = pa[p][i];
			q = pa[q][i];
		}
	}
	return pa[p][0];
}

int query(int p, int q){
	int lca = LCA(p, q);
	//path from p to q is divided into path from p to lca(p,q) and q to lca(p,q)
	int ans = query_up(p, lca);
	int temp = query_up(q, lca);
	if (temp > ans)ans = temp;
	return ans;
}

void change(int i, int val){
	int p = otherEnd[i];
	update(1, 0, sz, posInBase[p], val);
}

void HLD(int cur, int cost, int prev){
	if (chainHead[chainNo] == -1){
		chainHead[chainNo] = cur;
	}
	chainInd[cur] = chainNo;
	posInBase[cur] = sz;
	arr[sz++] = cost;

	int sc = -1, sc_cost;
	for (int i = 0; i < adj[cur].size(); i++){
		if (adj[cur][i] != prev){
			if (sc == -1 || subsz[sc] < subsz[adj[cur][i]]){
				sc = adj[cur][i];
				sc_cost = costs[cur][i];
			}
		}
	}
	if (sc != -1)HLD(sc, sc_cost, cur);

	for (int i = 0; i < adj[cur].size(); i++){
		if (adj[cur][i] != prev && sc != adj[cur][i]){
			//new chain at each normal node :)
			chainNo++;
			HLD(adj[cur][i], costs[cur][i], cur);
		}
	}
}

void dfs(int cur, int prev, int level = 0){
	pa[cur][0] = prev;
	depth[cur] = level;
	subsz[cur] = 1;
	for (int i = 0; i < adj[cur].size(); i++){
		if (adj[cur][i] != prev){
			otherEnd[ind[cur][i]] = adj[cur][i];
			dfs(adj[cur][i], cur, level + 1);
			subsz[cur] += subsz[adj[cur][i]];
		}
	}
}

int main(){
	int t;
	scanf("%d", &t);
	while (t--){
		//printf("\n");
		sz = 0;
		scanf("%d", &n);
		for (int i = 0; i < n; i++){
			adj[i].clear();
			costs[i].clear();
			ind[i].clear();
			chainHead[i] = -1;
			for (int j = 0; j < LN; j++)pa[i][j] = -1;
		}
		for (int i = 1; i < n; i++){
			int a, b, c;
			scanf("%d %d %d", &a, &b, &c);
			--a;--b;
			adj[a].push_back(b);
			adj[b].push_back(a);
			ind[a].push_back(i - 1);
			ind[b].push_back(i - 1);
			costs[a].push_back(c);
			costs[b].push_back(c);
		}
		chainNo = 0;
		dfs(0, -1);//setup subtree size, depth and parent for each node;
		//cout << "dfs over !!!" << endl;
		HLD(0, -1, -1);//decompose the tree and create the baseArray
		//cout << "HDL over" << endl;
		build(1, 0, sz);
		//cout << "Build over" << endl;
		//for (int i = 0; i < n; i++)cout << "edge : " << i << " " << otherEnd[i]<<"   "<<posInBase[i] << endl;

		//bottom up DP code for LCA:
		for (int j = 1; j < LN; j++){
			for (int i = 0; i < n; i++){
				if (pa[i][j - 1] != -1)pa[i][j] = pa[pa[i][j - 1]][j - 1];
			}
		}
		char ch[20];
		while (1){
			scanf("%s", ch);
			if (ch[0] == 'D')break;
			int a, b;
			scanf("%d %d", &a, &b);
			//cout << ch << " " << a << " " << b << Endl;
			if (ch[0] == 'Q')
				printf("%d\n", query(a - 1, b - 1));
			else
				change(a - 1, b);
		}
	}
	return 0;
}
