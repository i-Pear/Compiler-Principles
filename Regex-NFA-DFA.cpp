#include <bits/stdc++.h>

using namespace std;

class Edge{
public:
    char op;
    int to;

    Edge(char op,int to) : op(op),to(to){}
};

vector<list<Edge>> edges;
set<int> start_able;
set<int> end_able;
set<int> final_state;

char symbol[6]={'(','*','&','|',')','#'};
int pri[][6]={
        {0,0,0,0,1,1},
        {1,1,1,1,1,1},
        {0,0,1,1,1,1},
        {0,0,0,1,1,1},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0}
};
unordered_map<char,unordered_map<char,int>> cmp;

void init_cmp(){
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            cmp[symbol[i]][symbol[j]]=pri[i][j];
        }
    }
}

int addNode(){
    edges.emplace_back();
    return edges.size()-1;
}

void addEdge(int u,int v,char op){
    edges[u].push_back({op,v});
}

class Segment{
public:
    int start;
    int end;

    Segment(int start,int end) : start(start),end(end){}

    Segment connect(const Segment &b) const{
        addEdge(end,b.start,0);
        return {start,b.end};
    }

    Segment star() const{
        int l=addNode();
        int r=addNode();
        addEdge(l,start,0);
        addEdge(end,r,0);
        addEdge(end,start,0);
        addEdge(l,r,0);
        return {l,r};
    }

    Segment plus(const Segment &b) const{
        int l=addNode();
        int r=addNode();
        addEdge(l,start,0);
        addEdge(l,b.start,0);
        addEdge(end,r,0);
        addEdge(b.end,r,0);
        return {l,r};
    }
};

string addConnectSymbol(const string &s){
    string r;
    int len=s.length();
    r.push_back(s[0]);
    for(int i=1;i<len;i++){
        if(s[i]!='*' && s[i]!='|' && s[i]!=')' && r.back()!='|' && r.back()!='('){
            r.push_back('&');
        }
        r.push_back(s[i]);
    }
    return r;
}

void check_start_able(int n){
    int vis[edges.size()];
    memset(vis,0,sizeof(vis));
    start_able.clear();
    queue<int> q;
    q.push(n);
    start_able.insert(n);
    while(!q.empty()){
        int cnt=q.front();
        q.pop();
        for(auto &e:edges[cnt]){
            if(!vis[e.to]){
                start_able.insert(e.to);
                vis[e.to]=true;
                q.push(e.to);
            }
        }
    }
}

void check_final_state(int final){
    // reverse edges
    vector<Edge> rev[edges.size()];
    for(int i=0;i<edges.size();i++){
        for(auto &e:edges[i]){
            rev[e.to].emplace_back(e.op,i);
        }
    }

    int vis[edges.size()];
    memset(vis,0,sizeof(vis));
    final_state.clear();
    queue<int> q;
    q.push(final);
    final_state.insert(final);
    vis[final]=true;
    while(!q.empty()){
        int cnt=q.front();
        q.pop();
        for(auto &e:rev[cnt]){
            if(e.op)continue;
            if(!vis[e.to]){
                final_state.insert(e.to);
                vis[e.to]=true;
                q.push(e.to);
            }
        }
    }
}

void check_end_able(){
    // reverse edges
    vector<Edge> rev[edges.size()];
    for(int i=0;i<edges.size();i++){
        for(auto &e:edges[i]){
            rev[e.to].emplace_back(e.op,i);
        }
    }

    int vis[edges.size()];
    memset(vis,0,sizeof(vis));
    end_able.clear();
    queue<int> q;
    for(int n:final_state){
        q.push(n);
        end_able.insert(n);
        vis[n]=true;
    }
    while(!q.empty()){
        int cnt=q.front();
        q.pop();
        for(auto &e:rev[cnt]){
            if(!vis[e.to]){
                end_able.insert(e.to);
                vis[e.to]=true;
                q.push(e.to);
            }
        }
    }
}

vector<int> _findRelation(Segment s){
    bool vis[edges.size()];
    vector<int> res;
    memset(vis,0,sizeof(vis));
    queue<int> q;
    q.push(s.start);
    while(!q.empty()){
        int cnt=q.front();
        res.push_back(cnt);
        q.pop();
        for(auto &i:edges[cnt]){
            if(!vis[i.to]){
                q.push(i.to);
                vis[i.to]=true;
            }
        }
    }
    return res;
}

