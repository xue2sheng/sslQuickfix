/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef FIX_XMLSETTINGS_H
#define FIX_XMLSETTINGSR_H

#ifdef _MSC_VER
#pragma warning(disable : 4503 4355 4786)
#else
#include "config.h"
#endif

#include "quickfix/SessionSettings.h"
#include <string>
#include <vector>

// vanilla FIX::Settings configuration file doesn't allow to define SocketAcceptorPort at SESSION level
struct XMLSettings {
    XMLSettings(const std::string& filename);
    bool isSSLSupport() const;
    FIX::SessionSettings SSLInitiator;
    FIX::SessionSettings Initiator;
    FIX::SessionSettings SSLAcceptor;
    FIX::SessionSettings Acceptor;
};

#endif // FIX_XMLSETTINGSR_H
