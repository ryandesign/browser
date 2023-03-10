// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "Script.r"
#include "Types.r"

#include "ResourceConstants.h"
#include "config.h"

#define k_app_name "hellolite"
#define k_app_copyright_string "\0xA9 2022 Ryan C Schmidt"

#define k_about_window_left 40
#define k_about_window_top 40
#define k_about_window_width 200
#define k_about_window_height 100
#define k_about_window_right (k_about_window_left + k_about_window_width)
#define k_about_window_bottom (k_about_window_top + k_about_window_height)

#define k_dialog_padding 10
#define k_about_text_left k_dialog_padding
#define k_about_text_top k_dialog_padding
#define k_about_text_right (k_about_window_width - k_dialog_padding)
#define k_about_text_bottom (k_about_window_height - k_dialog_padding)

#define k_window_left 40
#define k_window_top 40
#define k_window_width 200
#define k_window_height 200
#define k_window_right (k_window_left + k_window_width)
#define k_window_bottom (k_window_top + k_window_height)

#define k_header_button_x_padding 4
#define k_header_button_y_padding 4
#define k_header_button_width 20
#define k_header_button_height 20
#define k_header_height (k_header_button_height + 2 * k_header_button_y_padding)

#define k_back_button_left (k_header_button_x_padding)
#define k_back_button_top (k_header_button_y_padding)
#define k_back_button_right (k_back_button_left + k_header_button_width)
#define k_back_button_bottom (k_back_button_top + k_header_button_height)

#define k_forward_button_left (k_back_button_right - 1)
#define k_forward_button_top (k_back_button_top)
#define k_forward_button_right (k_forward_button_left + k_header_button_width)
#define k_forward_button_bottom (k_back_button_bottom)

#define k_home_button_left (k_forward_button_right + k_header_button_x_padding)
#define k_home_button_top (k_forward_button_top)
#define k_home_button_right (k_home_button_left + k_header_button_width)
#define k_home_button_bottom (k_forward_button_bottom)

#define k_reload_button_right (k_window_width - k_header_button_x_padding)
#define k_reload_button_bottom (k_home_button_bottom)
#define k_reload_button_left (k_reload_button_right - k_header_button_width)
#define k_reload_button_top (k_home_button_top)

#define k_text_field_inset 3
#define k_address_bar_left (k_home_button_right + k_header_button_x_padding + k_text_field_inset)
#define k_address_bar_top (k_home_button_top + k_text_field_inset)
#define k_address_bar_right (k_reload_button_left - k_header_button_x_padding - k_text_field_inset)
#define k_address_bar_bottom (k_home_button_bottom - k_text_field_inset)

#define k_browser_left 0
#define k_browser_top (k_header_height + 1)
#define k_browser_right (k_window_width - 15)
#define k_browser_bottom (k_window_height - 15)

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

resource 'CNTL' (r_CNTL_header, purgeable) {
	{-1, -1, k_header_height + 1, k_window_width + 1},
	0,
	visible,
	0,
	0,
	kControlWindowHeaderProc,
	k_resize_horizontally,
	""
};

resource 'CNTL' (r_CNTL_back_button, purgeable) {
	{k_back_button_top, k_back_button_left, k_back_button_bottom, k_back_button_right},
	0,
	visible,
	r_icsx_back,
	kControlBehaviorPushbutton << 8 | kControlContentIconSuiteRes,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};

resource 'CNTL' (r_CNTL_forward_button, purgeable) {
	{k_forward_button_top, k_forward_button_left, k_forward_button_bottom, k_forward_button_right},
	0,
	visible,
	r_icsx_forward,
	kControlBehaviorPushbutton << 8 | kControlContentIconSuiteRes,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};

resource 'CNTL' (r_CNTL_home_button, purgeable) {
	{k_home_button_top, k_home_button_left, k_home_button_bottom, k_home_button_right},
	0,
	visible,
	r_icsx_home,
	kControlBehaviorPushbutton << 8 | kControlContentIconSuiteRes,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};

resource 'CNTL' (r_CNTL_reload_button, purgeable) {
	{k_reload_button_top, k_reload_button_left, k_reload_button_bottom, k_reload_button_right},
	0,
	visible,
	r_icsx_reload,
	kControlBehaviorPushbutton << 8 | kControlContentIconSuiteRes,
	kControlBevelButtonSmallBevelProc,
	k_move_horizontally,
	""
};

resource 'CNTL' (r_CNTL_address_bar, purgeable) {
	{k_address_bar_top, k_address_bar_left, k_address_bar_bottom, k_address_bar_right},
	0,
	visible,
	0,
	0,
	kControlEditTextProc,
	k_resize_horizontally,
	""
};

resource 'CNTL' (r_CNTL_horizontal_scroll_bar, purgeable) {
	{k_window_height - 15, -1, k_window_height + 1, k_window_width - 14},
	0,
	visible,
	0,
	0,
	kControlScrollBarProc,
	k_move_vertically | k_resize_horizontally,
	""
};

resource 'CNTL' (r_CNTL_vertical_scroll_bar, purgeable) {
	{k_header_height, k_window_width - 15, k_window_height - 14, k_window_width + 1},
	0,
	visible,
	0,
	0,
	kControlScrollBarProc,
	k_move_horizontally | k_resize_vertically,
	""
};

