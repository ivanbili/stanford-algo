#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

vector<vector<pair<int, int>>> vertices;
vector<bool> processed;

long long int find_MST_distance(int start)
{
    long long int sum = 0;
    const int num_vertices = vertices.size();
    processed.clear();
    processed.resize(num_vertices, false);
    processed[start] = true;
    for (int iter = 2; iter <= num_vertices; iter++)
    {
        int min_edge_dist = INT_MAX;
        int next_vertex = INT_MAX;
        for (int i = 0; i < num_vertices; i++)
        {
            if (processed[i])
            {
                for (int j = 0; j < vertices[i].size(); j++)
                {
                    if (!processed[vertices[i][j].first])
                    {
                        if (vertices[i][j].second < min_edge_dist)
                        {
                            min_edge_dist = vertices[i][j].second;
                            next_vertex = vertices[i][j].first;
                        }
                    }
                }
            }
        }
        processed[next_vertex] = true;
        sum += min_edge_dist;
    }
    return sum;
}

int main()
{
    clock_t start = clock();
    string line;
    ifstream myfile("prim.txt");
    if (myfile.is_open())
    {
        getline(myfile, line);
        int i = 0;
        int current_int = 0;
        for (int i = 0; line[i] != ' ' && i < line.size(); i++)
            current_int = current_int * 10 + (line[i] - '0');
        vertices.resize(current_int);
        while (getline(myfile, line))
        {
            current_int = 0;
            int first_vertex = -1;
            int second_vertex = -1;
            int weight = 1;
            const int line_size = line.size();
            bool first = true;
            i = 0;
            while (i < line_size)
            {
                if (line[i] == ' ')
                {
                    if (first_vertex == -1)
                        first_vertex = current_int - 1;
                    else
                        second_vertex = current_int - 1;
                    current_int = 0;
                    i++;
                }
                if (line[i] == '-')
                {
                    weight = -1;
                    i++;
                }
                current_int = current_int * 10 + (line[i] - '0');
                i++;
            }
            weight *= current_int;
            vertices[first_vertex].push_back(make_pair(second_vertex, weight));
            vertices[second_vertex].push_back(make_pair(first_vertex, weight));
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    clock_t end = clock();
    cout << "File reading took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    start = clock();
    long long int distance = find_MST_distance(0);
    end = clock();
    cout << "Prim's Minimum Spanning Tree algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    cout << "\nDistance is " << distance << "\n";
    return 0;
}
