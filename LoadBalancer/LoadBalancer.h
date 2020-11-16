#pragma once

#include "RingBuffer.h"

#include <algorithm>

using std::vector;

class LoadBalancer
{
public:
    LoadBalancer(size_t workersNumber, size_t buffersSize);

    vector<double> getLoads() const;

    template<class T>
    void setTimes(const vector<T> &times)
    {
        assert(times.size() == m_workersInfo.size());

        double sum = 0.0;
        for (size_t i = 0; i < times.size(); i++) {
            WorkerInfo& info = m_workersInfo[i];

            double v = info.currentLoad / times[i];
            info.history.append(v);

            double m = median(info.history);
            
            info.currentLoad = m;
            sum += m;
        }

        for (WorkerInfo &wi : m_workersInfo)
            wi.currentLoad /= sum;
    }

private:
    static double median(const RingBuffer<double>& buffer);

    struct WorkerInfo
    {
        double currentLoad;
        RingBuffer<double> history;
    };

    vector<WorkerInfo> m_workersInfo;
};
