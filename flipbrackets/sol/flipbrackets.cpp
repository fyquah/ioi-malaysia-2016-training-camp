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

void dfs(int uu)
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
	int ans1, ans2, ans3, ans4, l, r;
	bool lazy;
};

node combine(node a, node b)
{
	node res;
	res.ans1 = a.ans1 + b.ans1 + min(a.l - a.ans1, b.r - b.ans1);
	res.ans2 = a.ans2 + b.ans2 + min(a.r - a.ans2, b.l - b.ans2);
	res.ans3 = a.ans3 + b.ans3 + min(b.l - b.ans3, a.r - a.ans3);
	res.ans4 = a.ans4 + b.ans4 + min(b.r - b.ans4, a.l - a.ans4);
	res.l = a.l + b.l;
	res.r = a.r + b.r;
	res.lazy = false;
	return res;
}

node empt, leftnode, rightnode;
vector<vector<node> > st;
vector<vector<char> > basearr;
int chainsize[MAXN];
int chainpos[MAXN];
int chainhead[MAXN];
int chainidx[MAXN];
int globalchaincnt = 0;
vector<char> emp;
vector<node> emp2;

void flip(int segid, int pos)
{
	swap(st[segid][pos].l, st[segid][pos].r);
	swap(st[segid][pos].ans1, st[segid][pos].ans2);
	swap(st[segid][pos].ans3, st[segid][pos].ans4);
	st[segid][pos].lazy ^= 1;
}

void shift(int segid, int pos, int l, int r)
{
	if(st[segid][pos].lazy)
	{
		flip(segid, pos);
		if(r - l < 2) return ;
		st[segid][pos*2].lazy ^= 1;
		st[segid][pos*2 + 1].lazy ^= 1;
	}
}

void hld(int u)
{
	globalchaincnt = 0;
	stack<ii> s;
	s.push(ii(u, 0));
	basearr.pb(emp);
	st.pb(emp2);
	while(!s.empty())
	{
		int v = s.top().fi; int ch = s.top().se; s.pop();
		if(chainhead[ch] == -1)
		{
			chainhead[ch] = v;
			chainsize[ch] = 0;
		}
		chainpos[v] = chainsize[ch];
		chainidx[v] = ch;
		chainsize[ch]++;
		int imax = -1; int maxval = -1;
		for(int i = 0; i < adj[v].size(); i++)
		{
			if(subsize[adj[v][i].fi] > maxval)
			{
				maxval = subsize[adj[v][i].fi];
				imax = i;
			}
		}
		for(int i = 0; i < adj[v].size(); i++)
		{
			if(i != imax)
			{
				globalchaincnt++;
				basearr.pb(emp); st.pb(emp2);
				s.push(ii(adj[v][i].fi, globalchaincnt));
			}
		}
		if(imax != -1)
		{
			basearr[ch].pb(adj[v][imax].se);
			s.push(ii(adj[v][imax].fi, ch));
		}
	}
}

void buildst(int segno, int id, int l, int r)
{
	if(l >= r) return ;
	if(r - l < 2)
	{
		if(basearr[segno][l] == '(')
		{
			st[segno][id] = leftnode;
		}
		else
		{
			st[segno][id] = rightnode;
		}
		return ;
	}
	int mid = (l + r)/2;
	buildst(segno, id*2, l, mid);
	buildst(segno, id*2+1, mid, r);
	st[segno][id] = combine(st[segno][id*2], st[segno][id*2+1]);
}

void updatest(int segno, int id, int x, int y, int l, int r)
{
	if(l >= r) return ;
	shift(segno, id, l, r);
	if(x >= r || l >= y) return ;
	if(x <= l && r <= y)
	{
		st[segno][id].lazy = true;
		shift(segno, id, l, r);
		return ;
	}
	int mid = (l + r)/2;
	updatest(segno, id*2, x, y, l, mid);
	updatest(segno, id*2 + 1, x, y, mid, r);
	st[segno][id] = combine(st[segno][id*2], st[segno][id*2 + 1]);
}

