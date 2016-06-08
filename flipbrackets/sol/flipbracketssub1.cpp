#include <bits/stdc++.h>
#include "flipbrackets.h"

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef pair<int, ii> iii;
typedef vector<ii> vii;

typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

const int MAXN = 2e5 + 10;
const int MAXQ = 2e5 + 10;
const int LG = 20;

vector<pair<int, char> > adj[MAXN];
int par[MAXN];
char edge[MAXN];
int rmqtable[MAXN<<1][LG];
int level[MAXN<<1];
int euler[MAXN<<1];
int depth[MAXN];
int firstocc[MAXN];
int eulercnt = 0;
int subsize[MAXN];
bool visited[MAXN];

void dfs1(int uu)
{
	stack<ii> s;
	s.push(ii(uu, -1));
	while(!s.empty())
	{
		int u = s.top().fi; int v = s.top().se; s.pop();
		if(!visited[u])
		{
			euler[eulercnt] = u; eulercnt++;
			if(par[u] == -1) depth[u] = 0;
			else depth[u] = depth[par[u]] + 1;
			firstocc[u] = eulercnt - 1;
			subsize[u] = 1;
			for(int i = 0; i < adj[u].size(); i++)
			{
				s.push(ii(u, adj[u][i].fi));
				s.push(ii(adj[u][i].fi, u));
				//dfs1(adj[u][i].fi);
			}
		}
		else
		{
			subsize[u] += subsize[v];
			euler[eulercnt] = u; eulercnt++;
		}
		visited[u] = true;
	}
}

void filllevel()
{
	for(int i = 0; i < eulercnt; i++)
	{
		level[i] = depth[euler[i]];
	}
	level[eulercnt] = 1e9 + 7;
	for(int j = 0; j < LG; j++)
	{
		for(int i = 0; i < eulercnt; i++)
		{
			rmqtable[i][j] = eulercnt;
			if(i + (1<<j) - 1 < eulercnt)
			{
				if(j == 0)
				{
					rmqtable[i][j] = i;
				}
				else
				{
					if(level[rmqtable[i][j - 1]] < level[rmqtable[i + (1<<(j-1))][j - 1]])
					{
						rmqtable[i][j] = rmqtable[i][j-1];
					}
					else
					{
						rmqtable[i][j] = rmqtable[i + (1<<(j-1))][j - 1];
					}
				}
			}
		}
	}
}

int rmq(int l, int r)
{
	int k = 31 - __builtin_clz(r-l);
	//cout << l << ' ' << r << ' ' << rmqtable[l][k] << ' ' << rmqtable[r - (1<<k) + 1][k] << endl;
	if(level[rmqtable[l][k]] < level[rmqtable[r - (1<<k) + 1][k]])
	{
		return rmqtable[l][k];
	}
	else
	{
		return rmqtable[r - (1<<k) + 1][k];
	}
}

int lca(int u, int v)
{
	if(firstocc[u] > firstocc[v]) swap(u, v);
	//cout << firstocc[u] << ' ' << firstocc[v] << ' ' << rmq(firstocc[u], firstocc[v]) << ' ' << euler[rmq(firstocc[u], firstocc[v])] << endl;
	return euler[rmq(firstocc[u], firstocc[v])];
}

struct node
{
	int ans1, ans2, a, b;
	bool lazy;
};

void printnode(node a)
{
	cout << a.ans1 << ' ' << a.ans2 << ' ' << a.a << ' ' << a.b << endl;
}

node combine(node l, node r)
{
	node res;
	res.ans1 = l.ans1 + r.ans1 + min(l.a - l.ans1, r.b - r.ans1);
	res.ans2 = l.ans2 + r.ans2 + min(l.b - l.ans2, r.a - r.ans2);
	res.a = l.a + r.a;
	res.b = l.b + r.b;
	res.lazy = false;
	return res;
}

node flip(node x)
{
	node res;
	res.ans1 = x.ans2;
	res.ans2 = x.ans1;
	res.a = x.b;
	res.b = x.a;
	res.lazy = false;
	return res;
}

int chainno = 0;
int chainhead[MAXN];
int chainpos[MAXN]; //pos in chain
int chainidx[MAXN]; //idx of chain contained
int chainsiz[MAXN];
vector<node> chainst[MAXN]; //stores segment tree of the chain
vector<char> chainbase[MAXN]; //stores base array of the chain
int numberofchains;
vector<char> emp;
vector<node> emp2;
node empt;
node tmpl;
node tmpr;

