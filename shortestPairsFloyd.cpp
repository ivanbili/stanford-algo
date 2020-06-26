#include<iostream>
#include<fstream>
#include <time.h>
#include <ppl.h>
using namespace std;
using namespace concurrency;

int main()
{
    clock_t start = clock();
    ifstream InFile;
    InFile.open("g3.txt"); //g1.txt, g2.txt, g3.txt, large.txt
    int vertix_num;
    int edge_num;
    InFile >> vertix_num;
    InFile >> edge_num;
    int * floydMat = NULL;
    int ** floydMat2d = NULL;
    floydMat = new int[vertix_num * vertix_num];
    floydMat2d = new int* [vertix_num];
    fill(floydMat, floydMat + vertix_num * vertix_num, INT_MAX);
    for (size_t i = 0; i != vertix_num; i++)
    {
        floydMat2d[i] = &floydMat[i * vertix_num];
        floydMat2d[i][i] = 0;
    }
    for (int i = 0; i < edge_num; i++)
    {
        int src_vertix;
        int dest_vertix;
        int weight;
        InFile >> src_vertix;
        InFile >> dest_vertix;
        InFile >> weight;
        floydMat2d[src_vertix - 1][dest_vertix - 1] = min(weight, floydMat2d[src_vertix - 1][dest_vertix - 1]);
    }
    InFile.close();
    for (int k = 0; k < vertix_num; k++)
    {
        parallel_for(size_t(0), size_t(vertix_num), [&](size_t i)
            {
                for (int j = 0; j < vertix_num; j++)
                {
                    int case1 = floydMat2d[i][j];
                    int case2a = floydMat2d[i][k];
                    int case2b = floydMat2d[k][j];
                    if (case2a == INT_MAX || case2b == INT_MAX)
                        floydMat2d[i][j] = case1;
                    else
                        floydMat2d[i][j] = min(case1, case2a + case2b);
                }
            });
    }
    int min_dist = INT_MAX;
    bool negative_cycle = false;
    for (int i = 0; i < vertix_num; i++)
    {
        for (int j = 0; j < vertix_num; j++)
        {
            if (i == j)
            {
                if (floydMat2d[i][j] < 0)
                {
                    negative_cycle = true;
                    break;
                }
                continue;
            }
            min_dist = min(min_dist, floydMat2d[i][j]);
        }
    }
    delete[] floydMat, floydMat2d;
    if (negative_cycle)
        cout << "Floyd-Warshall: The graph has a negative cycle!\n";
    else
        cout << "Floyd-Warshall: Minimum distance is: " << min_dist << "\n";
    cout << "Floyd time: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds.\n";
    return 0;
}
