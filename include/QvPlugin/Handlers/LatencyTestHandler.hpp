#pragma once

#include "QvPlugin/Common/CommonTypes.hpp"

namespace uvw
{
    class Loop;
}

namespace Qv2rayPlugin::Latency
{
    struct LatencyTestRequest
    {
        LatencyTestEngineId engine;
        ConnectionId id;
        QString host;
        int port;
    };

    struct LatencyTestResponse
    {
        LatencyTestEngineId engine;
        int total;
        int failed;
        QString error;
        long worst = LATENCY_TEST_VALUE_ERROR;
        long best = LATENCY_TEST_VALUE_ERROR;
        long avg = LATENCY_TEST_VALUE_ERROR;
    };

    class LatencyTestEngine
    {
      public:
        explicit LatencyTestEngine() = default;
        virtual ~LatencyTestEngine() = default;
        virtual LatencyTestResponse TestLatency(const LatencyTestRequest &request)
        {
            return TestLatencyAsync(nullptr, request);
        };
        virtual LatencyTestResponse TestLatencyAsync(std::shared_ptr<uvw::Loop>, const LatencyTestRequest &request)
        {
            return TestLatency(request);
        }
    };

    struct LatencyTestEngineInfo
    {
        LatencyTestEngineId Id;
        bool isAsync;
        QString Name;
        QString Description;
        std::function<std::unique_ptr<LatencyTestEngine>(void)> Create;
    };

    class ILatencyHandler
    {
      public:
        virtual QList<LatencyTestEngineInfo> PluginLatencyTestEngines() const = 0;
    };

} // namespace Qv2rayPlugin::Latency
