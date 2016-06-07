/*
 * sensor_analog.c
 *
 * Copyright (C) 2016 Brian Masney <masneyb@onstation.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <mcp3002.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yadl.h"

static void _analog_init(yadl_config *config)
{
	if (config->adc == NULL) {
		fprintf(stderr, "You must specify the --adc argument\n");
		usage();
	}
	config->adc->adc_init(config);
}

static yadl_result *_analog_read_data(yadl_config *config)
{
	config->logger("Beginning to perform analog read. adc_millivolts=%d, adc_resolution=%d, adc_show_millivolts=%d\n",
			config->adc_millivolts, config->adc->adc_resolution, config->adc_show_millivolts);

	int reading = config->adc->adc_read(config);
	int read_millivolts = (float) reading * ((float) config->adc_millivolts / (float) config->adc->adc_resolution);
	config->logger("Got analog reading %d (%d millivolts).\n", reading, read_millivolts);

	yadl_result *result;
	result = malloc(sizeof(*result));
	result->value = config->adc_show_millivolts ? read_millivolts : reading;
	return result;
}

sensor analog_sensor_funcs = {
	.init = &_analog_init,
	.read = _analog_read_data
};
