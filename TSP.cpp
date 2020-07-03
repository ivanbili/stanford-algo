#include<iostream>
#include<fstream>
#include <utility> 
#include <vector>
#include <limits>
#include <time.h>
#include <ppl.h>

using namespace std;
using namespace concurrency;

static const int bitmasks[] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,
                                0x10000,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,
                                0x10000000,0x20000000,0x40000000,0x80000000 };

inline uint_fast8_t countBits(uint_fast32_t i)
{
    uint_fast8_t num = 0;
    while (i != 0)
    {
        num += i & 1;
        i >>= 1;
    }
    return num;
}

inline bool testBit(uint_fast32_t bitmap, uint_fast8_t bit)
{
    return bitmap & bitmasks[bit];
}

float dist(pair<float, float> i, pair <float, float> j)
{
    float dx = (j.first - i.first);
    float dy = (j.second - i.second);
    return sqrt(dx * dx + dy * dy);
}

int main()
{
    clock_t start = clock();
    ifstream InFile;
    InFile.open("tsp.txt");
    vector<pair<float,float>>vertices;
    unsigned int curr_int;
    InFile >> curr_int;
    if (curr_int > 32)
    {
        cout << "Too many vertices: " << curr_int << "!\n";
        return 1;
    }
    cout << "Number of vertices: " << curr_int << "\n";
    uint_fast8_t num_vertices = curr_int;
    for (uint_fast8_t i = 0; i < num_vertices; i++)
    {
        float fl1;
        float fl2;
        InFile >> fl1;
        InFile >> fl2;
        vertices.push_back(make_pair(fl1, fl2));
    }
    uint_fast32_t sets = 1<<num_vertices;
    float* tspMat =  new float[num_vertices * sets];
    float** tspMat2d = new float* [num_vertices];
    float* distMat = new float[num_vertices * num_vertices];
    float** distMat2d = new float* [num_vertices];
    fill(tspMat, tspMat + (num_vertices * sets), numeric_limits<float>::max());
    for (uint_fast8_t i = 0; i < num_vertices; i++)
    {
        tspMat2d[i] = &tspMat[i * sets];
        distMat2d[i] = &distMat[i * num_vertices];
        for (uint_fast8_t j = 0; j != num_vertices; j++)
            distMat2d[i][j] = dist(vertices[i], vertices[j]);
    }
    tspMat2d[0][1] = 0;
    uint_fast8_t* numBits = new uint_fast8_t[sets];
    for (uint_fast32_t i = 0; i < sets; i++)
        numBits[i] = countBits(i);
    for (uint_fast8_t m = 2; m <= num_vertices; m++)
    {
        parallel_for(size_t(0), size_t(sets), [&](size_t S)
            {
                if (testBit(S, 0) && numBits[S] == m)
                {
                    for (uint_fast8_t j = 1; j < num_vertices; j++)
                    {
                        if (testBit(S, j))
                        {
                            uint_fast32_t mask = ~(1 << j);
                            uint_fast32_t removeJFromSIndex = S & mask;
                            float min_value = numeric_limits<float>::max();
                            for (int k = 0; k < num_vertices; k++)
                            {
                                if (!testBit(S, k) || k == j)
                                    continue;
                                float prevDist = tspMat2d[k][removeJFromSIndex];
                                float KJDist = distMat2d[k][j];
                                float curr_value = prevDist + KJDist;
                                if (curr_value < min_value)
                                    min_value = curr_value;
                            }
                            tspMat2d[j][S] = min_value;
                        }
                    }
                }
            });
    }
    float min_dist = numeric_limits<float>::max();
    for (uint_fast8_t j = 1; j < num_vertices; j++)
    {
        float curr_dist = tspMat2d[j][sets - 1] + distMat2d[0][j];
        if (curr_dist < min_dist)
            min_dist = curr_dist;
    }
    cout << "Min distance: " << min_dist << "\n"; //26442.7
    cout << "Time: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds.\n"; //18.3 seconds
    return 0; 
}
