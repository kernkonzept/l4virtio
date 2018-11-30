/*
 * (c) 2013-2014 Alexander Warg <warg@os.inf.tu-dresden.de>
 *               Matthias Lange <matthias.lange@kernkonzept.com>
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 */

#pragma once

/**
 * \defgroup l4virtio L4 VIRTIO Interface
 */

/**
 * \defgroup l4virtio_transport L4 VIRTIO Transport Layer
 * \ingroup l4virtio
 *
 * L4 specific VIRTIO Transport layer.
 *
 * The L4 specific VIRTIO Transport layer is based on L4Re::Dataspace as shared
 * memory and L4::Irq for signaling. The VIRTIO configuration space is mostly
 * based on a shared memory implementation too and accompanied by two IPC
 * functions to synchronize the configuration between device and driver.
 *
 * \{
 */

#include <l4/sys/utcb.h>
#include <l4/sys/ipc.h>
#include <l4/sys/types.h>

/// L4-VIRTIO protocol number
enum L4_virtio_protocol
{
  L4VIRTIO_PROTOCOL = 0,
};

/// L4-VIRTIO opcodes
enum L4_virtio_opcodes
{
  L4VIRTIO_OP_SET_STATUS  = 0,  ///< Set status register in device config
  L4VIRTIO_OP_CONFIG_QUEUE,     ///< Set queue config in device config
  L4VIRTIO_OP_REGISTER_IFACE,   ///< Register a transport driver to the device
  L4VIRTIO_OP_REGISTER_DS,      ///< Register a data space as transport memory
};

/// Virtio device IDs as reported in the driver's config space.
enum L4virtio_device_ids
{
  L4VIRTIO_ID_NET           = 1,      ///< Virtual ethernet card.
  L4VIRTIO_ID_BLOCK         = 2,      ///< General block device.
  L4VIRTIO_ID_CONSOLE       = 3,      ///< Simple device for data IO via ports.
  L4VIRTIO_ID_RNG           = 4,      ///< Entropy source.
  L4VIRTIO_ID_BALLOON       = 5,      ///< Memory balooning device.
  L4VIRTIO_ID_RPMSG         = 7,      ///< Device using rpmsg protocol.
  L4VIRTIO_ID_SCSI          = 8,      ///< SCSI host device.
  L4VIRTIO_ID_9P            = 9,      ///< Device using 9P transport protocol.
  L4VIRTIO_ID_RPROC_SERIAL  = 11,     ///< Rproc serial device.
  L4VIRTIO_ID_CAIF          = 12,     ///< Device using CAIF network protocol.

  L4VIRTIO_ID_SOCK          = 0x9999, ///< Inofficial socket device.
};

/// Virtio device status bits.
enum L4virtio_device_status
{
  L4VIRTIO_STATUS_ACKNOWLEDGE = 1,   ///< Guest OS has found device.
  L4VIRTIO_STATUS_DRIVER      = 2,   ///< Guest OS knows how to drive device.
  L4VIRTIO_STATUS_DRIVER_OK   = 4,   ///< Driver is set up.
  L4VIRTIO_STATUS_FEATURES_OK = 8,   ///< Driver has acknowledged feature set. (virtio 1.0 only)
  L4VIRTIO_STATUS_FAILED      = 0x80 ///< Fatal error in driver or device.
};

/**
 * VIRTIO IRQ status codes (l4virtio_config_hdr_t::irq_status).
 * \note l4virtio_config_hdr_t::irq_status is currently unused.
 */
enum L4_virtio_irq_status
{
  L4VIRTIO_IRQ_STATUS_VRING  = 1, ///< VRING IRQ pending flag
  L4VIRTIO_IRQ_STATUS_CONFIG = 2, ///< CONFIG IRQ pending flag
};

/**
 * L4-VIRTIO config header, provided in shared data space.
 */
typedef struct l4virtio_config_hdr_t
{
  l4_uint32_t magic;          ///< magic value (must be 'virt').
  l4_uint32_t version;        ///< VIRTIO version
  l4_uint32_t device;         ///< device ID
  l4_uint32_t vendor;         ///< vendor ID
  l4_uint32_t num_queues;     ///< number of virtqueues
  l4_uint32_t queues_offset;  ///< offset of virtqueue config array
  l4_uint32_t dev_cfg_offset; ///< offset of device specific config
  l4_uint32_t generation;     ///< generation counter

  /**
   * Device status register (read-only). The register must be written
   * using l4virtio_set_status().
   */
  l4_uint32_t status;
  l4_uint32_t irq_status;  ///< IRQ status (currently unused).

  l4_uint32_t host_features[8];  ///< host feature bitmap
  l4_uint32_t guest_features[8]; ///< guest feature bitmap
  /**
   * Guest page size (used for PFN to address translation).
   */
  l4_uint32_t guest_page_size;
} l4virtio_config_hdr_t;

