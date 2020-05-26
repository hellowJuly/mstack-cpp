#pragma once
#include "icmp.hpp"
#include "ipv4.hpp"
#include "ipv4_addr.hpp"
#include "layer.hpp"
#include "packets.hpp"
#include <optional>
namespace mstack {

class l3_hook : public base_hook_funcs<l3_packet, l4_packet> {
public:
    icmp_hook_t icmp_hook;
    ipv4_hook_t ipv4_hook;

    void
    hook_register_protocol(protocol_interface<l3_packet>& in_protocol)
    {
        DLOG(INFO) << "[REGISTER] " << in_protocol.tag();
        return;
    }

    void
    hook_dispatch(std::optional<l3_packet>& in_packet)
    {
        return;
    }

    void hook_gather(std::optional<l3_packet>& in_packet)
    {
        if (!in_packet) {
            return;
        }
        if (in_packet->_proto == icmp::PROTO) {
            icmp_hook.icmp_filter(in_packet);
        }
        ipv4_hook.ipv4_filter(in_packet);
        return;
    }

    std::optional<l4_packet>
    hook_forward_to_upper(std::optional<l3_packet> in_packet)
    {
        if (!in_packet) {
            return std::nullopt;
        }
        return std::nullopt;
    }

    std::optional<l3_packet>
    hook_forward_from_upper(std::optional<l4_packet> in_packet)
    {
        if (!in_packet) {
            return std::nullopt;
        }
        return std::nullopt;
    }
};

using l3_layer = mstack::layer<l3_packet, l4_packet, l3_hook>;

template <>
std::optional<int> get_proto(std::optional<l3_packet>& packet)
{
    DLOG(INFO) << "[PROTO L3_PACKET] " << std::setiosflags(std::ios::uppercase) << std::hex << packet->_proto;
    return packet->_proto;
}
};