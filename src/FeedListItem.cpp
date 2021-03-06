/*
 * Copyright 2021, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "FeedListItem.h"

#include <Catalog.h>
#include <View.h>

#include "Feed.h"
#include "Util.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "FeedListItem"


FeedListItem::FeedListItem(Feed* feed)
	:
	BStringItem(feed->Title().String(), 0, false),
	fStatus(kClearStatus),
	fFeedUrl(feed->Url()),
	fFeedIdentifier(feed->Identifier()),
	fFeedSource(feed->Source())
{
	fFeed = feed;
	if (feed->Title().IsEmpty() == true)
		SetText(B_TRANSLATE("Untitled Feed"));
}


void
FeedListItem::DrawItem(BView* owner, BRect frame, bool complete)
{
	BStringItem::DrawItem(owner, frame, complete);

	font_height fontHeight;
	owner->GetFontHeight(&fontHeight);
	int16 imageHeight = int16(fontHeight.ascent + fontHeight.descent + 6);

	switch (fStatus) {
		case kDownloadingStatus:
		case kParsingStatus:
		{
			owner->MovePenTo(frame.right - 20, frame.top + BaselineOffset());
			owner->DrawString("…");
			break;
		}
		case kErrorStatus:
		{
			owner->MovePenTo(frame.right - 20, frame.top + BaselineOffset());
			owner->DrawString("X");
//			owner->MovePenTo(frame.right - imageHeight, frame.top);
//			owner->DrawBitmap(loadVectorIcon("error-status", imageHeight,
//				imageHeight));
			break;
		}
	}
}


const char*
FeedListItem::FeedIdentifier()
{
	return fFeedIdentifier.String();
}


const char*
FeedListItem::FeedSource()
{
	return fFeedSource.String();
}


BUrl
FeedListItem::FeedUrl()
{
	return fFeedUrl;
}


void
FeedListItem::SetStatus(int8 status)
{
	fStatus = status;
}


Feed*
FeedListItem::GetFeed()
{
	return fFeed;
}


