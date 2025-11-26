/*
* Samsung Exynos5 SoC series FIMC-IS driver
 *
 * exynos5 fimc-is vender functions
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IS_VENDOR_CONFIG_H
#define IS_VENDOR_CONFIG_H

#ifdef CONFIG_SEC_DETECT
#include <linux/sec_detect.h>
#endif

#define USE_BINARY_PADDING_DATA_ADDED	/* for DDK signature */

#if defined(USE_BINARY_PADDING_DATA_ADDED) && (defined(CONFIG_USE_SIGNED_BINARY) || defined(CONFIG_SAMSUNG_PRODUCT_SHIP))
#define TZ_CONTROLLED_MEM_ATTRIBUTE 1
#else
#define TZ_CONTROLLED_MEM_ATTRIBUTE 0
#endif

/* Unified RSV config for all devices */
#ifdef CONFIG_SEC_DETECT
#include "rsv/is-vendor-config_rsv_unified.h"
#else
#if defined(CONFIG_CAMERA_RSV_V01)
#include "rsv/is-vendor-config_rsv_v01.h"
#elif defined(CONFIG_CAMERA_RSV_V02)
#include "rsv/is-vendor-config_rsv_v02.h"
#elif defined(CONFIG_CAMERA_RSV_V03)
#include "rsv/is-vendor-config_rsv_v03.h"
#elif defined(CONFIG_CAMERA_RSW_V11)
#include "rsw_v11/is-vendor-config_rsw_v11.h"
#else
#include "rsv/is-vendor-config_rsv_v02.h" /* Default */
#endif
#endif

/* Runtime configuration overrides based on sec_detect */
#ifdef CONFIG_SEC_DETECT

/* Helper function to get OIS stabilization delay */
static inline u32 is_vendor_get_ois_stabilization_delay(void)
{
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R0S:
		/* RSV_V01: S22 */
		return 7000; /* 7ms */
	case SEC_G0S:
		/* RSV_V02: S22+ */
		return 7000; /* 7ms */
	case SEC_B0S:
		/* RSV_V03: S22 Ultra */
		return 15000; /* 15ms */
	case SEC_R11S:
		/* S23 FE - PLACEHOLDER */
		return 7000; /* Default to 7ms */
	default:
		/* Fallback */
		return 7000;
	}
}

/* Helper function to get tele OIS tilt ROM ID */
/* Returns int to avoid needing full enum definition */
static inline int is_vendor_get_tele_ois_tilt_rom_id(void)
{
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R0S:
		/* RSV_V01: S22 */
		return 4; /* ROM_ID_REAR3 = TELE_OIS_ROM_ID */
	case SEC_G0S:
		/* RSV_V02: S22+ */
		return 4; /* ROM_ID_REAR3 = TELE_OIS_ROM_ID */
	case SEC_B0S:
		/* RSV_V03: S22 Ultra */
		return 6; /* ROM_ID_REAR4 = TELE2_OIS_ROM_ID */
	case SEC_R11S:
		/* S23 FE - PLACEHOLDER */
		return 4; /* ROM_ID_REAR3 - Default to REAR3 */
	default:
		/* Fallback */
		return 4; /* ROM_ID_REAR3 */
	}
}

#ifdef USE_SHARE_I2C_CLIENT

/* Helper functions to get sensor names at runtime based on sec_detect */
static inline u32 is_vendor_get_source_sensor_name(void)
{
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R0S:
		/* RSV_V01: S22 */
		return SENSOR_NAME_S5KGN3;
	case SEC_G0S:
		/* RSV_V02: S22+ */
		return SENSOR_NAME_S5KGN3;
	case SEC_B0S:
		/* RSV_V03: S22 Ultra */
		return SENSOR_NAME_S5KHM3;
	case SEC_R11S:
		/* RSW_V11: S23 FE */
		return SENSOR_NAME_S5KGN3;
	default:
		/* Fallback */
		return SOURSE_SENSOR_NAME;
	}
}

static inline u32 is_vendor_get_target_sensor_name(void)
{
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R0S:
		/* RSV_V01: S22 */
		return SENSOR_NAME_S5K2LD;
	case SEC_G0S:
		/* RSV_V02: S22+ */
		return SENSOR_NAME_S5K2LD;
	case SEC_B0S:
		/* RSV_V03: S22 Ultra */
		return SENSOR_NAME_S5KHM1;
	case SEC_R11S:
		/* RSW_V11: S23 FE */
		return SENSOR_NAME_S5K2LD;
	default:
		/* Fallback */
		return TARGET_SENSOR_NAME;
	}
}
#endif

/* Helper functions for runtime feature checks */
static inline bool is_vendor_use_camera_check_eeprom_status(void)
{
#ifdef CONFIG_SEC_DETECT
	return sec_get_mcd_feat(MCD_CAMERA_CHECK_EEPROM_STATUS);
#else
	return false;
#endif
}

static inline bool is_vendor_use_mipi_phy_tuning(void)
{
#ifdef CONFIG_SEC_DETECT
	return sec_get_mcd_feat(MCD_CAMERA_MIPI_PHY_TUNING);
#else
	return false;
#endif
}

static inline bool is_vendor_use_camera_heap_for_all(void)
{
#ifdef CONFIG_SEC_DETECT
	return sec_get_mcd_feat(MCD_CAMERA_HEAP_FOR_ALL);
#else
	return false;
#endif
}

static inline bool is_vendor_use_ois_hall_data_for_vdis(void)
{
#ifdef CONFIG_SEC_DETECT
	return sec_get_mcd_feat(MCD_OIS_HALL_DATA_FOR_VDIS);
#else
	return false;
#endif
}

/* Helper function to get camera retention share power string */
static inline const char *is_vendor_get_camera_retention_share_power(void)
{
#ifdef CONFIG_SEC_DETECT
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R0S:
	case SEC_G0S:
	case SEC_B0S:
		/* RSV: S22, S22+, S22 Ultra */
		return "VDDIO_1.8V_CAM";
	case SEC_R11S:
		/* RSW_V11: S23 FE */
		return "VDDIO_WIDE_1P8";
	default:
		/* Fallback */
		return "VDDIO_1.8V_CAM";
	}
#else
	return CAMERA_RETENTION_SHARE_POWER;
#endif
}

/* Helper function to get AK737X soft landing delay for Directors View */
static inline u32 is_vendor_get_ak737x_soft_landing_delay(void)
{
#ifdef CONFIG_SEC_DETECT
	enum SEC_devices device = sec_get_current_device();

	switch (device) {
	case SEC_R11S:
		/* RSW_V11: S23 FE */
		return 40; // 40ms
	default:
		/* RSV: S22, S22+, S22 Ultra - no delay */
		return 0;
	}
#else
	return AK737X_SOFT_LANDING_DELAY_ON_DIRECTORS_VIEW;
#endif
}

#endif /* CONFIG_SEC_DETECT */

#endif