node queryst(int segno, int id, int x, int y, int l, int r)
{
	if(l >= r) return empt;
	shift(segno, id, l, r);
	if(x >= r || l >= y) return empt;
	if(x <= l && r <= y)
	{
		return st[segno][id];
	}
	int mid = (l + r)/2;
	return combine(queryst(segno, id*2, x, y, l, mid), queryst(segno, id*2 + 1, x, y, mid, r));
}

void updateup(int u, int v) //flip all edges from u to v, v is ancenstor of u
{
	int uchain = chainidx[u]; 
	int vchain = chainidx[v];
	while(1)
	{
		if(u == v) break;
		uchain = chainidx[u];
		//if(u != 0) cout << uchain<<' ' <<vchain<<' ' <<u << ' ' << v << endl;
		if(uchain == vchain)
		{
			updatest(uchain, 1, chainpos[v], chainpos[u], 0, chainsize[uchain] - 1); //check this line if trouble occurs
			return ;
		}
		updatest(uchain, 1, 0, chainpos[u], 0, chainsize[uchain] - 1);
		u = chainhead[uchain];
		//if(u!=0)cout << u << endl;
		if(u == v) break;
		if(edge[u] == '(') edge[u] = ')';
		else edge[u] = '(';
		u = par[u];
	}
}

node queryup(int u, int v) //query the path from u to v, v is ancestor of u
{
	int uchain = chainidx[u]; 
	int vchain = chainidx[v];
	node ans = empt;
	while(1)
	{
		if(u == v) break;
		uchain = chainidx[u];
		//cout << uchain << ' ' << vchain << ' ' << u << ' ' << v << endl;
		if(uchain == vchain)
		{
			ans = combine(queryst(uchain, 1, chainpos[v], chainpos[u], 0, chainsize[uchain] - 1), ans); //check this line if trouble occurs
			break;
		}
		ans = combine(queryst(uchain, 1, 0, chainpos[u], 0, chainsize[uchain] - 1), ans);
		u = chainhead[uchain];
		if(u == v) break;
		if(edge[u] == '(') 
		{
			ans = combine(leftnode, ans);
		}
		else
		{
			ans = combine(rightnode, ans);
		}
		u = par[u];
	}
	return ans;
}

void buildtree()
{
	for(int i = 0; i <= globalchaincnt; i++)
	{
		int elements = chainsize[i] - 1;
		elements *= 4; elements++;
		//cout << i << ' ' << elements << endl;
		st[i].assign(elements, empt);
		buildst(i, 1, 0, chainsize[i] - 1);
	}
}

void initialize(int N, int P[], char C[])
{
	empt.ans1 = empt.ans2 = empt.ans3 = empt.ans4 = empt.l = empt.r = 0; empt.lazy = false;
	leftnode.ans1 = leftnode.ans2 = leftnode.ans3 = leftnode.ans4 = leftnode.l = leftnode.r = 0; leftnode.lazy = false; leftnode.l = 1;
	rightnode.ans1 = rightnode.ans2 = rightnode.ans3 = rightnode.ans4 = rightnode.l = rightnode.r = 0; rightnode.lazy = false; rightnode.r = 1;
	int n;
	n = N;
	memset(chainhead, -1, sizeof(chainhead));
	for(int i = 1; i <= n - 1; i++)
	{
		char c;
		par[i] = P[i];
		c = C[i];
		edge[i] = c;
		adj[par[i]].pb(mp(i, c));
	}
	eulercnt = 0;
	dfs(0);
	filllevel();
	hld(0);
	//cout << globalchaincnt << endl;
	buildtree();
}


void printnode(node x)
{
	cout << "Node Info : " << x.ans1 << ' ' << x.ans2 << ' ' << x.ans3 << ' ' << x.ans4 << ' ' << x.l << ' ' << x.r << ' ' << x.lazy << endl;
}

void flipbrackets(int u, int v)
{
	int lc = lca(u, v);
	//cout << u << ' ' << v << ' ' << lc << endl;
	updateup(u, lc);
	updateup(v, lc);
}

int querypath(int u, int v)
{
	int lc = lca(u, v);
	//cout << u << ' ' << v << ' ' << lc << endl;
	node x = queryup(u, lc);
	node y = queryup(v, lc);
	//printnode(x);
	//printnode(y);
	int ans = x.ans3 + y.ans1 + min(x.l - x.ans3, y.r - y.ans1);
	return ans;
}

