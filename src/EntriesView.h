/*
 * Copyright 2021, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ENTRIESVIEW_H 
#define ENTRIESVIEW_H

#include <SupportDefs.h>
#include <GroupView.h>

class BBox;
class BButton;
class BFilePanel;
class BMenuField;
class BMessage;
class BPopUpMenu;
class BRadioButton;
class BStringView;
class BTextControl;


enum
{
	kEntryFolderText	= 'txef',
	kEntryFolderBrowse	= 'tbef',
	kOpenHtmlRadio		= 'rdow',
	kOpenUrlRadio		= 'roow',
	kOpenWithSelect		= 'mnow',
	kOpenWithPath		= 'pbow',
	kOpenWithBrowse		= 'tbow'
};


class EntriesView : public BGroupView {
public:
	EntriesView(const char* name);

	void MessageReceived(BMessage* msg);
	void AttachedToWindow();

private:
	void _InitInterface();
	void _PopulateOpenWithMenu();


	BBox*			fSavingBox;
	BStringView*	fEntryFolderLabel;
	BTextControl*	fEntryFolderText;
	BButton*		fEntryFolderBrowseButton;

	BBox*			fOpeningBox;
	BStringView*	fOpenAsLabel;
	BRadioButton*	fOpenAsHtmlRadio;
	BRadioButton*	fOpenAsUrlRadio;
	BStringView*	fOpenWithLabel;
	BPopUpMenu*		fOpenWithMenu;
	BMenuField*		fOpenWithMenuField;
	BButton*		fOpenWithSelectButton;
	BFilePanel*		fOpenWithPanel;
};


#endif // ENTRIESVIEW_H

