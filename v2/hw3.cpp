#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

int dataN;
vector<int> dataLabel;
vector<vector<int>> dataCon;
map<int, vector<int>> dataLabelCount;

void readData(char *fileName){
    FILE *file = fopen(fileName, "r");

    fscanf(file, " %*c%*d%d", &dataN);  // t 1 <N>

    dataLabel.resize(dataN);
    dataCon.resize(dataN);

    for(int i = 0; i < dataN; i++){
        int id, label;
        fscanf(file, " %*c%d%d", &id, &label);  // v <id> <label>

        dataLabel[id] = label;
    }

    for(;;){
        int v1, v2;
        if(fscanf(file, " %*c%d%d%*d", &v1, &v2) == EOF) break;  // e <v1> <v2> <label>

        dataCon[v1].push_back(v2);
        dataCon[v2].push_back(v1);
    }

    fclose(file);

    for(int i = 0; i < dataN; i++){
        int deg = dataCon[i].size();
        dataLabelCount[dataLabel[i]].push_back(deg);
    }

    for(auto &p : dataLabelCount){
        sort(p.second.begin(), p.second.end());
    }
}

struct V {
    int id, cand;

    bool operator<(const V &r) const {
        return cand != r.cand ? cand > r.cand : id > r.id;  // Reverse for priority queue
    }
};

void doQuery(char *fileName, int numQuery){
    FILE *file = fopen(fileName, "r");

    while(numQuery--){
        int N, sumDegree;
        fscanf(file, " %*c%*d%d%d", &N, &sumDegree);  // t <id> <N> <sumDegree>

        vector<int> queryLabel(N);
        vector<vector<int>> queryCon(N);

        vector<int> cand(N);

        for(int i = 0; i < N; i++){
            int id, label, deg;
            fscanf(file, "%d%d%d", &id, &label, &deg);  // <id> <label> <deg> <vertex list>

            queryLabel[id] = label;

            while(deg--){
                int x; fscanf(file, "%d", &x);
                queryCon[id].push_back(x);
            }
        }

        for(int i = 0; i < N; i++){
            int label = queryLabel[i], deg = queryCon[i].size();

            // candCount : same label && degree >= deg
            vector<int> &labelCount = dataLabelCount[label];
            cand[i] = labelCount.end() - lower_bound(labelCount.begin(), labelCount.end(), deg);
        }

        V root = { -1, dataN + 1 };

        for(int i = 0; i < N; i++){
            V now = { i, cand[i] }; if(root < now) root = now;
        }

        priority_queue<V> pq;
        vector<bool> vs(N);  // Warning: vector<bool> is dangerous, pay attention

        pq.push(root); vs[root.id] = true;
        bool f = false;

        while(!pq.empty()){
            V now = pq.top(); pq.pop();

            if(f) putchar(' ');
            else f = true;

            printf("%d", now.id);

            for(int y : queryCon[now.id]){
                if(!vs[y]){ pq.push({ y, cand[y] }); vs[y] = true; }
            }
        }

        putchar('\n'); fflush(stdout);
    }

    fclose(file);
}

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <data> <query> <# of query>\n", argv[0]);
        return -1;
    }

    readData(argv[1]);
    doQuery(argv[2], atoi(argv[3]));

    return 0;
}