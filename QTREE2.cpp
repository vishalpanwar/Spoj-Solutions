#include<bits/stdc++.h>
typedef long long ll;
#define fast std::ios::sync_with_stdio(false);std::cin.tie(false)
#define endl "\n"
//#define abs(a) a >= 0 ? a : -a
#define ll long long int
#define mod 1000000007
#define Endl endl
using namespace std;
const int N = 200000 + 10;
const int MAX = 1e6 + 10;
const int LN = 15;
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
	st[node] = st[2 * node] + st[2 * node + 1];
}

int query_tree(int node, int l, int r, int i, int j){
	if (l > r || l > j || r < i){
		return 0;
	}
	if (l >= i && r <= j){
		return st[node];
	}
	int mid = (l + r) >> 1;
	return (query_tree(2 * node, l, mid, i, j) + query_tree(2 * node + 1, mid + 1, r, i, j));
}

int query_up(int u,int v){
	if (u == v)return 0;
	int uchain, vchain = chainInd[v], ans = 0;//uchain and vchain contains the chain number
	while (1){
		uchain = chainInd[u];
		if (uchain == vchain){
			if (u == v)break;
			ans += query_tree(1, 0, sz, posInBase[v] + 1, posInBase[u]);//please note that we are doing +1 because arr[v] contains the edge length
																		//between node 'v-1' and 'v'. So by doing arr[posInBase[v] + 1] we get the
																		//first edge between v and v + 1 and so forth and so on upto u.
			break;
		}
		ans += query_tree(1, 0, sz, posInBase[chainHead[uchain]], posInBase[u]);
		u = chainHead[uchain];
		u = pa[u][0];
	}
	return ans;
}

int LCA(int u, int v){
	if (depth[u] < depth[v])swap(u, v);
	int diff = depth[u] - depth[v];
	for (int i = 0; i < LN; i++){
		if ((diff >> i) & 1){
			u = pa[u][i];
		}
	}
	if (u == v)return u;
	//Now u and v are at the same level
	for (int i = LN - 1; i >= 0; i--){
		if (pa[u][i] != pa[v][i]){
			u = pa[u][i];
			v = pa[v][i];
		}
	}
	return pa[u][0];
}

int query(int u, int v){
	int lca = LCA(u, v);
	int ans = query_up(u, lca);
	ans += query_up(v, lca);
	//cout << "query:   " << u << "  " << v << "   lca : " << lca << endl;
	return ans;
}

void HLD(int node, int prev, int cost){
	//cout << "head :  " << chainNo << " " << node<<"   "<<chainHead[chainNo] << endl;
	if (chainHead[chainNo] == -1){
		chainHead[chainNo] = node;
	}
	chainInd[node] = chainNo;
	posInBase[node] = sz;
	arr[sz++] = cost;

	int sc = -1, ncost;
	for (int i = 0; i < adj[node].size(); i++){
		if (adj[node][i] != prev){
			if (sc == -1 || subsz[sc] < subsz[adj[node][i]]){
				sc = adj[node][i];
				ncost = costs[node][i];
			}
		}
	}
	if (sc != -1){
		HLD(sc, node, ncost);
	}
	for (int i = 0; i < adj[node].size(); i++){
		if (adj[node][i] != prev && adj[node][i] != sc){
			chainNo++;
			HLD(adj[node][i], node, costs[node][i]);
		}
	}
	return;
}

void dfs(int node, int prev, int level){
	pa[node][0] = prev;
	depth[node] = level;
	subsz[node] = 1;
	for (int i = 0; i < adj[node].size(); i++){
		if (adj[node][i] != prev){
			otherEnd[ind[node][i]] = adj[node][i];
			dfs(adj[node][i], node, level + 1);
			subsz[node] += subsz[adj[node][i]];
		}
	}
}

int getkth(int p, int q, int k){
	int lca = LCA(p, q), d;
	if (lca == p){
		d = depth[q] - depth[p] + 1;
		swap(p, q);//we want p to be at higher depth.....so swap p and q if p is at lower depth i.e. it is the lca
		k = d - k + 1;//decide 'k' accordingly i.e. k will now become total distance minus k as we have now change our p(which was originally q)
	}
	else if (q == lca);//do nothing if q is lca
	//case when neither p and q are lca
	else{
		d = depth[p] + depth[q] - 2 * depth[lca] + 1;
		/*
		d denotes the total dist between the nodes p and q. it will be = dist(p,lca) + dist(lca,q) - 1
		= (depth[p] - depth[lca] + 1) + (depth[q] - depth[lca] + 1) - 1
		= depth[p] + depth[q] - 2 * depth[lca] + 1
		*/
		if (k > depth[p] - depth[lca] + 1){//case when 'k' will be between lca and q i.e. dist b/w lca and 'p' is less than k
			k = d - k + 1;//change 'k' accordingly
			swap(p, q);//swap p and q  as we want to calculate the dist from 'p' only.
		}
	}
	//Now we have set starting node as 'p' and changed k accordingly such that the kth node between 'p'
	//and 'q' will always lie between 'p' and lca(p,q) at a dist 'k' from p
	//Also dist(p,lca) > k
	//cout << "p : " << p << "  q : " << q << "   k : " << k << "   lca : " << lca << endl;
	k--;//decrement k as k = 1 will indicate p itself.
	for (int i = LN - 1; i >= 0; i--){
		if ((1 << i) <= k){//if k is greater than or equal to 2^i than we can move up by that much nodes
			p = pa[p][i];//p will become 2^i th ancestor of p
			k -= (1 << i);//we will move 2^i nodes up and k will be decreased by that amount
		}
	}
	return p;
}

int main(){
	int t, a, b, c, x, y;
	scanf("%d", &t);
	while (t--){
		scanf("%d", &n);
		for (int i = 0; i < n; i++){
			ind[i].clear();
			adj[i].clear();
			costs[i].clear();
			chainHead[i] = -1;
			for (int j = 0; j < LN; j++)pa[i][j] = -1;
		}
		for (int i = 1; i < n; i++){
			scanf("%d %d %d", &a, &b, &c);
			--a; --b;
			adj[a].push_back(b);
			adj[b].push_back(a);
			costs[a].push_back(c);
			costs[b].push_back(c);
			ind[a].push_back(i - 1);
			ind[b].push_back(i - 1);
		}
		//cout << "chaiHead : "; for (int i = 0; i < n; i++)cout << chainHead[i] << "  "; cout << endl;
		chainNo = 0;
		dfs(0, -1, 0);
		HLD(0, -1, -1);
		//cout<<"arr : ";for (int i = 0; i <= sz; i++)cout << arr[i] << "  "; cout << endl;
		build(1, 0, sz);
		for (int j = 1; j < LN; j++){
			for (int i = 0; i < n; i++){
				if (pa[i][j - 1] != -1)pa[i][j] = pa[pa[i][j - 1]][j - 1];
			}
		}
		//cout << "sz :  " << sz << endl;
		//for (int i = 0; i <= chainNo; i++)cout << "chain : " << i << "  " << chainHead[i] << endl;
		char ch[20];
		while (1){
			scanf("%s", ch);
			if (ch[1] == 'O')break;
			if (ch[0] == 'D'){
				scanf("%d %d", &a, &b);
				printf("%d\n", query(a-1, b-1));
			}
			else if (ch[0] == 'K'){
				scanf("%d %d %d", &a, &b, &c);
				printf("%d\n", getkth(a - 1, b - 1, c) + 1);
			}
		}
	}
	return 0;
}
