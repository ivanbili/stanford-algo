#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stack>
#include <time.h>
#include <utility> 

using namespace std;

vector<vector<pair<int, int>>> vertices;
vector<int> shortest_distance;
vector<bool> processed;

void find_shortest_path(int start)
{
    const int num_vertices = vertices.size();
    shortest_distance.resize(num_vertices);
    processed.clear();
    processed.resize(num_vertices, false);
    processed[start] = true;
    shortest_distance[start] = 0;
    for (int iter = 2; iter <= num_vertices; iter++)
    {
        int min_step = INT_MAX;
        int next_vertex = INT_MAX;
        for (int i = 0; i < num_vertices; i++)
        {
            if (processed[i])
            {
                for (int j = 0; j < vertices[i].size(); j++)
                {
                    if (!processed[vertices[i][j].first])
                    {
                        int curr_step = shortest_distance[i] + vertices[i][j].second;
                        if (curr_step < min_step)
                        {
                            min_step = curr_step;
                            next_vertex = vertices[i][j].first;
                        }
                    }
                }
            }
        }
        processed[next_vertex] = true;
        shortest_distance[next_vertex] = min_step;

    }
}

int main()
{
    clock_t start = clock();
    string line;
    ifstream myfile("dijkstraData.txt");
    if (myfile.is_open())
    {
        int curr_vertex = 0;
        vector<pair<int,int>> temp_list;
        vertices.push_back(temp_list);
        pair<int, int> curr_pair;
        while (getline(myfile, line))
        {
            int i = 0;
            int current_int = 0;
            const int line_size = line.size();
            bool first = true;
            while (i < line_size)
            {
                if (line[i] == '\t')
                {
                    
                    if (first)
                    {
                        curr_vertex = current_int - 1;
                        if (vertices.size() < curr_vertex + 1)
                            vertices.resize((curr_vertex + 1) * 2);
                        first = false;
                    }
                    else
                    {
                        curr_pair.second = current_int;
                        vertices[curr_vertex].push_back(curr_pair);
                    }
                    current_int = 0;
                    i++;
                }
                else if (line[i] == ',')
                {
                    curr_pair.first = current_int - 1;
                    current_int = 0;
                    i++;
                }
                current_int = current_int * 10 + (line[i] - '0');
                i++;
            }
        }
        myfile.close();
        vertices.resize(curr_vertex + 1);
    }
    else
    {
        cout << "Unable to open file";
    }
    clock_t end = clock();
    cout << "File reading took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    start = clock();
    find_shortest_path(0);
    end = clock();
    cout << "Dijkstra's algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";

    cout << "\nDistances to 7,37,59,82,99,115,133,165,188,197:\n";
    cout << shortest_distance[6] << ",";
    cout << shortest_distance[36] << ",";
    cout << shortest_distance[58] << ",";
    cout << shortest_distance[81] << ",";
    cout << shortest_distance[98] << ",";
    cout << shortest_distance[114] << ",";
    cout << shortest_distance[132] << ",";
    cout << shortest_distance[164] << ",";
    cout << shortest_distance[187] << ",";
    cout << shortest_distance[196] << "\n";
    return 0;
}