void _outputGraph(Segment s,const string &filename="1.dot"){
    ofstream ofs(filename,ios::trunc);
    ofs<<"digraph finite_state_machine {\n"
         "\trankdir = LR\n"
         "\tedge [fontname=\"Segoe UI Symbol\"];\n"
         "\tnode [fontname=\"Segoe UI Symbol\"];\n"
         "\tsize=\"800,1500\";\n"
         "\tfixedsize=true;\n"
         "\tautosize=false;\n"
         "\tnode [shape = doublecircle]; q"<<s.end<<";\n""\tnode [shape = circle];\n";
    auto nodes=_findRelation(s);
    for(int cnt:nodes){
        for(auto &i:edges[cnt]){
            ofs<<"\tq"<<cnt<<"->q"<<i.to<<" [label=\"";
            if(i.op){
                ofs<<i.op;
            }else{
                ofs<<"ε";
            }
            ofs<<"\"];\n";
        }
    }
    ofs<<"}";
    ofs.close();
    system("dot 1.dot -o 1.png -Tpng");
    system("cmd.exe /c start 1.png");
}

void outputGraph(Segment s,const string &filename="1.dot"){
    check_end_able();
    check_start_able(s.start);
    set<int> nodes;
    set_intersection(start_able.begin(),start_able.end(),end_able.begin(),end_able.end(),
                     insert_iterator<set<int>>(nodes,nodes.begin()));
    ofstream ofs(filename,ios::trunc);
    map<int,int> remap;
    int count=0;
    for(int x:nodes){
        remap[x]=count++;
    }
    ofs<<"digraph finite_state_machine {\n"
         "\trankdir = LR\n"
         "\tedge [fontname=\"Segoe UI Symbol\"];\n"
         "\tnode [fontname=\"Segoe UI Symbol\"];\n"
         "\tsize=\"800,1500\";\n"
         "\tfixedsize=true;\n"
         "\tautosize=false;\n"
         "\tnode [shape = doublecircle];";
    for(int end:final_state){
        if(nodes.find(end)!=nodes.end()){
            ofs<<" q"<<remap[end];
        }
    }
    ofs<<";\n""\tnode [shape = circle];\n";
    for(int cnt:nodes){
        for(auto &i:edges[cnt]){
            if(nodes.find(i.to)==nodes.end())continue;
            ofs<<"\tq"<<remap[cnt]<<"->q"<<remap[cnt]<<" [label=\"";
            if(i.op){
                ofs<<i.op;
            }else{
                ofs<<"ε";
            }
            ofs<<"\"];\n";
        }
    }
    ofs<<"start->q"<<s.start<<";\n";
    ofs<<"}";
    ofs.close();
    system("dot 1.dot -o 1.png -Tpng");
    system("cmd.exe /c start 1.png");
}

Segment regex2Segment(const string &regex){
    stack<char> sym;
    stack<Segment> obj;
    sym.push('#');
    for(char c:regex){
        if(c=='*' || c=='|' || c=='(' || c==')' || c=='#' || c=='&'){
            // symbol
            while(cmp[sym.top()][c]>0){
                // pop out symbols
                if(sym.top()=='*'){
                    sym.pop();
                    obj.top()=obj.top().star();
                }else if(sym.top()=='|'){
                    sym.pop();
                    Segment a=obj.top();
                    obj.pop();
                    Segment b=obj.top();
                    obj.pop();
                    obj.push(b.plus(a));
                }else if(sym.top()=='&'){
                    sym.pop();
                    Segment a=obj.top();
                    obj.pop();
                    Segment b=obj.top();
                    obj.pop();
                    obj.push(b.connect(a));
                }else if(sym.top()=='('){
                    sym.pop();
                }
            }
            if(c=='*' || c=='|' || c=='(' || c=='&'){
                sym.push(c);
            }
        }else{
            // object
            int l=addNode();
            int r=addNode();
            addEdge(l,r,c);
            obj.push({l,r});
        }
    }
    return obj.top();
}

void eraseEmpty(Segment s){
    check_final_state(s.end);
    int outEmptyDegree[edges.size()];
    memset(outEmptyDegree,0,sizeof(outEmptyDegree));
    for(int i=0;i<edges.size();i++){
        for(auto &j:edges[i]){
            if(j.op==0){
                outEmptyDegree[i]++;
            }
        }
    }
    queue<int> q;
    for(int i=0;i<edges.size();i++){
        if(outEmptyDegree[i]==0){
            q.push(i);
        }
    }
    // turp
    while(!q.empty()){
        // cnt has no empty outEdge
        int cnt=q.front();
        q.pop();
        // erase empty trans towards cnt
        for(int node=0;node<edges.size();node++){
            for(auto i=edges[node].begin();i!=edges[node].end();i++){
                if(i->op==0 && i->to==cnt){
                    // node->cnt is an empty edge
                    for(auto &e:edges[cnt]){
                        addEdge(node,e.to,e.op);
                    }
                    // erase it
                    auto b=i;
                    i++;
                    edges[node].erase(b);
                    i--;
                    outEmptyDegree[node]--;
                    if(outEmptyDegree[node]==0){
                        q.push(node);
                    }
                }
            }
        }
    }
}

int main(){
    init_cmp();
    string regex;
    cin>>regex;
    regex=addConnectSymbol(regex)+'#';
    Segment graph=regex2Segment(regex);
    _outputGraph(graph);
    eraseEmpty(graph);
    //outputGraph(graph);
}