resource 'CNTL' (r_CNTL_browser, purgeable) {
	{k_browser_top, k_browser_left, k_browser_bottom, k_browser_right},
	0,
	visible,
	0,
	0,
	kControlUserPaneProc,
	k_resize_horizontally | k_resize_vertically,
	""
};

resource 'CNTL' (r_CNTL_about_text, purgeable) {
	{k_about_text_top, k_about_text_left, k_about_text_bottom, k_about_text_right},
	0,
	visible,
	0,
	0,
	kControlStaticTextProc,
	k_resize_horizontally | k_resize_vertically,
	""
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

resource 'ics#' (r_icsx_back, purgeable) {
	{
		$"0000 0000 0000 0060 00E0 01C0 0380 0700"
		$"0700 0380 01C0 00E0 0060",
		$"0000 0000 0000 0060 00E0 01C0 0380 0700"
		$"0700 0380 01C0 00E0 0060"
	}
};

resource 'ics#' (r_icsx_forward, purgeable) {
	{
		$"0000 0000 0000 0600 0700 0380 01C0 00E0"
		$"00E0 01C0 0380 0700 06",
		$"0000 0000 0000 0600 0700 0380 01C0 00E0"
		$"00E0 01C0 0380 0700 06"
	}
};

resource 'ics#' (r_icsx_home, purgeable) {
	{
		$"0000 0000 0000 01B0 03F0 07F0 0E70 1C38"
		$"1818 0990 0990 0990 0FF0",
		$"0000 0000 0000 01B0 03F0 07F0 0E70 1C38"
		$"1818 0990 0990 0990 0FF0"
	}
};

resource 'ics#' (r_icsx_reload, purgeable) {
	{
		$"0000 0000 0000 03C8 0FF8 0C38 1878 1800"
		$"1800 1818 0C30 0FF0 03C0",
		$"0000 0000 0000 03C8 0FF8 0C38 1878 1800"
		$"1800 1818 0C30 0FF0 03C0"
	}
};

resource 'ics#' (r_icsx_stop, purgeable) {
	{
		$"0000 0000 0000 1818 1C38 0E70 07E0 03C0"
		$"03C0 07E0 0E70 1C38 1818",
		$"0000 0000 0000 1818 1C38 0E70 07E0 03C0"
		$"03C0 07E0 0E70 1C38 1818"
	}
};

resource 'MBAR' (r_MBAR) {
	{
		r_MENU_apple;
		r_MENU_file;
		r_MENU_edit;
		r_MENU_window
	}
};

resource 'MENU' (r_MENU_apple) {
	k_apple_menu_id,
	kMenuStdMenuProc,
	0b1111111111111111111111111111101,
	enabled,
	apple,
	{
		"About " k_app_name, noIcon, noKey, noMark, plain;
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (r_MENU_file) {
	k_file_menu_id,
	kMenuStdMenuProc,
	0b1111111111111111111111111111011,
	enabled,
	"File",
	{
		"New Window", noIcon, "N", noMark, plain;
		"Close Window", noIcon, "W", noMark, plain;
		"-", noIcon, noKey, noMark, plain;
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (r_MENU_edit) {
	k_edit_menu_id,
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

resource 'MENU' (r_MENU_window) {
	k_window_menu_id,
	kMenuStdMenuProc,
	0b1111111111111111111111111111111,
	enabled,
	"Window",
	{
		"Zoom", noIcon, "/", noMark, plain;
		" ", noIcon, "M", noMark, plain
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
		k_app_name " could not be opened because the Appearance Extension is not installed.",
		k_app_name " could not be opened because a required resource is missing.",
		"There is not enough memory to perform that operation.",
		"An unexpected error occurred."
	}
};

resource 'STR#' (r_STRx_menu_items, preload, purgeable) {
	{
		"Collapse",
		"Expand"
	}
};

resource 'STR#' (r_STRx_about, purgeable) {
	{
		k_app_name
	}
};

resource 'vers' (r_vers_program, purgeable) {
	k_app_version_major,
	(k_app_version_minor << 4) | k_app_version_patch,
	k_app_development_stage,
	k_app_prerelease_revision,
	verUS,
	k_app_version_string,
	k_app_version_string ", " k_app_copyright_string
};

resource 'vers' (r_vers_package, purgeable) {
	k_app_version_major,
	(k_app_version_minor << 4) | k_app_version_patch,
	k_app_development_stage,
	k_app_prerelease_revision,
	verUS,
	k_app_version_string,
	k_app_name " " k_app_version_string
};

resource 'WIND' (r_WIND_browser, purgeable) {
	{k_window_top, k_window_left, k_window_bottom, k_window_right},
	kWindowFullZoomGrowDocumentProc,
	invisible,
	goAway,
	0x0,
	"",
	noAutoCenter
};

resource 'WIND' (r_WIND_about, purgeable) {
	{k_about_window_top, k_about_window_left, k_about_window_bottom, k_about_window_right},
	kWindowDocumentProc,
	invisible,
	goAway,
	0x0,
	"About " k_app_name,
	centerMainScreen
};
