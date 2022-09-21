#include "Types.r"

#include "ResourceConstants.h"

#define kProjectName "hellolite"

resource 'ALRT' (rFatalErrorAlert, purgeable) {
	{84, 64, 198, 448},
	rFatalErrorAlert,
	{
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'DITL' (rFatalErrorAlert, purgeable) {
	{
		{84, 316, 104, 374},
		Button {
			enabled,
			"OK"
		},
		{10, 72, 74, 374},
		StaticText {
			disabled,
			"^0^1^2^3"
		}
	}
};

resource 'MBAR' (rMenuBar) {
	{
		mApple;
		mFile;
		mEdit
	}
};

resource 'MENU' (mApple) {
	mApple,
	kMenuStdMenuProc,
	0b1111111111111111111111111111101,
	enabled,
	apple,
	{
		"About " kProjectName "É", noIcon, noKey, noMark, plain;
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (mFile) {
	mFile,
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

resource 'MENU' (mEdit) {
	mEdit,
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

resource 'SIZE' (rSize, purgeable)  {
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

resource 'STR#' (rFatalErrorStrings, purgeable) {
	{
		"The application cannot start because the Appearance extension is not installed.",
		"The application cannot start because a required application resource is missing."
	}
};

resource 'WIND' (rWindow, purgeable) {
	{40, 40, 200, 200},
	kWindowFullZoomGrowDocumentProc,
	invisible,
	goAway,
	0x0,
	"",
	noAutoCenter
};
