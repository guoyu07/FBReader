/*
 * Copyright (C) 2004-2012 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <stdlib.h>

#include <ZLApplication.h>
#include <ZLFile.h>
#include <ZLXMLReader.h>
#include <ZLResource.h>
#include <ZLibrary.h>
#include <ZLPopupData.h>
#include "ZLToolbar.h"
#include "ZLApplicationWindow.h"

#include <optionEntries/ZLStringInfoEntry.h>

class ZLToolbarCreator : public ZLXMLReader {

public:
	ZLToolbarCreator(ZLToolbar &toolbar);
	void startElementHandler(const char *tag, const char **attributes);

private:
	ZLToolbar &myToolbar;
};

void ZLApplication::createToolbar(int index) {
	toolbar(index);
	std::string fileName = ZLibrary::DefaultFilesPathPrefix();
	const bool isWindowToolbar = index == ZLApplicationWindow::WINDOW_TOOLBAR;
	fileName += isWindowToolbar ? "toolbar.xml" : "fullscreen_toolbar.xml";
	ZLToolbarCreator(isWindowToolbar ? *myToolbar : *myFullscreenToolbar).readDocument(ZLFile(fileName));
}

const ZLToolbar &ZLApplication::toolbar(int index) const {
	if (index == ZLApplicationWindow::WINDOW_TOOLBAR) {
		if (myToolbar.isNull()) {
			myToolbar = new ZLToolbar();
		}
		return *myToolbar;
	} else {
		if (myFullscreenToolbar.isNull()) {
			myFullscreenToolbar = new ZLToolbar();
		}
		return *myFullscreenToolbar;
	}
}

void ZLApplication::registerPopupData(const std::string &actionId, shared_ptr<ZLPopupData> popupData) {
	toolbar(ZLApplicationWindow::WINDOW_TOOLBAR);
	myToolbar->registerPopupData(actionId, popupData);
}

void ZLToolbar::registerPopupData(const std::string &actionId, shared_ptr<ZLPopupData> popupData) {
	myPopupDataMap[actionId] = popupData;
}

ZLToolbarCreator::ZLToolbarCreator(ZLToolbar &toolbar) : myToolbar(toolbar) {
}

void ZLToolbarCreator::startElementHandler(const char *tag, const char **attributes) {
	static const std::string BUTTON = "button";
	static const std::string MENU_BUTTON = "menuButton";
	static const std::string SEPARATOR = "separator";

	const char *id = attributeValue(attributes, "id");

	if (SEPARATOR == tag) {
		new ZLToolbar::SeparatorItem(myToolbar, ZLToolbar::Item::SEPARATOR);
	} else if (id == 0) {
		return;
	} else if (BUTTON == tag) {
		new ZLToolbar::PlainButtonItem(myToolbar, id);
	} else if (MENU_BUTTON == tag) {
		new ZLToolbar::MenuButtonItem(myToolbar, id);
	}
}

const ZLToolbar::ItemVector &ZLToolbar::items() const {
	return myItems;
}

const ZLResource &ZLToolbar::resource(const std::string &id) {
	return ZLResource::resource("toolbar")[id];
}

ZLToolbar::Item::Item(ZLToolbar &toolbar, Type type) : myToolbar(toolbar), myType(type) {
	toolbar.myItems.push_back(this);
}

ZLToolbar::Item::~Item() {
}

const ZLToolbar &ZLToolbar::Item::toolbar() const {
	return myToolbar;
}

ZLToolbar::Item::Type ZLToolbar::Item::type() const {
	return myType;
}

ZLToolbar::ActionItem::ActionItem(ZLToolbar &toolbar, Type type, const std::string &actionId) :
	Item(toolbar, type), myActionId(actionId), myResource(toolbar.resource(actionId)) {
}

const std::string &ZLToolbar::ActionItem::actionId() const {
	return myActionId;
}

const std::string &ZLToolbar::ActionItem::label() const {
	const ZLResource &labelResource = myResource["label"];
	if (!labelResource.hasValue()) {
		static const std::string EMPTY;
		return EMPTY;
	}
	return labelResource.value();
}

const std::string &ZLToolbar::ActionItem::tooltip() const {
	const ZLResource &tooltipResource = myResource["tooltip"];
	if (!tooltipResource.hasValue()) {
		static const std::string EMPTY;
		return EMPTY;
	}
	return tooltipResource.value();
}

ZLToolbar::AbstractButtonItem::AbstractButtonItem(ZLToolbar &toolbar, Type type, const std::string &actionId) : ActionItem(toolbar, type, actionId) {
}

ZLToolbar::PlainButtonItem::PlainButtonItem(ZLToolbar &toolbar, const std::string &actionId) :
	AbstractButtonItem(toolbar, PLAIN_BUTTON, actionId) {
}

ZLToolbar::MenuButtonItem::MenuButtonItem(ZLToolbar &toolbar, const std::string &actionId) :
	AbstractButtonItem(toolbar, MENU_BUTTON, actionId) {
}

const std::string &ZLToolbar::MenuButtonItem::popupTooltip() const {
	const ZLResource &popupResource = myResource["popup"];
	if (!popupResource.hasValue()) {
		static const std::string EMPTY;
		return EMPTY;
	}
	return popupResource.value();
}

shared_ptr<ZLPopupData> ZLToolbar::MenuButtonItem::popupData() const {
	std::map<std::string,shared_ptr<ZLPopupData> >::const_iterator it =
		toolbar().myPopupDataMap.find(actionId());
	return (it == toolbar().myPopupDataMap.end()) ? 0 : it->second;
}

const std::string &ZLToolbar::AbstractButtonItem::iconName() const {
	return actionId();
}

ZLToolbar::ZLToolbar() {
}

ZLToolbar::SeparatorItem::SeparatorItem(ZLToolbar &toolbar, Type type) : Item(toolbar, type) {
}

const std::string &ZLToolbar::SeparatorItem::actionId() const {
  static const std::string EMPTY;
	return EMPTY;
}

const ZLResource &ZLPopupData::resource(const std::string &actionId) {
	return ZLToolbar::resource(actionId);
}
