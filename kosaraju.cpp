#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stack>
#include <time.h>

using namespace std;

vector<list<int>>vertices;
vector<list<int>>vertices_ordered;
vector<list<int>>back_vertices;
vector<int>finishing_time;
vector<bool>visited;
vector<int>leader;
int cur_time;
int cur_start_point;

void DFS_recursive(vector<list<int>>& graph, int start_vertex, bool second_pass)
{
    visited[start_vertex] = true;
    leader[start_vertex] = cur_start_point;
    list<int>::const_iterator it;
    for (it = graph[start_vertex].begin(); it != graph[start_vertex].end(); it++)
    {
        if (!visited[*it])
        {
            DFS_recursive(graph, *it, second_pass);
        }
    }
    finishing_time[start_vertex] = cur_time;
    cur_time++;
}

void DFS(vector<list<int>>& graph, int start_vertex)
{
    stack<int> dfs_stack;
    stack<int> finishing_time_stack;
    dfs_stack.push(start_vertex);
    while (!dfs_stack.empty())
    {
        int curr_vertex = dfs_stack.top();
        if (!visited[curr_vertex])
            finishing_time_stack.push(curr_vertex);
        dfs_stack.pop();
        visited[curr_vertex] = true;
        leader[curr_vertex] = cur_start_point;
        list<int>::const_iterator it;
        for (it = graph[curr_vertex].begin(); it != graph[curr_vertex].end(); it++)
        {
            if (!visited[*it])
                dfs_stack.push(*it);
        }
    }
    while (!finishing_time_stack.empty())
    {
        int curr_vertex = finishing_time_stack.top();
        finishing_time[curr_vertex] = cur_time;
        finishing_time_stack.pop();
        cur_time++;
    }
}

void DFS_loop(vector<list<int>>& graph, vector<int> & visit_order)
{
    fill(visited.begin(), visited.end(), false);
    cur_time = 0;
    for (int i = 0; i < visit_order.size(); i++)
    {
        int index = visit_order[i];
        if (!visited[index])
        {
            cur_start_point = index;
            DFS(graph, index);
        }
    }
}

int main()
{
    clock_t start = clock();
    string line;
    ifstream myfile("SCC.txt");
    if (myfile.is_open())
    {
        int curr_vertex = 0;
        list<int> temp_list;
        vertices.push_back(temp_list);
        while (getline(myfile, line))
        {
            int i = 0;
            int current_int = 0;
            const int line_size = line.size();
            while (i < line_size)
            {
                if (line[i] == ' ')
                {
                    if (i == line_size - 1)
                        vertices[curr_vertex].push_back(current_int-1);
                    else
                    {
                        curr_vertex = current_int - 1;
                        if (vertices.size() < curr_vertex + 1)
                           vertices.resize((curr_vertex + 1)*2);
                    }
                    current_int = 0;
                    i++;
                }
                current_int = current_int * 10 + (line[i] - '0');
                i++;
            }
        }
        myfile.close();
        vertices.resize(curr_vertex+1);
    }
    else
    {
        cout << "Unable to open file";
    }
    clock_t end = clock();
    cout << "File reading took " << double(end-start)/CLOCKS_PER_SEC <<" seconds.\n";
    start = clock();
    const int num_vertices = vertices.size();
    back_vertices.resize(num_vertices);
    leader.resize(num_vertices);
    finishing_time.resize(num_vertices);
    visited.resize(num_vertices);
    for (int i = 0; i < num_vertices; i++)
        {
        list<int>::const_iterator it;
        for (it = vertices[i].begin(); it != vertices[i].end(); it++)
           back_vertices[*it].push_back(i);
        }

    vector<int>ordering(num_vertices);

    end = clock();
    cout << "Creating additional data structures took " << double(end-start)/CLOCKS_PER_SEC <<" seconds.\n";
    start = clock();
    for (int i = 0; i < num_vertices; i++)
        ordering[i] = i;
    DFS_loop(back_vertices, ordering);
    end = clock();
    cout << "First DFS loop took " << double(end-start)/CLOCKS_PER_SEC <<" seconds.\n";
    start = clock();

    vector<int>time_to_index(num_vertices);
    for (int i = 0; i < num_vertices; i++)
        time_to_index[finishing_time[i]] = i;
    for (int i = 0; i < num_vertices; i++)
        ordering[i] = time_to_index[num_vertices-i-1];
    DFS_loop(vertices, ordering);
    
    end = clock();
    cout << "Second DFS loop took " << double(end-start)/CLOCKS_PER_SEC <<" seconds.\n";
    start = clock();

    vector<int>tally(num_vertices, 0);
    for (int i = 0; i < num_vertices; i++)
        tally[leader[i]]++;
    make_heap(tally.begin(), tally.end());
    cout << "The biggest strongly connected components are:\n";
    for (int i = 0; i < 5; i++)
    {
        cout << tally.front() << ",";
        pop_heap(tally.begin(), tally.end()-i);
    }
    end = clock();
    cout << "\nTallying up and ordering took took " << double(end-start)/CLOCKS_PER_SEC <<" seconds.\n";
    return 0; 
}
