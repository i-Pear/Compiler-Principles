#include <bits/stdc++.h>

using namespace std;

class Edge{
public:
    char op;
    int to;

    Edge(char op,int to) : op(op),to(to){}
};

vector<vector<Edge>> edges;

char symbol[6]={'(','*','&','+',')','#'};
int pri[][6]={
        {1, 1, 1, 1, 0, 1},
        {-1,1, 1, 1, 1, 1},
        {-1,-1,1, 1, 1, 1},
        {-1,-1,-1,1, 1, 1},
        {0, -1,-1,-1,1, 1},
        {-1,-1,-1,-1,-1,0}
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
        if(s[i]!='*' && s[i]!='+' && s[i]!=')'){
            r.push_back('&');
        }
        r.push_back(s[i]);
    }
    return r;
}

int main(){
    init_cmp();
    string regex;
    cin>>regex;
    regex=addConnectSymbol(regex)+'#';

    stack<char> sym;
    stack<Segment> obj;
    sym.push('#');
    for(char c:regex){
        if(c=='*' || c=='+' || c=='(' || c==')' || c=='#' || c=='&'){
            // symbol
            while(cmp[sym.top()][c]>0){
                // pop out syms
                if(sym.top()=='*'){

                }else if(sym.top()=='+'){

                }else if(sym.top()=='&'){

                }else if(sym.top()==')'){

                }
            }
        }else{
            // object
            int l=addNode();
            int r=addNode();
            addEdge(l,r,c);
            obj.push({l,r});
        }
    }
}