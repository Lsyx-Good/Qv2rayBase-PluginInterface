#pragma once

#include "ConnectionsBaseTypes.hpp"

namespace Qv2rayPlugin::Connections
{
    class IProfileManager
    {
      public:
        IProfileManager() = default;
        virtual ~IProfileManager() = default;
        virtual bool IsConnected(const ConnectionGroupPair &id) const = 0;

        virtual const ProfileContent GetConnection(const ConnectionId &id) const = 0;
        virtual const GroupObject GetGroupObject(const GroupId &id) const = 0;
        virtual const ConnectionObject GetConnectionObject(const ConnectionId &id) const = 0;

        virtual const QList<ConnectionId> GetConnections() const = 0;
        virtual const QList<ConnectionId> GetConnections(const GroupId &groupId) const = 0;
        virtual const QList<GroupId> GetGroups() const = 0;
        virtual const QList<GroupId> GetGroups(const ConnectionId &connId) const = 0;

        virtual bool StartConnection(const ConnectionGroupPair &identifier) = 0;
        virtual void StopConnection() = 0;

        virtual const ConnectionGroupPair CreateConnection(const ProfileContent &root, const QString &displayName, const GroupId &groupId = DefaultGroupId) = 0;
        virtual void RenameConnection(const ConnectionId &id, const QString &newName) = 0;
        virtual void UpdateConnection(const ConnectionId &id, const ProfileContent &root) = 0;
        virtual bool RemoveFromGroup(const ConnectionId &id, const GroupId &gid) = 0;
        virtual bool MoveToGroup(const ConnectionId &id, const GroupId &sourceGid, const GroupId &targetGid) = 0;
        virtual bool LinkWithGroup(const ConnectionId &id, const GroupId &newGroupId) = 0;

        virtual const GroupId CreateGroup(const QString &displayName) = 0;
        virtual const std::optional<QString> DeleteGroup(const GroupId &id) = 0;
        virtual const std::optional<QString> RenameGroup(const GroupId &id, const QString &newName) = 0;
        virtual const RoutingId GetGroupRoutingId(const GroupId &id) = 0;
    };

} // namespace Qv2rayPlugin::Connections
