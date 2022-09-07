/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021-2022, Ideas On Board
 *
 * ipa_context.h - RkISP1 IPA Context
 *
 */

#pragma once

#include <linux/rkisp1-config.h>

#include <libcamera/base/utils.h>

#include <libcamera/geometry.h>

#include <libipa/fc_queue.h>

namespace libcamera {

namespace ipa::rkisp1 {

struct IPASessionConfiguration {
	struct {
		utils::Duration minShutterSpeed;
		utils::Duration maxShutterSpeed;
		double minAnalogueGain;
		double maxAnalogueGain;
		struct rkisp1_cif_isp_window measureWindow;
	} agc;

	struct {
		struct rkisp1_cif_isp_window measureWindow;
		bool enabled;
	} awb;

	struct {
		bool enabled;
	} lsc;

	struct {
		utils::Duration lineDuration;
		Size size;
	} sensor;

	struct {
		rkisp1_cif_isp_version revision;
	} hw;
};

struct IPAActiveState {
	struct {
		uint32_t exposure;
		double gain;
	} agc;

	struct {
		struct {
			struct {
				double red;
				double green;
				double blue;
			} manual;
			struct {
				double red;
				double green;
				double blue;
			} automatic;
		} gains;

		bool autoEnabled;
	} awb;

	struct {
		int8_t brightness;
		uint8_t contrast;
		uint8_t saturation;
		bool updateParams;
	} cproc;

	struct {
		bool denoise;
		bool updateParams;
	} dpf;

	struct {
		uint8_t denoise;
		uint8_t sharpness;
		bool updateParams;
	} filter;
};

struct IPAFrameContext : public FrameContext {
	struct {
		uint32_t exposure;
		double gain;
	} agc;

	struct {
		struct {
			double red;
			double green;
			double blue;
		} gains;

		double temperatureK;
		bool autoEnabled;
	} awb;

	struct {
		uint32_t exposure;
		double gain;
	} sensor;
};

struct IPAContext {
	IPASessionConfiguration configuration;
	IPAActiveState activeState;

	FCQueue<IPAFrameContext> frameContexts;
};

} /* namespace ipa::rkisp1 */

} /* namespace libcamera*/
