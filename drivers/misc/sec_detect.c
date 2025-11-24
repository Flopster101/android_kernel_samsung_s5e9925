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

#include <linux/sec_detect.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/string.h>
#ifdef CONFIG_SEC_DETECT_SYSFS
#include <linux/kobject.h>
#include <linux/sysfs.h>
#endif

static int g_sec_current_device = DEVICE_UNKNOWN;
static char g_sec_current_device_name[32] = "Unknown";

// Device feature flags
static bool sec_feat_flags[SEC_FEAT_COUNT] = {0};

bool sec_get_feat(enum sec_feat feat) {
	if (feat < 0 || feat >= SEC_FEAT_COUNT)
		return false;
	return sec_feat_flags[feat];
}
EXPORT_SYMBOL_GPL(sec_get_feat);

// Helper functions for each g_sec_ variable
enum SEC_devices sec_get_current_device(void) { return g_sec_current_device; }
EXPORT_SYMBOL_GPL(sec_get_current_device);

// Camera feature flags
static bool mcd_feat_flags[MCD_FEAT_COUNT] = {0};

bool sec_get_mcd_feat(enum mcd_feat feat) {
	if (feat < 0 || feat >= MCD_FEAT_COUNT)
		return false;
	return mcd_feat_flags[feat];
}
EXPORT_SYMBOL_GPL(sec_get_mcd_feat);

static bool g_detection_complete = false;

bool sec_is_detection_complete(void) {
    return g_detection_complete;
}
EXPORT_SYMBOL_GPL(sec_is_detection_complete);

#ifdef CONFIG_SEC_DETECT_SYSFS
// Sysfs attribute to show the current device name
static ssize_t device_name_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, 32, "%s\n", g_sec_current_device_name);
}

// Sysfs attribute to show the current device model
static ssize_t device_model_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	const char *model_name = "Unknown";
	if (g_sec_current_device >= 0 && g_sec_current_device < ARRAY_SIZE(device_names))
		model_name = device_names[g_sec_current_device];
	return snprintf(buf, 32, "%s\n", model_name);
}

static struct kobj_attribute device_name_attr = __ATTR(device_name, 0444, device_name_show, NULL);
static struct kobj_attribute device_model_attr = __ATTR(device_model, 0444, device_model_show, NULL);

static struct attribute *attrs[] = {
	&device_name_attr.attr,
	&device_model_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *device_kobj;
#endif

static inline void setup_camera_params(void) {
	switch (g_sec_current_device) {
	case SEC_R0S:
		// S22 camera features - TODO
		break;
	case SEC_G0S:
		// S22+ camera features - TODO
		break;
	case SEC_B0S:
		// S22 Ultra camera features - TODO
		break;
	case SEC_R11S:
		// S23 FE camera features - TODO
		break;
	default:
		break;
	}
}

// New function to print machine name and sec_ variables
static inline void print_sec_variables(const char *machine_name) {
	SEC_DETECT_LOG("Current machine name: %s\n", machine_name);
	// Example: SEC_DETECT_LOG("sec_feat_doze = %s\n", sec_get_feat(SEC_FEAT_DOZE) ? "true" : "false");
}

static int __init sec_detect_init(void) {
	struct device_node *root;
	const char *machine_name;
	int retval = 0;
#ifdef CONFIG_SEC_DETECT_SYSFS
	int sysfs_ret = 0;
#endif

	root = of_find_node_by_path("/");
	if (!root) {
		SEC_DETECT_LOG("Failed to find device tree root\n");
		smp_wmb();
		g_detection_complete = true;
		retval = -ENOENT;
		goto exit_no_root;
	}

	machine_name = of_get_property(root, "model", NULL);
	if (!machine_name)
		machine_name = of_get_property(root, "compatible", NULL);

	if (!machine_name) {
		SEC_DETECT_LOG("Failed to find machine name\n");
		smp_wmb();
		g_detection_complete = true;
		retval = -ENOENT;
		goto exit_put_root;
	}

	if (strstr(machine_name, "R0S") != NULL) {
		g_sec_current_device = SEC_R0S;
		strscpy(g_sec_current_device_name, "r0s", sizeof(g_sec_current_device_name));
		// sec_feat_flags[SEC_FEAT_DOZE] = true;
	} else if (strstr(machine_name, "G0S") != NULL) {
		g_sec_current_device = SEC_G0S;
		strscpy(g_sec_current_device_name, "g0s", sizeof(g_sec_current_device_name));
		// sec_feat_flags[SEC_FEAT_DOZE] = true;
	} else if (strstr(machine_name, "B0S") != NULL) {
		g_sec_current_device = SEC_B0S;
		strscpy(g_sec_current_device_name, "b0s", sizeof(g_sec_current_device_name));
		// sec_feat_flags[SEC_FEAT_DOZE] = true;
	} else if (strstr(machine_name, "R11S") != NULL) {
		g_sec_current_device = SEC_R11S;
		strscpy(g_sec_current_device_name, "r11s", sizeof(g_sec_current_device_name));
		// sec_feat_flags[SEC_FEAT_DOZE] = true;
	}

	// Print machine name and sec_ variables
	print_sec_variables(machine_name);

#ifdef CONFIG_SEC_DETECT_SYSFS
	// Create the sysfs entry
	device_kobj = kobject_create_and_add("sec_detect", kernel_kobj);
	if (!device_kobj) {
		SEC_DETECT_LOG("Failed to create sysfs kobject\n");
		retval = -ENOMEM;
		goto exit_put_root;
	}

	sysfs_ret = sysfs_create_group(device_kobj, &attr_group);
	if (sysfs_ret) {
		SEC_DETECT_LOG("Failed to create sysfs group, error %d\n", sysfs_ret);
		kobject_put(device_kobj);
		device_kobj = NULL;
	}
#endif

	setup_camera_params();
exit_put_root:
	of_node_put(root);
exit_no_root:
	smp_wmb();
	g_detection_complete = true;
	if (!retval)
		SEC_DETECT_LOG("Initialization complete and ready.\n");

	return retval;
}

static void __exit sec_detect_exit(void) {
#ifdef CONFIG_SEC_DETECT_SYSFS
	kobject_put(device_kobj);
#endif
	return;
}

rootfs_initcall(sec_detect_init); // runs before regular drivers init
module_exit(sec_detect_exit);

MODULE_AUTHOR("Flopster101");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Detects the Samsung device currently running this kernel. Also exposes device information through sysfs.");