void hld(int uu)
{
	stack<ii> s;
	s.push(ii(uu, chainno));
	while(!s.empty())
	{
		int u = s.top().fi; int v = s.top().se; s.pop();
		//cout << u << ' ' << v << endl;
		if(chainhead[v] == -1)
		{
			chainhead[v] = u;
			chainsiz[v] = 0;
		}
		chainidx[u] = v;
		chainpos[u] = chainsiz[v];
		chainsiz[v]++;
		
		int maxsiz = -1; int maxidx = -1;
		for(int i = 0; i < adj[u].size(); i++)
		{
			if(subsize[adj[u][i].fi] > maxsiz)
			{
				maxidx = i;
				maxsiz = subsize[adj[u][i].fi];
			}
		}
		for(int i = 0; i < adj[u].size(); i++)
		{
			if(i != maxidx)
			{
				chainno++;
				numberofchains++;
				s.push(ii(adj[u][i].fi, chainno));
			}
		}
		if(maxidx >= 0)
		{
			chainbase[v].pb(adj[u][maxidx].se);
			s.push(ii(adj[u][maxidx].fi, v));
		}
	}
}

//Seg Tree Functions
void assignseg()
{
	for(int i = 0; i < numberofchains; i++)
	{
		int x = chainbase[i].size();
		chainst[i].assign((x<<2)|1, empt);
	}
}

void build(int segno, int id, int l, int r) //Build the segno-th seg tree. r should be the size of this seg tree, which is chainbase[segno].size()
{
	if(l == r) return ;
	if(r - l < 2)
	{
		if(chainbase[segno][l] == '(')
		{
			chainst[segno][id].a++;
		}
		else
		{
			chainst[segno][id].b++;
		}
		return ;
	}
	int mid = (l+r)>>1;
	build(segno, (id<<1), l, mid);
	build(segno, (id<<1)|1, mid, r);
	chainst[segno][id] = combine(chainst[segno][(id<<1)], chainst[segno][(id<<1)|1]);
}

void push(int segno, int id, int l, int r)
{
	if(chainst[segno][id].lazy)
	{
		chainst[segno][id] = flip(chainst[segno][id]);
		if(r - l < 2) return ;
		chainst[segno][(id<<1)].lazy = true;
		chainst[segno][(id<<1)|1].lazy = true;
	}
}

void update(int segno, int id, int x, int y, int l, int r) //[l, r), [x, y)
{
	if(l == r) return ;
	if(x >= r || l >= y) return ;
	if(x <= l && r <= y)
	{
		chainst[segno][id].lazy = true;
		push(segno, id, l, r);
		return ;
	}
	push(segno, id, l, r);
	int mid = (l+r)>>1;
	update(segno, (id<<1), x, y, l, mid);
	update(segno, (id<<1)|1, x, y, mid, r);
	chainst[segno][id] = combine(chainst[segno][(id<<1)], chainst[segno][(id<<1)|1]);
}

node query(int segno, int id, int x, int y, int l, int r)
{
	if(l == r) return empt;
	if(x >= r || l >= y) return empt;
	if(x <= l && r <= y)
	{
		push(segno, id, l, r);
		return chainst[segno][id];
	}
	push(segno, id, l, r);
	int mid = (l+r)>>1;
	return combine(query(segno, (id<<1), x, y, l, mid), query(segno, (id<<1)|1, x, y, mid, r));
}
//End Seg Tree Functions

void buildtree() //Build the segment trees
{
	assignseg();
	for(int i = 0; i < numberofchains; i++)
	{
		//cout << i << ' ' << chainbase[i].size() << ' ' << chainst[i].size() << endl;
		build(i, 1, 0, chainbase[i].size());
	}
}

node query_up(int u, int v)
{
	if(u == v) return empt;
	int uchain, vchain = chainidx[v];
	node ans = empt;
	while(1)
	{
		uchain = chainidx[u];
		//cout << "Query_up : " << u << ' ' << v << ' ' << uchain << ' ' << vchain << endl;
		if(uchain == vchain)
		{
			if(u == v) break;
			ans = combine(ans, query(uchain, 1, chainpos[v], chainpos[u], 0, chainbase[uchain].size()));
			//cout << chainpos[v] << ' ' << chainpos[u] << endl;
			break;
		}
		ans = combine(ans, query(uchain, 1, 0, chainpos[u], 0, chainbase[uchain].size()));
		//cout << chainpos[uchain] << ' ' << chainbase[uchain].size() << endl;
		u = chainhead[uchain];
		if(u == v) break;
		if(edge[u] == '(')
		{
			ans = combine(ans, tmpl);
		}
		else
		{
			ans = combine(ans, tmpr);
		}
		u = par[u];
	}
	return ans;
}

node query_down(int u, int v) //combine in the opposite direction
{
	if(u == v) return empt;
	int uchain, vchain = chainidx[v];
	node ans = empt;
	while(1)
	{
		uchain = chainidx[u];
		//cout << "Query_down : " << u << ' ' << v << ' ' << uchain << ' ' << vchain << endl;
		if(uchain == vchain)
		{
			if(u == v) break;
			//cout << chainpos[v] << ' ' << chainpos[u] << ' ' << chainbase[uchain].size() << endl;
			ans = combine(query(uchain, 1, chainpos[v],chainpos[u], 0, chainbase[uchain].size()), ans);
			//cout << chainpos[v] << ' ' << chainpos[u] << endl;
			break;
		}
		ans = combine(query(uchain, 1, 0, chainpos[u], 0, chainbase[uchain].size()), ans);
		u = chainhead[uchain];
		if(u == v) break;
		if(edge[u] == '(')
		{
			ans = combine(tmpl, ans);
		}
		else
		{
			ans = combine(tmpr, ans);
		}
		u = par[u];
	}
	return ans;
}

