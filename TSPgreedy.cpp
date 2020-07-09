#include<iostream>
#include<fstream>
#include <utility> 
#include <unordered_set>
#include <limits>
#include <time.h>

using namespace std;

double distSquared(pair<double, double> i, pair <double, double> j)
{
    double dx = (j.first - i.first);
    double dy = (j.second - i.second);
    return dx * dx + dy * dy;
}

void addDistance(uint64_t * integral, double * fraction, double add)
{
    int add_integral = (int)add;
    double add_frac = add - add_integral;
    (*integral) += add_integral;
    (*fraction) += add_frac;
    if ((*fraction) > (double)1.0)
    {
        (*fraction) -= (double)1.0;
        (*integral)++;
    }
}

int main()
{
    clock_t start = clock();
    ifstream InFile;
    InFile.open("nn.txt");
    vector<pair<double, double>>coordinates;
    unordered_set<int> cities;
    int num_cities;
    InFile >> num_cities;
    cout << "Number of cities: " << num_cities << "\n";
    for (int i = 0; i < num_cities; i++)
    {
        int int1;
        double fl1;
        double fl2;
        InFile >> int1;
        InFile >> fl1;
        InFile >> fl2;
        cities.insert(int1-1);
        coordinates.push_back(make_pair(fl1, fl2));
    }
    InFile.close();
    double tour_dist_fraction = 0;
    uint64_t tour_dist_int = 0;
    int currCity = 0;
    cities.erase(currCity);
    while (!cities.empty())
    {
        unordered_set<int>::const_iterator nextCityIt;
        double minDistSqr = numeric_limits<double>::max();
        for (auto it = cities.begin(); it != cities.end(); it++)
        {
            double currDistSqr = distSquared(coordinates[*it], coordinates[currCity]);
            if (currDistSqr <= minDistSqr)
            {
                if (currDistSqr != minDistSqr || *it < *nextCityIt)
                {
                    minDistSqr = currDistSqr;
                    nextCityIt = it;
                }
            }
        }
        addDistance(&tour_dist_int, &tour_dist_fraction, sqrt(minDistSqr));
        currCity = *nextCityIt;
        cities.erase(nextCityIt);
    }
    addDistance(&tour_dist_int, &tour_dist_fraction, sqrt(distSquared(coordinates[currCity], coordinates[0])));
    cout << "Min tour distance: " << tour_dist_int << "\n"; //1203406
    cout << "Time: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds.\n"; //2.8 seconds
    return 0;
}
