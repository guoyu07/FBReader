/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __DOCBOOKDESCRIPTIONREADER_H__
#define __DOCBOOKDESCRIPTIONREADER_H__

#include <string>

#include "DocBookReader.h"
#include "../../description/BookDescription.h"

class DocBookDescriptionReader : public DocBookReader {

public:
	DocBookDescriptionReader(BookDescription &description);
	bool readDescription(ZLInputStream &stream);

	void startElementHandler(int tag, const char **attributes);
	void endElementHandler(int tag);
	void characterDataHandler(const char *text, int len);

private:
	WritableBookDescription myDescription;

	bool myReturnCode;

	bool myReadTitle;
	bool myReadAuthor;
	bool myReadAuthorName[3];

	std::string myAuthorNames[3];

	int myDepth;
};

#endif /* __DOCBOOKDESCRIPTIONREADER_H__ */
