#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <utility>
#include <time.h>
using namespace std;

int getAndUpdateDominantVertex(vector<int>& dominant_vertex, int orig_ind)
{
    int dom_ind = orig_ind;
    while (dominant_vertex[dom_ind] != dom_ind)
        dom_ind = dominant_vertex[dom_ind];
    int i = orig_ind;
    while (dominant_vertex[i] != dom_ind)
    {
        int temp = dominant_vertex[i];
        dominant_vertex[i] = dom_ind;
        i = temp;
    }
    return dom_ind;
}

void contractEdge(vector<list<int>>& vertices, vector<pair<int, int>>& edges, vector<int>& dominant_vertex, const int edge_ind)
{
    int v1 = getAndUpdateDominantVertex(dominant_vertex, edges[edge_ind].first);
    int v2 = getAndUpdateDominantVertex(dominant_vertex, edges[edge_ind].second);

    // make v1 the dominant vertix over v2
    dominant_vertex[v2] = v1;
    
    // merge the connected vertices to the dominant vertix list
    vertices[v1].splice(vertices[v1].begin(), vertices[v2]);

    // make the merged list point to the new dominant vertex number
    for (std::list<int>::iterator it = vertices[v1].begin(); it != vertices[v1].end(); it++)
    {
        while (dominant_vertex[*it] != *it)
           *it = dominant_vertex[*it];
    }
    // remove self edges in the vertex list
    vertices[v1].remove(v1);

    // swap and delete the edge from the list
    swap(edges[edge_ind], edges[edges.size()-1]);
    edges.pop_back();

    // remove self edges from the edge list (again swap to end and delete from end for efficiency)
    int i = 0;
    while (i < edges.size())
    {
        int v1 = getAndUpdateDominantVertex(dominant_vertex, edges[i].first);
        int v2 = getAndUpdateDominantVertex(dominant_vertex, edges[i].second);
        if (v1 == v2)
        {
            swap(edges[i], edges[edges.size() - 1]);
            edges.pop_back();
        }
        else
        {
            i++;
        }
    }
}

int main()
{
    vector<list<int>>vertices;
    vector<pair<int, int>> edges;
    vector<int>dominant_vertex;
    string line;
    ifstream myfile("kargerMinCut.txt");
    if (myfile.is_open())
    {
        int current_line = 0;
        while (getline(myfile, line))
        {
            current_line++;
            dominant_vertex.push_back(current_line-1);
            int i = 0;
            while (line[i] != '\t')
                i++;
            i++;
            list<int>nums;

            int current_int = 0;
            while (i < line.size())
            {
                if (line[i] == '\t')
                {
                    nums.push_back(current_int-1);
                    if ((current_int - 1) > (current_line - 1))
                       edges.push_back(pair<int, int>(current_line-1, current_int - 1));
                    current_int = 0;
                    i++;
                }
                current_int = current_int * 10 + (line[i] - '0');
                i++;
            }
            vertices.push_back(nums);
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file";
    }

    srand(time(NULL));
    int min_cut = edges.size();
    for (int i = 0; i < 10000; i++)
    {
       vector<list<int>>vertices_copy(vertices);
       vector<pair<int, int>> edges_copy(edges);
       vector<int>dominant_vertex_copy(dominant_vertex);
       int contractions = vertices_copy.size() - 2;
       for (int i = 0; i < contractions; i++)
          contractEdge(vertices_copy, edges_copy, dominant_vertex_copy, rand() % edges_copy.size());
       int curr_cut = edges_copy.size();
       if (curr_cut < min_cut)
       {
          min_cut = curr_cut;
          cout << "The minimum cut we found so far is " << min_cut << " after "<< i + 1 << " tries. \n";
       }
    }
    cout << "The minimum cut we found was: "<< min_cut<< "\n";
}

