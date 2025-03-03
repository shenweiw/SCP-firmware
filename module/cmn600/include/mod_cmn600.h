
/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_CMN600_H
#define MOD_CMN600_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * \addtogroup GroupModules Modules
 * @{
 */

/*!
 * \defgroup GroupModuleCMN600 CMN600
 *
 * \brief Arm Coherent Mesh Network (CMN) 600 module
 *
 * \details This module adds support for the CMN600 interconnect
 * @{
 */

/*!
 * \brief Max entries of the Memory Map table
 *        for the Home Agent.
 */
#define MAX_HA_MMAP_ENTRIES     4

/*!
 * \brief Module API indices
 */
enum mod_cmn600_api_idx {
    /*! Index of the PPU_V1 power state observer API */
    MOD_CMN600_API_IDX_PPU_OBSERVER,

    /*! Index of the CCIX config setup API */
    MOD_CMN600_API_IDX_CCIX_CONFIG,

    /*! Number of APIs */
    MOD_CMN600_API_COUNT
};

/*!
 * \brief Memory region configuration type
 */
enum mod_cmn600_memory_region_type {
    /*! Input/Output region (serviced by dedicated HN-I and HN-D nodes) */
    MOD_CMN600_MEMORY_REGION_TYPE_IO,

    /*!
     * Region backed by the system cache (serviced by all HN-F nodes in the
     * system)
     */
    MOD_CMN600_MEMORY_REGION_TYPE_SYSCACHE,

    /*!
     * Sub region of the system cache for non-hashed access (serviced by
     * dedicated SN-F nodes).
     */
    MOD_CMN600_REGION_TYPE_SYSCACHE_SUB,

    /*!
     * Region used for CCIX access.
     */
    MOD_CMN600_REGION_TYPE_CCIX,
};

/*!
 * \brief Memory region map descriptor
 */
struct mod_cmn600_memory_region_map {
    /*! Base address */
    uint64_t base;

    /*! Region size in bytes */
    uint64_t size;

    /*! Region configuration type */
    enum mod_cmn600_memory_region_type type;

    /*!
     * \brief Target node identifier
     *
     * \note Not used for \ref
     *      mod_cmn600_memory_region_type.MOD_CMN600_MEMORY_REGION_TYPE_SYSCACHE
     *      memory regions as it uses the pool of HN-F nodes available in the
     *      system
     */
    unsigned int node_id;
};

/*!
 * \brief CMN600 configuration data
 */
struct mod_cmn600_config {
    /*! Peripheral base address. */
    uintptr_t base;

    /*! Size along x-axis of the interconnect mesh */
    unsigned int mesh_size_x;

    /*! Size along y-axis of the interconnect mesh */
    unsigned int mesh_size_y;

    /*! Default HN-D node identifier containing the global configuration */
    unsigned int hnd_node_id;

    /*!
     * \brief Table of SN-Fs used as targets for the HN-F nodes
     *
     * \details Each entry of this table corresponds to a HN-F node in the
     *      system. The HN-F's logical identifiers are used as indices in this
     *      table
     */
    const unsigned int *snf_table;

    /*! Number of entries in the \ref snf_table */
    size_t snf_count;

    /*! Host SA count */
    unsigned int sa_count;

    /*! Host CXG LA Node id */
    unsigned int cxgla_node_id;

    /*! Table of region memory map entries */
    const struct mod_cmn600_memory_region_map *mmap_table;

    /*! Number of entries in the \ref mmap_table */
    size_t mmap_count;

    /*! Identifier of the clock that this device depends on */
    fwk_id_t clock_id;
};

/*!
 * \brief CCIX HA memory table entry structure
 */
struct mod_cmn600_ccix_ha_mmap {

    /*! HA node ID */
    uint8_t ha_id;

    /*! Base address */
    uint64_t base;

    /*! Region size in bytes */
    uint64_t size;
};


/*!
 * \brief CMN600 CCIX configuration data from remote node
 */
struct mod_cmn600_ccix_remote_node_config {
    /*! Remote RA count */
    uint8_t remote_ra_count;

    /*! Remote HA count */
    uint8_t remote_sa_count;

    /*! Remote HA count */
    uint8_t remote_ha_count;

    /*! PCIe traffic class used for CCIX Virtual Channel */
    uint8_t ccix_tc;

    /*! CCIX message packing flag */
    bool ccix_msg_pack_enable;

    /*! PCIe bus number on which CCIX link is enabled */
    uint8_t pcie_bus_num;

    /*! CCIX link identifier */
    uint8_t ccix_link_id;

    /*! optimised tlp mode */
    bool    ccix_opt_tlp;

    /*! Remote HA memory map table count */
    uint8_t remote_ha_mmap_count;

    /*! Remote HA memory map table */
    struct mod_cmn600_ccix_ha_mmap remote_ha_mmap[MAX_HA_MMAP_ENTRIES];
};

/*!
 * \brief CMN600 CCIX configuration data from host node
 */
struct mod_cmn600_ccix_host_node_config {
    /*! Host RA count */
    uint8_t host_ra_count;

    /*! Host SA count */
    uint8_t host_sa_count;

    /*! Host HA count */
    uint8_t host_ha_count;

    /*! CCIX HA memory map table count for endpoints */
    uint8_t ccix_host_mmap_count;

    /*! CCIX HA memory map table for endpoints */
    struct mod_cmn600_ccix_ha_mmap ccix_host_mmap[MAX_HA_MMAP_ENTRIES];

};

/*!
 * \brief CMN600 CCIX configuration interface
 */
struct mod_cmn600_ccix_config_api {
   /*!
    * \brief Get the CCIX host configuration
    *
    * \param[out] config CCIX host configuration
    *
    * \retval FWK_SUCCESS if the operation succeed.
    * \return one of the error code otherwise.
    */
   int (*get_config)(struct mod_cmn600_ccix_host_node_config *config);
   /*!
    * \brief set the CCIX endpoint configuration
    *
    * \param[in] config CCIX endpoint configuration
    *
    * \retval FWK_SUCCESS if the operation succeed.
    * \return one of the error code otherwise.
    */
   int (*set_config)(struct mod_cmn600_ccix_remote_node_config *config);
   /*!
    * \brief Interface to trigger the protocol credit exchange
    *
    * \param  link_id Link on which the protocol credit exchange
    *                 would initiate.
    *
    * \retval FWK_SUCCESS if the operation succeed.
    * \return one of the error code otherwise.
    */
   int (*exchange_protocol_credit)(uint8_t link_id);
   /*!
    * \brief Interface to configure for system coherency
    *
    * \param  link_id Link on which the coherency has to
    *                 be enabled.
    *
    * \retval FWK_SUCCESS if the operation succeed.
    * \return one of the error code otherwise.
    */
   int (*enter_system_coherency)(uint8_t link_id);
};


/*!
 * @}
 */

/*!
 * @}
 */

#endif /* MOD_CMN600_H */
