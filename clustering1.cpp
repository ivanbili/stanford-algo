#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <stack>
#include <algorithm>
using namespace std;

vector<vector<pair<int, int>>> vertices;
vector<pair<pair<int, int>, int>> edges;
vector<int>leader;
vector<int>groupSize; // only accurate for leaders that exist

int findGroup(int v)
{
    return leader[v];
}

int unionVertices(int v1, int v2)
{
    int smallerGroup = findGroup(v1);
    int biggerGroup = findGroup(v2);
    if (groupSize[smallerGroup] > groupSize[biggerGroup])
    {
        smallerGroup = findGroup(v2);
        biggerGroup = findGroup(v1);
    }
    groupSize[biggerGroup] += groupSize[smallerGroup];

    stack<int> stack;
    stack.push(smallerGroup);
    while (!stack.empty())
    {
        int curr_vertex = stack.top();
        stack.pop();
        leader[curr_vertex] = biggerGroup;
        
        for (int i = 0; i < vertices[curr_vertex].size(); i++)
        {
            int adjacent_v = vertices[curr_vertex][i].first;
            if (findGroup(adjacent_v) == smallerGroup)
                stack.push(adjacent_v);
        }
    }

    return 0;
}

bool cmpEdgeDist(pair<pair<int, int>, int> a, pair<pair<int, int>, int> b)
{
    return (a.second < b.second);
}


int main()
{
    ifstream InFile;
    InFile.open("clustering1.txt");
    vector<int>numbers;
    int curr_int;
    InFile >> curr_int;
    const int num_vertices = curr_int;
    vertices.resize(num_vertices);
    leader.resize(num_vertices);
    groupSize.resize(num_vertices);
    
    string line;
    getline(InFile, line);
    while (getline(InFile, line))
    {
        curr_int = 0;
        int i = 0;
        int v1 = 0;
        int v2 = 0;
        int dist = 0;
        const int line_size = line.size();
        int tab_count = 0;
        while (i < line_size)
        {
            if (line[i] == ' ')
            {
                if (tab_count == 0)
                {
                    v1 = curr_int - 1;
                    tab_count++;
                }
                else
                {
                    v2 = curr_int - 1;
                    tab_count++;
                }
                curr_int = 0;
                i++;
            }
            curr_int = curr_int * 10 + (line[i] - '0');
            i++;
        }
        dist = curr_int;
        vertices[v1].push_back(make_pair(v2, dist));
        vertices[v2].push_back(make_pair(v1, dist));
        edges.push_back(make_pair(make_pair(v1, v2), dist));
    }

    sort(edges.begin(), edges.end(), cmpEdgeDist);

    for (int i = 0; i < num_vertices; i++)
    {
        leader[i] = i;
        groupSize[i] = 1;
    }

    const int targetClusters = 4;
    int currClusters = vertices.size();

    int nextEdge = 0;
    
    while (currClusters > targetClusters)
    {
        while (nextEdge < edges.size())
        {
            int v1 = edges[nextEdge].first.first;
            int v2 = edges[nextEdge].first.second;
            nextEdge++;
            if (findGroup(v1) != findGroup(v2))
            {
                unionVertices(v1, v2);
                currClusters--;
                nextEdge = 0;
                break;
            }
        }
    }
    int min_distance = -1;
    for (int i = nextEdge; i < edges.size(); i++)
    {
        int v1 = edges[i].first.first;
        int v2 = edges[i].first.second;
        if (findGroup(v1) != findGroup(v2))
        {
            min_distance = edges[i].second;
            break;
        }
            
    }
    std::cout << "Min distance between " << targetClusters << " clusters: " << min_distance << "\n";
    system("pause");
}
