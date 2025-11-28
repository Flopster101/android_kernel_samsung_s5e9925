/*
 *  Universal sensors core class
 *
 *  Author : Ryunkyun Park <ryun.park@samsung.com>
 */

#ifndef __SENSORS_CORE_H__
#define __SENSORS_CORE_H__

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/input.h>

int ssp_sensors_create_symlink(struct input_dev *inputdev);
void ssp_sensors_remove_symlink(struct input_dev *inputdev);
int ssp_sensors_register(struct device **pdev, void *drvdata, struct device_attribute *attributes[], char *name);
void ssp_sensors_unregister(struct device *dev, struct device_attribute *attributes[]);
void ssp_destroy_sensor_class(void);

#endif
