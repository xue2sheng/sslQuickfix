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

#ifdef _MSC_VER
#pragma warning(disable : 4503 4355 4786)
#else
#include "config.h"
#endif

#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/SSLSocketAcceptor.h"
#include "quickfix/SSLSocketInitiator.h"
#include "quickfix/ThreadedSSLSocketAcceptor.h"
#include "quickfix/ThreadedSSLSocketInitiator.h"
#endif
#include "Application.h"
#include "XMLSettings.h"
#include "getopt-repl.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Utility.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void logSettings(const FIX::SessionSettings& info, const std::string& label);
void launchInitiator(const FIX::SessionSettings& info, const std::string& label);
void launchAcceptor(const FIX::SessionSettings& info, const std::string& label);

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0]
                  << " configuration_file" << std::endl
                  << std::endl;
        return 0;
    }
    std::string file = argv[1];

    FIX::Initiator* initiator = 0;
    FIX::Acceptor* acceptor = 0;
    try {

        XMLSettings xmlSettings(file);

        std::cout << "SETTINGS:" << std::endl
                  << std::endl;
        logSettings(xmlSettings.Initiator, "Initiator");
        logSettings(xmlSettings.Acceptor, "Acceptor");

        Application application;
        FIX::FileStoreFactory storeFactoryInitiator(xmlSettings.Initiator);
        FIX::FileStoreFactory storeFactoryAcceptor(xmlSettings.Acceptor);
        FIX::ScreenLogFactory logFactoryInitiator(xmlSettings.Initiator);
        FIX::ScreenLogFactory logFactoryAcceptor(xmlSettings.Acceptor);

        /*
#ifdef HAVE_SSL
        if (isSSL.compare("SSL") == 0) {
            std::cout << std::endl
                      << "Multithreaded SSL" << std::endl
                      << std::endl;
            initiator = new FIX::ThreadedSSLSocketInitiator(application, storeFactory, xmlSettings.SSLInitiator, logFactory);
        } else
#endif
*/
        {
            std::cout << std::endl
                      << "No SSL" << std::endl
                      << std::endl;
            //initiator = new FIX::SocketInitiator(application, storeFactory, xmlSettings.SSLInitiator, logFactory);
            initiator = new FIX::SocketInitiator(application, storeFactoryInitiator, xmlSettings.Initiator, logFactoryInitiator);
            acceptor = new FIX::SocketAcceptor(application, storeFactoryAcceptor, xmlSettings.Acceptor, logFactoryAcceptor);
        }

        initiator->start();
        acceptor->start();

        FIX::process_sleep(3);
        if (xmlSettings.Initiator.size() > 0) {
            std::set<FIX::SessionID>::iterator firstOne = xmlSettings.Initiator.getSessions().begin();
            application.run(firstOne->getSenderCompID(), firstOne->getTargetCompID());
        }
        FIX::process_sleep(3);
        if (xmlSettings.Acceptor.size() > 0) {
            std::set<FIX::SessionID>::iterator firstOne = xmlSettings.Acceptor.getSessions().begin();
            application.run(firstOne->getSenderCompID(), firstOne->getTargetCompID());
        }
        FIX::process_sleep(3);

        initiator->stop();
        delete initiator;
        acceptor->stop();
        delete acceptor;

        return 0;

    } catch (std::exception& e) {
        std::cout << e.what();
        delete initiator;
        return 1;
    }
}

void logSettings(const FIX::SessionSettings& info, const std::string& label)
{
    std::cout << label << " -> " << info.size() << std::endl;
    if (info.size() > 0) {
        std::cout << info;
        if (info.get().has(FIX::CLIENT_CERT_FILE))
            std::cout << label << " got CLIENT_CERT_FILE" << std::endl;
        else
            std::cout << label << " got no CLIENT_CERT_FILE" << std::endl;
        if (info.get().has(FIX::CLIENT_CERT_KEY_FILE))
            std::cout << label << " got CLIENT_CERT_KEY_FILE" << std::endl;
        else
            std::cout << label << " got no CLIENT_CERT_KEY_FILE" << std::endl;
    }
    std::cout << std::endl;
}

void launchInitiator(const FIX::SessionSettings& info, const std::string& label)
{
}

void launchAcceptor(const FIX::SessionSettings& info, const std::string& label)
{
    return;
}
