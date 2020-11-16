#include "LoadBalancer.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

template<class T>
static void printVector(const string& name, const vector<T>& v)
{
    cout << name << ": ";
    for (const T& elem : v)
        cout << elem << " ";
    cout << endl;
}

int main(int argc, char* argv[])
{
    const int workersNumber = 5, windowSize = 10;
    LoadBalancer loadBalancer(workersNumber, windowSize);

    auto seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(static_cast<unsigned int>(seed));
    
    vector<normal_distribution<double>*> speeds(workersNumber);
    for (int worker = 0; worker < workersNumber - 1; worker++) {
        speeds[worker] = new normal_distribution<double>(10.0, 1.0);
    }
    speeds[workersNumber - 1] = new normal_distribution<double>(100.0, 10.0);

    for (int i = 0; i < 2 * windowSize; i++) {
        
        cout << "Iteration " << i << endl;

        vector<double> loads = loadBalancer.getLoads();
        printVector("loads", loads);

        vector<double> times(workersNumber);

        double tMin = std::numeric_limits<double>::max();
        double tMax = std::numeric_limits<double>::min();
        
        for (int worker = 0; worker < workersNumber; worker++) {
            double& t = times[worker];
            t = loads[worker] / speeds[worker]->operator()(generator);

            if (t < tMin) tMin = t;
            if (t > tMax) tMax = t;
        }

        printVector("times", times);
        cout << "Tmax - Tmin = " << tMax - tMin << endl;

        loadBalancer.setTimes(times);

        cout << endl;
    }

    return 0;
}