#include<iostream>
#include<fstream>
#include <unordered_set>

using namespace std;

void computeMaxWeightIndSet(vector<unsigned int> & vertices, vector<unsigned int> & max_weights)
{
    int weigths_size = max_weights.size();
    if (vertices.size() == weigths_size)
        return;
    if (max_weights[weigths_size - 2] + vertices[weigths_size] > max_weights[weigths_size - 1])
        max_weights.push_back(max_weights[weigths_size - 2] + vertices[weigths_size]);
    else
        max_weights.push_back(max_weights[weigths_size - 1]);
    computeMaxWeightIndSet(vertices, max_weights);
}

int main()
{
    ifstream InFile;
    InFile.open("mwis.txt");
    vector<unsigned int>vertices;
    vector<unsigned int>max_weights;
    vector<bool>included;
    int curr_int;
    InFile >> curr_int;
    const int num_vertices = curr_int;
    for (int i = 0; i < num_vertices; i++)
    {
        InFile >> curr_int;
        vertices.push_back(curr_int);
        included.push_back(false);
    }
    InFile.close();

    max_weights.push_back(vertices[0]);
    if (vertices[0] > vertices[1])
        max_weights.push_back(vertices[0]);
    else
        max_weights.push_back(vertices[1]);

    computeMaxWeightIndSet(vertices, max_weights);

    int i = num_vertices-1;
    while (i > 1)
    {
        if (max_weights[i-1] >= max_weights[i-2] + vertices[i])
        {
            i--;
        }
        else
        {
            included[i] = true;
            i -= 2;
        }
    }
    if (i == 0)
        included[0] = true;

    if (i == 1)
    {
        if (vertices[0] > vertices[1])
            included[0] = true;
        else
            included[1] = true;
    }
        
    std::cout << "vertices 1,2,3,4,17,117,517,997 included: "
              << included[0]
              << included[1]
              << included[2]
              << included[3]
              << included[16]
              << included[116]
              << included[516]
              << included[996] << " \n"; // 10100110
}

