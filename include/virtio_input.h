/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2019, 2022, 2024 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 */
#pragma once

/**
 * \ingroup l4virtio
 * \defgroup l4virtio_input L4 VIRTIO Input Device
 * \{
 */

#include <l4/sys/types.h>

/**
 * Device information selectors
 */
enum L4virtio_input_config_select
{
  L4VIRTIO_INPUT_CFG_UNSET = 0,
  L4VIRTIO_INPUT_CFG_ID_NAME = 1,
  L4VIRTIO_INPUT_CFG_ID_SERIAL = 2,
  L4VIRTIO_INPUT_CFG_ID_DEVIDS = 3,
  L4VIRTIO_INPUT_CFG_PROP_BITS = 0x10,
  L4VIRTIO_INPUT_CFG_EV_BITS = 0x11,
  L4VIRTIO_INPUT_CFG_ABS_INFO = 0x12
};

/**
 * Information about the absolute axis in the underlying evdev implementation.
 */
typedef struct l4virtio_input_absinfo_t
{
  l4_uint32_t min;
  l4_uint32_t max;
  l4_uint32_t fuzz;
  l4_uint32_t flat;
  l4_uint32_t res;
} l4virtio_absinfo_t;

/**
 * Device ID information for the device.
 */
typedef struct l4virtio_input_devids_t
{
  l4_uint16_t bustype;
  l4_uint16_t vendor;
  l4_uint16_t product;
  l4_uint16_t version;
} l4virtio_input_devids_t;

/**
 * Device configuration for input devices.
 */
typedef struct l4virtio_input_config_t
{
  l4_uint8_t    select;
  l4_uint8_t    subsel;
  l4_uint8_t    size;
  l4_uint8_t    reserved[5];
  union
  {
    char string[128];
    l4_uint8_t bitmap[128];
    struct l4virtio_input_absinfo_t abs;
    struct l4virtio_input_devids_t ids;
  } u;
} l4virtio_input_config_t;

/**
 * Single event in event or status queue.
 */
typedef struct l4virtio_input_event_t
{
  l4_uint16_t type;
  l4_uint16_t code;
  l4_uint32_t value;
} l4virtio_input_event_t;

/**\} */
