class UnionSet{
public:
    int*father;
    int size;

    UnionSet(int n) : size(n){
        father=new int[n];
        for(int i=0;i<n;i++){
            father[i]=i;
        }
    }

    int getF(int n){
        if(father[n]==n)return n;
        father[n]=getF(father[n]);
        return father[n];
    }

    void connect(int a,int b){
        a=getF(a);
        b=getF(b);
        if(a>b)swap(a,b);
        father[b]=a;
    }

    int operator[](int n) const{
        return father[n];
    }

    ~UnionSet(){
        delete[] father;
    }
};


auto findEmptyCircle(Segment s){
    int in[edges.size()];
    memset(in,0,sizeof(in));
    vector<int> nodes=findRelation(s);
    queue<int> q;
    for(int n:nodes){
        for(auto &i:edges[n]){
            if(i.op)continue;
            in[i.to]++;
        }
    }
    for(int n:nodes){
        if(in[n]==0){
            q.push(n);
        }
    }
    while(!q.empty()){
        int n=q.front();
        q.pop();
        for(auto &i:edges[n]){
            if(i.op)continue;
            in[i.to]--;
            if(in[i.to]==0){
                q.push(i.to);
            }
        }
    }
    // only nodes in circles has in!=0
    UnionSet us(edges.size());
    for(int n:nodes){
        if(!in[n]){
            for(auto &i:edges[n]){
                if(i.op)continue;
                if(!in[i.to]){
                    us.connect(n,i.to);
                }
            }
        }
    }
    map<int,set<int>> mp;
    for(int n:nodes){
        if(!in[n]){
            mp[us[n]].insert(n);
        }
    }
    return mp;
}

vector<int> findRelation(Segment s){
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