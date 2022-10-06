// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "Types.r"

#include "ResourceConstants.h"

#define kProjectName "hellolite"

resource 'ALRT' (r_ALRT_fatal_error) {
	{84, 64, 198, 448},
	r_DITL_fatal_error,
	{
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'DITL' (r_DITL_fatal_error) {
	{
		{84, 316, 104, 374},
		Button {
			enabled,
			"OK"
		},
		{10, 72, 74, 374},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'MBAR' (r_MBAR) {
	{
		r_MENU_apple;
		r_MENU_file;
		r_MENU_edit
	}
};

resource 'MENU' (r_MENU_apple) {
	r_MENU_apple,
	kMenuStdMenuProc,
	0b1111111111111111111111111111101,
	enabled,
	apple,
	{
		"About " kProjectName "\0xC9", noIcon, noKey, noMark, plain;
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (r_MENU_file) {
	r_MENU_file,
	kMenuStdMenuProc,
	0b1111111111111111111111111111011,
	enabled,
	"File",
	{
		"New", noIcon, "N", noMark, plain;
		"Close", noIcon, "W", noMark, plain;
		"-", noIcon, noKey, noMark, plain;
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (r_MENU_edit) {
	r_MENU_edit,
	kMenuStdMenuProc,
	0b1111111111111111111111111111101,
	enabled,
	"Edit",
	{
		"Undo", noIcon, "Z", noMark, plain;
		"-", noIcon, noKey, noMark, plain;
		"Cut", noIcon, "X", noMark, plain;
		"Copy", noIcon, "C", noMark, plain;
		"Paste", noIcon, "V", noMark, plain;
		"Clear", noIcon, noKey, noMark, plain;
		"Select All", noIcon, "A", noMark, plain
	}
};

resource 'SIZE' (r_SIZE, purgeable) {
	reserved,
	acceptSuspendResumeEvents,
	reserved,
	canBackground,
	doesActivateOnFGSwitch,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreAppDiedEvents,
	is32BitCompatible,
	notHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	dontUseTextEditServices,
	notDisplayManagerAware,
	reserved,
	reserved,
	kPreferredSize * 1024,
	kMinimumSize * 1024
};

resource 'STR#' (r_STRx_error_messages, preload) {
	{
		kProjectName " cannot start because the Appearance Extension is not installed.",
		kProjectName " cannot start because a required application resource is missing.",
		"There is not enough memory to perform that operation.",
		"An unexpected error occurred."
	}
};

resource 'WIND' (r_WIND_browser, purgeable) {
	{40, 40, 200, 200},
	kWindowFullZoomGrowDocumentProc,
	invisible,
	goAway,
	0x0,
	"",
	noAutoCenter
};
