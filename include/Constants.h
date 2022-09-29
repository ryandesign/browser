enum
{
	kNoWindow,
	kDocumentWindow,
	kDAWindow
};

// If we want to support animations or a blinking insertion point this
// will need to be computed at runtime instead.
#define kSleepTime 0x7fffffff

#define kVisualDelay 8

#define kNoEvents 0

#define kMinDocWidth 128
#define kMaxDocWidth 0x7fff

#define kMinDocHeight kMinDocWidth
#define kMaxDocHeight kMaxDocWidth
