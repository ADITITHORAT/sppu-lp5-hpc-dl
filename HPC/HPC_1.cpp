#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace chrono;

class Graph
{
public:
  int V;
  vector<vector<int>> adj;

  Graph(int v)
  {
    V = v;
    adj.resize(V);
  }

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void bfs(int start)
  {
    vector<bool> visited(V, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty())
    {
      int node = q.front();
      q.pop();
      //   cout << node << " ";

      for (int neigh : adj[node])
      {
        if (!visited[neigh])
        {
          visited[neigh] = true;
          q.push(neigh);
        }
      }
    }
  }

  void parallel_bfs(int start)
  {
    vector<bool> visited(V, false);
    vector<int> frontier, next;

    frontier.push_back(start);
    visited[start] = true;

    while (!frontier.empty())
    {
      next.clear();

#pragma omp parallel
      {
        vector<int> local_next;

#pragma omp for nowait
        for (int i = 0; i < frontier.size(); i++)
        {
          int node = frontier[i];

          // #pragma omp critical
          //   cout << node << " ";

          for (int neigh : adj[node])
          {
            bool add = false;

#pragma omp critical
            {
              if (!visited[neigh])
              {
                visited[neigh] = true;
                add = true;
              }
            }

            if (add)
              local_next.push_back(neigh);
          }
        }

#pragma omp critical
        next.insert(next.end(), local_next.begin(), local_next.end());
      }

      frontier = next;
    }
  }

  void dfs(int start)
  {
    vector<bool> visited(V, false);
    stack<int> s;

    s.push(start);

    while (!s.empty())
    {
      int node = s.top();
      s.pop();

      if (!visited[node])
      {
        visited[node] = true;
        // cout << node << " ";

        for (int neigh : adj[node])
        {
          if (!visited[neigh])
            s.push(neigh);
        }
      }
    }
  }

  void parallel_dfs_util(int node, vector<int> &visited)
  {
    for (int neigh : adj[node])
    {
      bool spawn_task = false;

#pragma omp critical
      {
        if (!visited[neigh])
        {
          visited[neigh] = 1;
          spawn_task = true;
          // cout << neigh << " ";
        }
      }

      if (spawn_task)
      {
#pragma omp task
        parallel_dfs_util(neigh, visited);
      }
    }

#pragma omp taskwait
  }

  void parallel_dfs(int start)
  {
    vector<int> visited(V, 0);

    visited[start] = 1;
    // cout << start << " ";

#pragma omp parallel
    {
#pragma omp single
      {
        parallel_dfs_util(start, visited);
      }
    }
  }

  void generateRandomGraph(int edges)
  {
    srand(time(0));

    set<pair<int, int>> usedEdges;

    while (usedEdges.size() < edges)
    {
      int u = rand() % V;
      int v = rand() % V;

      if (u == v)
        continue;

      if (usedEdges.count({u, v}) ||
          usedEdges.count({v, u}))
        continue;

      addEdge(u, v);

      usedEdges.insert({u, v});
    }
  }
};

// ---------------- MAIN ----------------
int main()
{
  int vertices = 200000;
  int edges = 10000;
  if (edges > (vertices * (vertices - 1)) / 2)
  {
    cout << "Too many edges!\n";
    return 0;
  }
  Graph g(vertices);

  g.generateRandomGraph(edges);

  cout << "Graph Generated Successfully\n";

  // -------- DFS --------
  cout << "Sequential DFS: ";
  auto start = high_resolution_clock::now();
  g.dfs(0);
  auto end = high_resolution_clock::now();
  cout << "\nTime: "
       << duration_cast<microseconds>(end - start).count() << " us\n";

  cout << "\nParallel DFS: ";
  start = high_resolution_clock::now();
  g.parallel_dfs(0);
  end = high_resolution_clock::now();
  cout << "\nTime: "
       << duration_cast<microseconds>(end - start).count() << " us\n";

  // -------- BFS --------
  cout << "\nSequential BFS: ";
  start = high_resolution_clock::now();
  g.bfs(0);
  end = high_resolution_clock::now();
  cout << "\nTime: "
       << duration_cast<microseconds>(end - start).count() << " us\n";

  cout << "\nParallel BFS: ";
  start = high_resolution_clock::now();
  g.parallel_bfs(0);
  end = high_resolution_clock::now();
  cout << "\nTime: "
       << duration_cast<microseconds>(end - start).count() << " us\n";

  return 0;
}