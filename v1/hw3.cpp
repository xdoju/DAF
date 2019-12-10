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

void doQuery(char *fileName, int numQuery){
  FILE *file = fopen(fileName, "r");

  while(numQuery--){
    int N, sumDegree;
    fscanf(file, " %*c%*d%d%d", &N, &sumDegree);  // t <id> <N> <sumDegree>

    vector<int> queryLabel(N);
    vector<vector<int>> queryCon(N);

    for(int i = 0; i < N; i++){
      int id, label, deg;
      fscanf(file, "%d%d%d", &id, &label, &deg);  // <id> <label> <deg> <vertex list>

      queryLabel[id] = label;

      while(deg--){
        int x; fscanf(file, "%d", &x);
        queryCon[id].push_back(x);
      }
    }

    // Select root
    int root = -1, rootDia = N, rootCand = dataN + 1;

    for(int i = 0; i < N; i++){
      // Find root candidates
      int label = queryLabel[i], deg = queryCon[i].size();

      // candCount : same label && degree >= deg
      vector<int> &labelCount = dataLabelCount[label];
      int candCount = labelCount.end() - lower_bound(labelCount.begin(), labelCount.end(), deg);

      if(candCount == 0){ root = i; break; }

      // Do BFS to find graph center
      // Total complexity: O(NM)

      int dia = 0;

      queue<int> q;
      vector<bool> vs(N);  // Warning: vector<bool> is dangerous, pay attention

      q.push(i); vs[i] = true;

      for(;;){
        int cnt = q.size();

        while(cnt--){
          int x = q.front(); q.pop();

          for(int y : queryCon[x]){
            if(!vs[y]){ q.push(y); vs[y] = true; }
          }
        }

        if(q.empty()) break;
        dia++;
      }

      if(dia < rootDia || (dia == rootDia && candCount < rootCand)){
        root = i; rootDia = dia; rootCand = candCount;
      }
    }

    queue<int> q;
    vector<bool> vs(N);  // Warning: vector<bool> is dangerous, pay attention

    q.push(root); vs[root] = true; printf("%d", root);

    while(!q.empty()){
      int x = q.front(); q.pop();

      for(int y : queryCon[x]){
        if(!vs[y]){ q.push(y); vs[y] = true; printf(" %d", y); }
      }
    }

    printf("\n"); fflush(stdout);
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
