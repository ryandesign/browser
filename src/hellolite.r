// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "Types.r"

#include "ResourceConstants.h"

#define k_app_name "hellolite"

#define k_about_window_left 40
#define k_about_window_top 40
#define k_about_window_width 200
#define k_about_window_height 100
#define k_about_window_right (k_about_window_left + k_about_window_width)
#define k_about_window_bottom (k_about_window_top + k_about_window_height)

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

#define k_forward_button_left (k_back_button_right)
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

#define k_web_view_left 0
#define k_web_view_top (k_header_height)
#define k_web_view_right (k_window_width - 15)
#define k_web_view_bottom (k_window_height - 15)

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
	0,
	0,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};

resource 'CNTL' (r_CNTL_forward_button, purgeable) {
	{k_forward_button_top, k_forward_button_left, k_forward_button_bottom, k_forward_button_right},
	0,
	visible,
	0,
	0,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};

resource 'CNTL' (r_CNTL_home_button, purgeable) {
	{k_home_button_top, k_home_button_left, k_home_button_bottom, k_home_button_right},
	0,
	visible,
	0,
	0,
	kControlBevelButtonSmallBevelProc,
	0,
	""
};


resource 'CNTL' (r_CNTL_reload_button, purgeable) {
	{k_reload_button_top, k_reload_button_left, k_reload_button_bottom, k_reload_button_right},
	0,
	visible,
	0,
	0,
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

resource 'CNTL' (r_CNTL_web_view, purgeable) {
	{k_web_view_top, k_web_view_left, k_web_view_bottom, k_web_view_right},
	0,
	visible,
	0,
	0,
	kControlUserPaneProc,
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
