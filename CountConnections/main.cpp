#include<bits/stdc++.h>

using namespace std;

void dfs(int r, int c, vector<vector<int>>& matrix, vector<vector<int>>& vis){
    int m = matrix.size();
    int n = matrix[0].size();
    vis[r][c]=1;
    for(int dr = -1; dr<=1; dr++){
        for(int dc=-1; dc<=1; dc++){
            int nr = r + dr;
            int nc = c + dc;
            if(nr>=0 && nr<m && nc>=0 && nc<n && matrix[nr][nc]==1 && vis[nr][nc]==0){
                dfs(nr,nc,matrix,grid);
            }
        }
    }
}

int countConnections(vector<vector<int>> matrix){
    int m = matrix.size();
    int n = matrix[0].size();
    vector<vector<int>> vis(m, vector<int>(n,0));
    int count = 0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j]==1 && vis[i][j]==0){
                dfs(i,j,matrix,vis);
                count+=1;
            }
        }
    }
    return count;
}