/**
 * Queue configuration entry. An array of such entries is available at the
 * l4virtio_config_hdr_t::queues_offset in the config data space.
 */
typedef struct l4virtio_config_queue_t
{
  /// maximum number of descriptors supported by this queue (read)
  l4_uint16_t num_max;
  /// number of descriptors configured for this queue (read-write)
  l4_uint16_t num;
  /// alignment of the used ring (read-write)
  l4_uint32_t align;
  /// start page frame of the queue (read-write)
  l4_uint32_t pfn;
} l4virtio_config_queue_t;

EXTERN_C_BEGIN

/**
 * Get the pointer to the first queue config.
 * \param cfg  Pointer to the config header.
 * \return pointer to queue config of queue 0.
 */
L4_INLINE l4virtio_config_queue_t *
l4virtio_config_queues(l4virtio_config_hdr_t const *cfg)
{
  return (l4virtio_config_queue_t *)(((l4_addr_t)cfg) + cfg->queues_offset);
}

/**
 * Get the pointer to the device configuration.
 * \param cfg  Pointer to the config header.
 * \return pointer to device configuration structure.
 */
L4_INLINE void *
l4virtio_device_config(l4virtio_config_hdr_t const *cfg)
{
  return (void *)(((l4_addr_t)cfg) + cfg->dev_cfg_offset);
}


/**
 * Write the VIRTIO status register.
 * \note All other registers are accessed via shared memory.
 *
 * \param cap    Capability to the VIRTIO host
 * \param status Status word to write to the VIRTIO status.
 *
 * \return 0 on success, <0 on error.
 */
L4_CV int
l4virtio_set_status(l4_cap_idx_t cap, unsigned status) L4_NOTHROW;

/**
 * Trigger queue configuration of the given queue.
 *
 * Usually all queues are configured when the status is written to running.
 * However, in some cases queues shall be disabled or enabled dynamically, in
 * this case this function triggers a reconfiguration from the shared memory
 * register of the queue config.
 *
 * \param cap    Capability to the VIRTIO host.
 * \param queue  Queue index for the queue to be configured.
 *
 * \retval 0 on success.
 * \retval -L4_EIO    The queue's status is invalid.
 * \retval -L4_ERANGE The queue index exceeds the number of queues.
 * \retval -L4_EINVAL Otherwise.
 */
L4_CV int
l4virtio_config_queue(l4_cap_idx_t cap, unsigned queue) L4_NOTHROW;

/**
 * Register a shared data space with VIRTIO host
 *
 * \param cap     Capability to the VIRTIO host
 * \param ds_cap  Data-space capability to register. The lower 8 bits determine
 *                the rights mask with which the guest's rights are masked during
 *                the registration of the dataspace at the VIRTIO host.
 * \param base    VIRTIO guest physical start address of shared memory region
 * \param offset  Offset within the data space that is attached to the
 *                given \a base in the guest physical memory.
 * \param size    Size of the memory region in the guest
 *
 * \return 0 on success, < 0 on error
 */
L4_CV int
l4virtio_register_ds(l4_cap_idx_t cap, l4_cap_idx_t ds_cap,
                     l4_uint64_t base, l4_umword_t offset,
                     l4_umword_t size) L4_NOTHROW;

/**
 * Register client to the given L4-VIRTIO host
 *
 * \param cap     Capability to the L4-VIRTIO host
 * \param guest_irq  IRQ capability for valid IRQ object for host-to-guest
 *                   notifications
 * \param host_irq   Capability selector for receiving the guest-to-host
 *                   notifications IRQ capability.
 * \param config_ds  Capability for receiving the data-space capability for
 *                   the shared L4-VIRTIO config data space.
 *
 * \retval 0 on success.
 * \retval -L4_EINVAL The host did not receive the \a guest_irq cap.
 */
L4_CV int
l4virtio_register_iface(l4_cap_idx_t cap, l4_cap_idx_t guest_irq,
                        l4_cap_idx_t host_irq, l4_cap_idx_t config_ds) L4_NOTHROW;
EXTERN_C_END

/**\}*/
