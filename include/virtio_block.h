/* SPDX-License-Identifier: MIT */
/*
 * (c) 2014 Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 */

#pragma once

/**
 * \ingroup l4virtio
 * \defgroup l4virtio_block L4 VIRTIO Block Device
 * \{
 */

#include <l4/sys/types.h>

/**
 * Kinds of operation over a block device.
 */
enum L4virtio_block_operations
{
  L4VIRTIO_BLOCK_T_IN      = 0,       /**<  Read from device */
  L4VIRTIO_BLOCK_T_OUT     = 1,       /**<  Write to device */
  L4VIRTIO_BLOCK_T_FLUSH   = 4,       /**<  Flush data to disk */
  L4VIRTIO_BLOCK_T_GET_ID  = 8,       /**<  Get device ID */
  L4VIRTIO_BLOCK_T_DISCARD = 11,      /**<  Discard a range of sectors */
  L4VIRTIO_BLOCK_T_WRITE_ZEROES = 13, /**< Write zeroes to a range of sectors */
};

/**
 * Status of a finished block request.
 */
enum L4virtio_block_status
{
  L4VIRTIO_BLOCK_S_OK     = 0, /**<  Request finished successfully */
  L4VIRTIO_BLOCK_S_IOERR  = 1, /**<  IO error on device */
  L4VIRTIO_BLOCK_S_UNSUPP = 2  /**<  Operation is not supported */
};

/**
 * Header structure of a request for a block device.
 */
typedef struct l4virtio_block_header_t
{
  l4_uint32_t type;   /**<  Kind of request, see L4virtio_block_operations */
  l4_uint32_t ioprio; /**<  Priority (unused) */
  l4_uint64_t sector; /**<  First sector to read/write */
} l4virtio_block_header_t;

enum L4virtio_block_discard_flags_t
{
  L4VIRTIO_BLOCK_DISCARD_F_UNMAP    = 0x00000001UL,
  L4VIRTIO_BLOCK_DISCARD_F_RESERVED = 0xFFFFFFFEUL,
};

/**
 * Structure used for the write zeroes and discard commands.
 */
typedef struct l4virtio_block_discard_t
{
  l4_uint64_t sector;
  l4_uint32_t num_sectors;
  l4_uint32_t flags;
} l4virtio_block_discard_t;

/**
 * Device configuration for block devices.
 */
typedef struct l4virtio_block_config_t
{
  l4_uint64_t capacity;  /**<  Capacity of device in 512-byte sectors */
  l4_uint32_t size_max;  /**<  Maximum size of a single segment */
  l4_uint32_t seg_max;   /**<  Maximum number of segments per request */
  struct l4virtio_block_config_geometry_t
  {
    l4_uint16_t cylinders;
    l4_uint8_t heads;
    l4_uint8_t sectors;
  } geometry;
  l4_uint32_t blk_size; /**<  Block size of underlying disk. */
  struct l4virtio_block_config_topology_t
  {
    /**  Number of logical blocks per physical block (log2) */
    l4_uint8_t physical_block_exp;
    /**  Offset of first aligned logical block */
    l4_uint8_t alignment_offset;
    /**  Suggested minimum I/O size in blocks */
    l4_uint16_t min_io_size;
    /**  Suggested optimal (i.e. maximum) I/O size in blocks */
    l4_uint32_t opt_io_size;
  } topology;
  l4_uint8_t writeback;
  l4_uint8_t unused0[1];
  l4_uint16_t num_queues;
  l4_uint32_t max_discard_sectors;
  l4_uint32_t max_discard_seg;
  l4_uint32_t discard_sector_alignment;
  l4_uint32_t max_write_zeroes_sectors;
  l4_uint32_t max_write_zeroes_seg;
  l4_uint8_t write_zeroes_may_unmap;
  l4_uint8_t unused1[3];
} l4virtio_block_config_t;

/**\}*/
