#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(size_t workersNumber, size_t buffersSize)
    : m_workersInfo(workersNumber)
{
    for (WorkerInfo& info : m_workersInfo) {
        info.currentLoad = 1.0 / workersNumber;
        info.history.resize(buffersSize);
    }
}

vector<double> LoadBalancer::getLoads() const
{
    vector<double> loads;
    for (const WorkerInfo& wi : m_workersInfo)
        loads.push_back(wi.currentLoad);
    return loads;
}

double LoadBalancer::median(const RingBuffer<double>& buffer)
{
    assert(buffer.size() != 0);

    vector<double> tmp;
    const size_t size = buffer.size();

    for (size_t i = 0; i < size; i++) {
        tmp.push_back(buffer[i]);
    }

    std::sort(tmp.begin(), tmp.end());

    if (size % 2)
        return tmp[size / 2];
    return 0.5 * (tmp[(size - 1) / 2] + tmp[size / 2]);
}
