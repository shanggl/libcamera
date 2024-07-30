/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2019, Raspberry Pi Ltd
 *
 * camera information for ov5647 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperGc2093 : public CamHelper
{
public:
	CamHelperGc2093();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;
	void getDelays(int &exposureDelay, int &gainDelay,
		       int &vblankDelay, int &hblankDelay) const override;
	unsigned int hideFramesStartup() const override;
	unsigned int hideFramesModeSwitch() const override;
	unsigned int mistrustFramesStartup() const override;
	unsigned int mistrustFramesModeSwitch() const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 4;
};

/*
 * OV5647 doesn't output metadata, so we have to use the "unicam parser" which
 * works by counting frames.
 */

CamHelperGc2093::CamHelperGc2093()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperGc2093::gainCode(double gain) const
{
	return static_cast<uint32_t>(gain * 16.0);
}

double CamHelperGc2093::gain(uint32_t gainCode) const
{
	return static_cast<double>(gainCode) / 16.0;
}

void CamHelperGc2093::getDelays(int &exposureDelay, int &gainDelay,
				int &vblankDelay, int &hblankDelay) const
{
	/*
	 * We run this sensor in a mode where the gain delay is bumped up to
	 * 2. It seems to be the only way to make the delays "predictable".
	 */
	exposureDelay = 2;
	gainDelay = 2;
	vblankDelay = 2;
	hblankDelay = 2;
}

unsigned int CamHelperGc2093::hideFramesStartup() const
{
	/*
	 * On startup, we get a couple of under-exposed frames which
	 * we don't want shown.
	 */
	return 2;
}

unsigned int CamHelperGc2093::hideFramesModeSwitch() const
{
	/*
	 * After a mode switch, we get a couple of under-exposed frames which
	 * we don't want shown.
	 */
	return 2;
}

unsigned int CamHelperGc2093::mistrustFramesStartup() const
{
	/*
	 * First couple of frames are under-exposed and are no good for control
	 * algos.
	 */
	return 2;
}

unsigned int CamHelperGc2093::mistrustFramesModeSwitch() const
{
	/*
	 * First couple of frames are under-exposed even after a simple
	 * mode switch, and are no good for control algos.
	 */
	return 2;
}

static CamHelper *create()
{
	return new CamHelperGc2093();
}

static RegisterCamHelper reg("gc2093", &create);

