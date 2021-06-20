#pragma once

#include "Utils/BindableProps.hpp"

#include <QHashFunctions>
#include <QJsonObject>
#include <QString>

namespace Qv2rayPlugin::Common::_base_types::safetype
{
    using namespace std::chrono;
    template<typename>
    struct SafeJsonType : public QJsonObject
    {
        // clang-format off
        template<class... Args> explicit SafeJsonType(Args... args) : QJsonObject(args...) {};
        const QJsonObject &raw() const { return *this; }
        template<typename TTarget> TTarget CopyAs() const { return TTarget(raw()); }
        // clang-format on
        template<typename TTarget>
        TTarget ForceCopyAs() const
        {
            static_assert(has_JsonOperators<TTarget>::value, "TTarget must have QJS_FUNC_JSON or have JSON operators.");
            TTarget t;
            t.loadJson(raw());
            return t;
        }
    };

    template<typename T>
    struct IDType
    {
        // clang-format off
        explicit IDType() : m_id("null"){};
        explicit IDType(const QString &id) : m_id(id){};
        ~IDType() = default;
        inline bool operator==(const IDType<T> &rhs) const { return m_id == rhs.m_id; }
        inline bool operator!=(const IDType<T> &rhs) const { return m_id != rhs.m_id; }
        inline const QString toString() const { return m_id; }
        inline bool isNull() const { return m_id == "null"; }
        // clang-format on

      private:
        QString m_id;
    };

    template<typename T>
    inline size_t qHash(const IDType<T> &key) noexcept
    {
        return ::qHash(key.toString());
    }

    template<typename T>
    inline QDebug operator<<(QDebug debug, const IDType<T> &key)
    {
        return debug << key.toString();
    }
} // namespace Qv2rayPlugin::Common::_base_types::safetype

using namespace Qv2rayPlugin::Common::_base_types::safetype;

#define DeclareSafeJson(CLASS)                                                                                                                                           \
    namespace Qv2rayPlugin::Common::_base_types::safetype                                                                                                                \
    {                                                                                                                                                                    \
        class __##CLASS##__;                                                                                                                                             \
        typedef Qv2rayPlugin::Common::_base_types::safetype::SafeJsonType<__##CLASS##__> CLASS;                                                                          \
    }                                                                                                                                                                    \
    Q_DECLARE_METATYPE(Qv2rayPlugin::Common::_base_types::safetype::CLASS)

#define DeclareSafeID(type)                                                                                                                                              \
    namespace Qv2rayPlugin::Common::_base_types::safetype                                                                                                                \
    {                                                                                                                                                                    \
        class __##type;                                                                                                                                                  \
        typedef Qv2rayPlugin::Common::_base_types::safetype::IDType<__##type> type;                                                                                      \
    }                                                                                                                                                                    \
    Q_DECLARE_METATYPE(Qv2rayPlugin::Common::_base_types::safetype::type)

DeclareSafeJson(IOProtocolSettings);
DeclareSafeJson(IOStreamSettings);
DeclareSafeJson(InboundExtraSettings);
DeclareSafeJson(RuleExtraSettings);
DeclareSafeJson(BalancerSelectorSettings);

DeclareSafeID(GroupId);
DeclareSafeID(ConnectionId);
DeclareSafeID(RoutingId);
DeclareSafeID(PluginId);
DeclareSafeID(KernelId);
DeclareSafeID(LatencyTestEngineId);

#undef DeclareSafeJson
#undef DeclareSafeID
