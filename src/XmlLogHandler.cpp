/*
 *  Kaidan - A user-friendly XMPP client for every device!
 *
 *  Copyright (C) 2017-2018 Kaidan developers and contributors
 *  (see the LICENSE file for a full list of copyright authors)
 *
 *  Kaidan is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  In addition, as a special exception, the author of Kaidan gives
 *  permission to link the code of its release with the OpenSSL
 *  project's "OpenSSL" library (or with modified versions of it that
 *  use the same license as the "OpenSSL" library), and distribute the
 *  linked executables. You must obey the GNU General Public License in
 *  all respects for all of the code used other than "OpenSSL". If you
 *  modify this file, you may extend this exception to your version of
 *  the file, but you are not obligated to do so.  If you do not wish to
 *  do so, delete this exception statement from your version.
 *
 *  Kaidan is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kaidan.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "XmlLogHandler.h"

// gloox
#include <gloox/client.h>
// Qt
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

XmlLogHandler::XmlLogHandler(gloox::Client *client) : client(client)
{
	client->logInstance().registerLogHandler(
		gloox::LogLevelDebug,
		gloox::LogAreaXmlIncoming | gloox::LogAreaXmlOutgoing,
		this
	);
}

XmlLogHandler::~XmlLogHandler()
{
	client->logInstance().removeLogHandler(this);
}

void XmlLogHandler::handleLog(gloox::LogLevel level, gloox::LogArea area,
                              const std::string &message)
{
	if (area == gloox::LogAreaXmlIncoming)
		qDebug() << "[client] [incoming] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
	else if (area == gloox::LogAreaXmlOutgoing)
		qDebug() << "[client] [outgoing] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";

	qDebug().noquote() << makeXmlPretty(QString::fromStdString(message));
}

QString XmlLogHandler::makeXmlPretty(QString xmlIn)
{
	QString xmlOut;

	QXmlStreamReader reader(xmlIn);
	QXmlStreamWriter writer(&xmlOut);
	writer.setAutoFormatting(true);

	while (!reader.atEnd()) {
		reader.readNext();
		if (!reader.isWhitespace() && !reader.hasError()) {
			writer.writeCurrentToken(reader);
		}
	}

	// remove xml header
	xmlOut.replace("<?xml version=\"1.0\"?>", "");

	// remove first & last char (\n)
	// first char is needed due to header replacement
	xmlOut = xmlOut.right(xmlOut.size() - 1);
	xmlOut = xmlOut.left(xmlOut.size() - 1);

	return xmlOut;
}
