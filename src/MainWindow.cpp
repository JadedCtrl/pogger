/*
 * Copyright 2020, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "MainWindow.h"

#include <iostream>

#include <Button.h>
#include <GroupView.h>
#include <LayoutBuilder.h>
#include <SeparatorView.h>
#include <StatusBar.h>
#include <String.h>
#include <TabView.h>

#include "App.h"
#include "EntriesView.h"
#include "FeedController.h"
#include "FeedsView.h"
#include "Notifier.h"
#include "UpdatesView.h"


MainWindow::MainWindow()
	:
	BWindow(((App*)be_app)->fPreferences->fMainWindowRect, "Pogger",
		B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	_InitInterface();
	MoveOnScreen();
}


MainWindow::~MainWindow()
{
	((App*)be_app)->fPreferences->fMainWindowRect = ConvertToScreen(Bounds());
	((App*)be_app)->fPreferences->fTabSelection = fTabView->Selection();
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case kFeedsAddButton:
		case kFeedsRemoveButton:
		case kFeedsEditButton:
		case kFeedsSelected:
		case kFeedsEdited:
		case kDownloadStart:
		case kDownloadComplete:
		case kDownloadFail:
		case kParseComplete:
		case kParseFail:
		{
			fFeedsView->MessageReceived(msg);
			break;
		}
		case kOpenWithSelect:
		{
			fEntriesView->MessageReceived(msg);
			break;
		}
		case kProgress:
		{
			fFeedsView->MessageReceived(msg);
			int32 total,current = 0;

			if (msg->FindInt32("total", &total) == B_OK
				&& msg->FindInt32("current", &current) == B_OK)
			{
				_UpdateProgress(total, current);
			}
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


void
MainWindow::_InitInterface()
{
	// Tabs
	fBaseView = new BGroupView("baseView");
	fTabView = new BTabView("tabView", B_WIDTH_FROM_WIDEST);
	fFeedsView = new FeedsView("Feeds");
	fEntriesView = new EntriesView("Entries");
	fUpdatesView = new UpdatesView("Updates");

	fTabView->AddTab(fFeedsView);
	fTabView->AddTab(fEntriesView);
	fTabView->AddTab(fUpdatesView);
	fTabView->SetBorder(B_NO_BORDER);
	fTabView->Select(((App*)be_app)->fPreferences->fTabSelection);
	fBaseView->AddChild(fTabView);

	// Bottom bar
	fStatusBar = new BStatusBar("feedProgress");

	fUpdateNowButton = new BButton("updateNow", "Update Now",
		new BMessage(kUpdateSubscribed));
	fUpdateNowButton->SetTarget((App*)be_app);
	fUpdateNowButton->SetExplicitAlignment(
		BAlignment(B_ALIGN_RIGHT, B_ALIGN_MIDDLE));


	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0, B_USE_DEFAULT_SPACING, 0, 0)
		.Add(fBaseView)
		.Add(new BSeparatorView(B_HORIZONTAL))
		.AddGroup(B_HORIZONTAL)
			.Add(fStatusBar)
			.Add(fUpdateNowButton)
			.SetInsets(B_USE_WINDOW_SPACING, B_USE_DEFAULT_SPACING,
				B_USE_DEFAULT_SPACING, B_USE_WINDOW_SPACING)
		.End()
	.End();
}


void
MainWindow::_UpdateProgress(int32 total, int32 current)
{
	if (current < fStatusBar->CurrentValue())
		fStatusBar->Reset();
	fStatusBar->SetMaxValue(total);
	fStatusBar->SetTo(current);
}


