#include "Types.r"

#include "ResourceConstants.h"

#define kProjectName "hellolite"

resource 'MBAR' (rMenuBar) {
	{
		mApple;
		mFile;
		mEdit
	}
};

resource 'MENU' (mApple) {
	mApple,
	textMenuProc,
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
	textMenuProc,
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
	textMenuProc,
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
	cannotBackground,
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

resource 'WIND' (rWindow) {
	{40, 40, 200, 200},
	zoomDocProc,
	invisible,
	goAway,
	0x0,
	"",
	noAutoCenter
};
