#include <iostream>
#include <queue>

using namespace std;
char canteen[2][1000000];
bool visited[2][1000000];
int q[2][1000000];


int row(int length, int jump){
    int moves[4][2] = {{0,1},
                       {0,-1},
                       {-1,jump},
                       {1,jump}};
    queue<pair<int,int> >myq;
    myq.push(make_pair(0,0));
    while(!myq.empty()){
        int pre_x = myq.front().first;
        int pre_y = myq.front().second;
        myq.pop();
        
        if(pre_x >= length || pre_y >=length){
            return q[pre_x][pre_y];
        }
        
        if(!visited[pre_x][pre_y]){
            
            visited[pre_x][pre_y] = true;
            for (int i = 0; i < 4; i++){
                int dx = (pre_x + moves[i][0]) % 2;
                int dy = pre_y + moves[i][1];
                if(dx < 0 || dy < 0 || !((canteen[dx][dy] != 'x' && !visited[dx][dy]))){
                    continue;
                } else {
                    int row = q[pre_x][pre_y];
                    q[dx][dy] = row + 1;
                    if (row != dy + 1){
                        myq.push(make_pair(dx,dy));
                    } else continue;
                }
            }
        }
    }
    return -1;
}

void BFS(int length, int jump){
    for (int i = 0; i < 2; i++){
        for(int j = 0; j < length; j++){
            q[i][j] = 0;
        }
    }
    cout << row(length,jump) << endl;
}

int main(int argc, const char * argv[]) {
    int length, jump;
    
    cin >> length >> jump;
    
    for (int i = 0; i < 2; i++){
        for(int j = 0; j < length; j++){
            cin >> canteen[i][j];
        }
    }
    BFS(length,jump);
    return 0;
}