int querycombined(int u, int v)
{
	int lc = lca(u, v);
	node a = query_up(u, lc);
	node b = query_down(v, lc);
	//printnode(a);
	//printnode(b);
	node tmp = combine(a, b);
	return tmp.ans1;
}

void update_up(int u, int v)
{
	if(u == v) return ;
	int uchain, vchain = chainidx[v];
	while(1)
	{
		uchain = chainidx[u];
		//cout << "Update_up : " << u << ' ' << v << ' ' << uchain << ' ' << vchain << endl;
		if(uchain == vchain)
		{
			if(u == v) break;
			update(uchain, 1, chainpos[v], chainpos[u], 0, chainbase[uchain].size());
			//cout << chainpos[v] << ' ' << chainpos[u] << endl;
			break;
		}
		update(uchain, 1, 0, chainpos[u], 0, chainbase[uchain].size());
		u = chainhead[uchain];
		if(u == v) break;
		if(edge[u] == '(')
		{
			edge[u] = ')';
		}
		else
		{
			edge[u] = '(';
		}
		u = par[u];
	}
}

void updatepath(int u, int v)
{
	int lc = lca(u, v);
	//cout << lc << endl;
	update_up(u, lc);
	update_up(v, lc);
}

void clearall()
{
	memset(par, -1, sizeof(par));
	for(int i = 0; i < MAXN; i++)
	{
		adj[i].clear();
		edge[i] = '%';
	}
	for(int i = 0; i < (MAXN<<1); i++)
	{
		memset(rmqtable[i], -1, sizeof(rmqtable[i]));
	}
	memset(level, 0, sizeof(level));
	memset(euler, 0, sizeof(euler));
	memset(depth, 0, sizeof(depth));
	memset(firstocc, 0, sizeof(firstocc));
	eulercnt = 0;
	memset(subsize, 0, sizeof(subsize));
	chainno = 0;
	//chainhead.clear();
	memset(chainpos, 0, sizeof(chainpos));
	memset(chainidx, 0, sizeof(chainidx));
	memset(visited, 0, sizeof(visited));
	//chainsiz.clear();
	//chainst.clear();
	//chainbase.clear();
	memset(chainhead, -1, sizeof(chainhead));
	memset(chainsiz, 0, sizeof(chainsiz));
	for(int i = 0; i < MAXN; i++)
	{
		chainst[i].clear();
		chainbase[i].clear();
	}
}

void initialize(int N, int P[], char C[])
{
	int n;
	n = N;
	clearall();
	memset(chainidx, -1, sizeof(chainidx));
	memset(chainhead, -1, sizeof(chainhead));
	empt.ans1 = 0; empt.ans2 = 0; empt.a = 0; empt.b = 0; empt.lazy = false;
	tmpl.ans1 = 0; tmpl.ans2 = 0; tmpl.a = 1; tmpl.b = 0; tmpl.lazy = false;
	tmpr.ans1 = 0; tmpr.ans2 = 0; tmpr.a = 0; tmpr.b = 1; empt.lazy = false;
	numberofchains = 1;
	par[0] = -1;
	for(int i = 1; i <= n - 1; i++)
	{
		char c;
		par[i] = P[i];
		c = C[i];
		edge[i] = c;
		adj[par[i]].pb(mp(i, c));
	}
	eulercnt = 0;
	dfs1(0);
	filllevel();
	hld(0);
	buildtree();
}

void flipbrackets(int u, int v)
{
	int lc = lca(u, v);
	while(u != lc)
	{
		if(edge[u] == '(')
		{
			edge[u] = ')';
		}
		else
		{
			edge[u] = '(';
		}
		u = par[u];
	}
	while(v != lc)
	{
		if(edge[v] == '(')
		{
			edge[v] = ')';
		}
		else
		{
			edge[v] = '(';
		}
		v = par[v];
	}
	//updatepath(u, v);
}

int querypath(int u, int v)
{
	int lc = lca(u, v);
	//cout << "LCA " << u << ' ' << v << " = " << lc << endl;
	node x = empt;
	while(u != lc)
	{
		if(edge[u] == '(')
		{
			x = combine(x, tmpl);
		}
		else
		{
			x = combine(x, tmpr);
		}
		u = par[u];
	}
	node y = empt;
	while(v != lc)
	{
		if(edge[v] == '(')
		{
			y = combine(tmpl, y);
		}
		else
		{
			y = combine(tmpr, y);
		}
		v = par[v];
	}
	//printnode(x);
	//printnode(y);
	x = combine(x, y);
	return x.ans1;
	//return querycombined(u, v);
}



