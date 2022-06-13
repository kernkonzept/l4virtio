/* SPDX-License-Identifier: GPL-2.0-only OR License-Ref-kk-custom */
/*
 * Copyright (C) 2022 Kernkonzept GmbH.
 * Author(s): Stephan Gerhold <stephan.gerhold@kernkonzept.com>
 */

#pragma once

/**
 * \ingroup l4virtio
 * \defgroup l4virtio_net L4 VIRTIO Network Device
 * \{
 */

#include <l4/sys/types.h>

/**
 * Header structure of a request for a network device.
 */
typedef struct l4virtio_net_header_t
{
  l4_uint8_t flags;
  l4_uint8_t gso_type;
  l4_uint16_t hdr_len;
  l4_uint16_t gso_size;
  l4_uint16_t csum_start;
  l4_uint16_t csum_offset;
  l4_uint16_t num_buffers;
} l4virtio_net_header_t;

/**
 * Device configuration for network devices.
 */
typedef struct l4virtio_net_config_t
{
  l4_uint8_t mac[6];
  l4_uint16_t status;
  l4_uint16_t max_virtqueue_pairs;
  l4_uint16_t mtu;
  l4_uint32_t speed;
  l4_uint8_t duplex;
} l4virtio_net_config_t;

/** Network device feature bits. */
enum L4virtio_net_feature_bits
{
  L4VIRTIO_NET_F_CSUM = 0,
  L4VIRTIO_NET_F_GUEST_CSUM = 1,
  L4VIRTIO_NET_F_MTU = 3,
  L4VIRTIO_NET_F_MAC = 5,
  L4VIRTIO_NET_F_GUEST_TSO4 = 7,
  L4VIRTIO_NET_F_GUEST_TSO6 = 8,
  L4VIRTIO_NET_F_GUEST_ECN = 9,
  L4VIRTIO_NET_F_GUEST_UFO = 10,
  L4VIRTIO_NET_F_HOST_TSO4 = 11,
  L4VIRTIO_NET_F_HOST_TSO6 = 12,
  L4VIRTIO_NET_F_HOST_ECN = 13,
  L4VIRTIO_NET_F_HOST_UFO = 14,
  L4VIRTIO_NET_F_MRG_RXBUF = 15,
  L4VIRTIO_NET_F_STATUS = 16,
  L4VIRTIO_NET_F_CTRL_VQ = 17,
  L4VIRTIO_NET_F_CTRL_RX = 18,
  L4VIRTIO_NET_F_CTRL_VLAN = 19,
  L4VIRTIO_NET_F_GUEST_ANNOUNCE = 21,
  L4VIRTIO_NET_F_MQ = 22,
  L4VIRTIO_NET_F_CTRL_MAC_ADDR = 23,
};

/**\}*/
