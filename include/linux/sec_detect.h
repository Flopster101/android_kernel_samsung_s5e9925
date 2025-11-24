// SPDX-License-Identifier: GPL-2.0-only
/*
 * Author: @Flopster101
 * Based on AkiraNoSushi's work for the Mi439 project.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_SEC_H
#define _LINUX_SEC_H

#include <linux/types.h>

#define SEC_DETECT_LOG(fmt, ...) printk(KERN_INFO "sec_detect: " fmt, ##__VA_ARGS__)
static const char *sec_detect_label = "sec_detect: ";

enum SEC_devices {
	DEVICE_UNKNOWN = -1,
	SEC_R0S,		// SM-S901B - S22
	SEC_G0S,		// SM-S906B - S22+
	SEC_B0S,		// SM-S908B - S22 Ultra
	SEC_R11S		// SM-S711B - S23 FE
};

static const char *const device_names[] = {
	[SEC_R0S] = "Galaxy S22",
	[SEC_G0S] = "Galaxy S22+",
	[SEC_B0S] = "Galaxy S22 Ultra",
	[SEC_R11S] = "Galaxy S23 FE",
};

// Device feature flags
enum sec_feat {
	SEC_FEAT_DOZE,				// Uses Samsung DRM Doze
	SEC_FEAT_COUNT
};

enum SEC_devices sec_get_current_device(void);

bool sec_get_feat(enum sec_feat feat);

// Camera feature flags
enum mcd_feat {
	MCD_CAMERA_REAR_DUAL_CAL,	// Example camera feature flag
	MCD_TYPE_RSV_V01,		// Camera RSV V01 type (r0s)
	MCD_TYPE_RSV_V02,		// Camera RSV V02 type (g0s)
	MCD_TYPE_RSV_V03,		// Camera RSV V03 type (b0s)
	MCD_FEAT_COUNT
};

bool sec_get_mcd_feat(enum mcd_feat feat);

bool sec_is_detection_complete(void);

#endif /* _LINUX_SEC_H */

