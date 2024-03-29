#ifndef LLARP_PATH_TRANSIT_HOP_HPP
#define LLARP_PATH_TRANSIT_HOP_HPP

#include <constants/path.hpp>
#include <path/ihophandler.hpp>
#include <path/path_types.hpp>
#include <routing/handler.hpp>
#include <router_id.hpp>

namespace llarp
{
  struct LR_CommitRecord;

  namespace dht
  {
    struct GotIntroMessage;
  }

  namespace path
  {
    struct TransitHopInfo
    {
      TransitHopInfo() = default;
      TransitHopInfo(const RouterID& down, const LR_CommitRecord& record);

      PathID_t txID, rxID;
      RouterID upstream;
      RouterID downstream;

      std::ostream&
      print(std::ostream& stream, int level, int spaces) const;

      struct PathIDHash
      {
        std::size_t
        operator()(const PathID_t& a) const
        {
          return AlignedBuffer< PathID_t::SIZE >::Hash()(a);
        }
      };

      struct Hash
      {
        std::size_t
        operator()(TransitHopInfo const& a) const
        {
          std::size_t idx0 = RouterID::Hash()(a.upstream);
          std::size_t idx1 = RouterID::Hash()(a.downstream);
          std::size_t idx2 = PathIDHash()(a.txID);
          std::size_t idx3 = PathIDHash()(a.rxID);
          return idx0 ^ idx1 ^ idx2 ^ idx3;
        }
      };
    };

    inline bool
    operator==(const TransitHopInfo& lhs, const TransitHopInfo& rhs)
    {
      return std::tie(lhs.txID, lhs.rxID, lhs.upstream, lhs.downstream)
          == std::tie(rhs.txID, rhs.rxID, rhs.upstream, rhs.downstream);
    }

    inline bool
    operator!=(const TransitHopInfo& lhs, const TransitHopInfo& rhs)
    {
      return !(lhs == rhs);
    }

    inline bool
    operator<(const TransitHopInfo& lhs, const TransitHopInfo& rhs)
    {
      return std::tie(lhs.txID, lhs.rxID, lhs.upstream, lhs.downstream)
          < std::tie(rhs.txID, rhs.rxID, rhs.upstream, rhs.downstream);
    }

    inline std::ostream&
    operator<<(std::ostream& out, const TransitHopInfo& info)
    {
      return info.print(out, -1, -1);
    }

    struct TransitHop : public IHopHandler, public routing::IMessageHandler
    {
      TransitHop();

      TransitHopInfo info;
      SharedSecret pathKey;
      ShortHash nonceXOR;
      llarp_time_t started = 0;
      // 10 minutes default
      llarp_time_t lifetime = default_lifetime;
      llarp_proto_version_t version;
      llarp_time_t m_LastActivity = 0;

      bool
      IsEndpoint(const RouterID& us) const
      {
        return info.upstream == us;
      }

      llarp_time_t
      ExpireTime() const;

      llarp_time_t
      LastRemoteActivityAt() const override
      {
        return m_LastActivity;
      }

      std::ostream&
      print(std::ostream& stream, int level, int spaces) const;

      bool
      Expired(llarp_time_t now) const override;

      bool
      ExpiresSoon(llarp_time_t now, llarp_time_t dlt) const override
      {
        return now >= ExpireTime() - dlt;
      }

      // send routing message when end of path
      bool
      SendRoutingMessage(const routing::IMessage& msg,
                         AbstractRouter* r) override;

      // handle routing message when end of path
      bool
      HandleRoutingMessage(const routing::IMessage& msg, AbstractRouter* r);

      bool
      HandleDataDiscardMessage(const routing::DataDiscardMessage& msg,
                               AbstractRouter* r) override;

      bool
      HandlePathConfirmMessage(const routing::PathConfirmMessage& msg,
                               AbstractRouter* r) override;
      bool
      HandlePathTransferMessage(const routing::PathTransferMessage& msg,
                                AbstractRouter* r) override;
      bool
      HandlePathLatencyMessage(const routing::PathLatencyMessage& msg,
                               AbstractRouter* r) override;

      bool
      HandleObtainExitMessage(const routing::ObtainExitMessage& msg,
                              AbstractRouter* r) override;

      bool
      HandleUpdateExitVerifyMessage(const routing::UpdateExitVerifyMessage& msg,
                                    AbstractRouter* r) override;

      bool
      HandleTransferTrafficMessage(const routing::TransferTrafficMessage& msg,
                                   AbstractRouter* r) override;

      bool
      HandleUpdateExitMessage(const routing::UpdateExitMessage& msg,
                              AbstractRouter* r) override;

      bool
      HandleGrantExitMessage(const routing::GrantExitMessage& msg,
                             AbstractRouter* r) override;
      bool
      HandleRejectExitMessage(const routing::RejectExitMessage& msg,
                              AbstractRouter* r) override;

      bool
      HandleCloseExitMessage(const routing::CloseExitMessage& msg,
                             AbstractRouter* r) override;

      bool
      HandleHiddenServiceFrame(
          ABSL_ATTRIBUTE_UNUSED const service::ProtocolFrame& frame) override
      {
        /// TODO: implement me
        LogWarn("Got hidden service data on transit hop");
        return false;
      }

      bool
      HandleGotIntroMessage(const dht::GotIntroMessage& msg);

      bool
      HandleDHTMessage(const dht::IMessage& msg, AbstractRouter* r) override;

      // handle data in upstream direction
      bool
      HandleUpstream(const llarp_buffer_t& X, const TunnelNonce& Y,
                     AbstractRouter* r) override;

      // handle data in downstream direction
      bool
      HandleDownstream(const llarp_buffer_t& X, const TunnelNonce& Y,
                       AbstractRouter* r) override;
    };

    inline std::ostream&
    operator<<(std::ostream& out, const TransitHop& h)
    {
      return h.print(out, -1, -1);
    }
  }  // namespace path
}  // namespace llarp

#endif
