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

#include "XMLSettings.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <string>
#include <vector>

bool XMLSettings::isSSLSupport() const
{
#ifdef HAVE_SSL
    return true;
#else
    return false;
#endif
}

// http://www.quickfixengine.org/quickfix/doc/html/configuration.html
XMLSettings::XMLSettings(const std::string& filename)
{
    boost::property_tree::ptree tree;
    read_xml(filename, tree);
    boost::optional<std::string> node;

    FIX::Dictionary defaults;

    // Common
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.SocketConnectHost"))
        defaults.setString(FIX::SOCKET_CONNECT_HOST, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.HeartBtInt"))
        defaults.setString(FIX::HEARTBTINT, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.StartTime"))
        defaults.setString(FIX::START_TIME, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.EndTime"))
        defaults.setString(FIX::END_TIME, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.ResetOnLogon"))
        defaults.setString(FIX::RESET_ON_LOGON, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.FileStorePath"))
        defaults.setString(FIX::FILE_STORE_PATH, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.FileLogPath"))
        defaults.setString(FIX::FILE_LOG_PATH, *node);
    if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.DataDictionary"))
        defaults.setString(FIX::DATA_DICTIONARY, *node);

    FIX::Dictionary sslDefaults;

    if (isSSLSupport()) {

        sslDefaults = defaults;

        // SSL
        if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.SSLProtocol"))
            sslDefaults.setString(FIX::SSL_PROTOCOL, *node);
        if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.SSLCipherSuite"))
            sslDefaults.setString(FIX::SSL_CIPHER_SUITE, *node);
        if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.CertificationAuthoritiesFile"))
            sslDefaults.setString(FIX::CERT_AUTH_FILE, *node);
        if (node = tree.get_optional<std::string>("settings.default.<xmlattr>.CertificationAuthoritiesDirectory"))
            sslDefaults.setString(FIX::CERT_AUTH_DIR, *node);
    }

    // FIX Sessions
    BOOST_FOREACH (boost::property_tree::ptree::value_type const& item, tree.get_child("settings.fixsessions")) {

        if (item.first == "fixsession") {

            FIX::SessionSettings* sessionCategory = NULL;
            FIX::Dictionary session;

            bool TLS = false;
            if (node = item.second.get_optional<std::string>("<xmlattr>.TLS"))
                TLS = isSSLSupport() && (*node == "true");

            boost::optional<std::string> connectionType;
            if (connectionType = item.second.get_optional<std::string>("<xmlattr>.ConnectionType")) {

                if (*connectionType == "initiator") {
                    if (TLS) {

                        sessionCategory = &SSLInitiator;
                        session = sslDefaults;
                        if (node = item.second.get_optional<std::string>("<xmlattr>.ClientCertificateFile"))
                            session.setString(FIX::CLIENT_CERT_FILE, *node);
                        if (node = item.second.get_optional<std::string>("<xmlattr>.ClientCertificateKeyFile"))
                            session.setString(FIX::CLIENT_CERT_KEY_FILE, *node);

                    } else {
                        sessionCategory = &Initiator;
                        session = defaults;
                    }

                } else if (*connectionType == "acceptor") {
                    if (TLS) {

                        sessionCategory = &SSLAcceptor;
                        session = sslDefaults;
                        if (node = item.second.get_optional<std::string>("<xmlattr>.ServerCertificateFile"))
                            session.setString(FIX::SERVER_CERT_FILE, *node);
                        if (node = item.second.get_optional<std::string>("<xmlattr>.ServerCertificateKeyFile"))
                            session.setString(FIX::SERVER_CERT_KEY_FILE, *node);
                        if (node = item.second.get_optional<std::string>("<xmlattr>.CertificateVerifyLevel"))
                            session.setString(FIX::VERIFY_LEVEL, *node);
                        if (node = item.second.get_optional<std::string>("<xmlattr>.CertificateRevocationListFile"))
                            session.setString(FIX::CRL_FILE, *node);
                        if (node = item.second.get_optional<std::string>("<xmlattr>.CertificateRevocationListDirectory"))
                            session.setString(FIX::CRL_DIR, *node);

                    } else {
                        sessionCategory = &Acceptor;
                        session = defaults;
                    }
                }

                if (sessionCategory) {

                    // Common
                    if (node = item.second.get_optional<std::string>("<xmlattr>.SocketConnectHost"))
                        session.setString(FIX::SOCKET_CONNECT_HOST, *node);
                    if (node = item.second.get_optional<std::string>("<xmlattr>.SocketConnectPort"))
                        session.setString(FIX::SOCKET_CONNECT_PORT, *node);

                    boost::optional<std::string> BeginString = item.second.get_optional<std::string>("<xmlattr>.BeginString");
                    boost::optional<std::string> SenderCompID = item.second.get_optional<std::string>("<xmlattr>.SenderCompID");
                    boost::optional<std::string> TargetCompID = item.second.get_optional<std::string>("<xmlattr>.TargetCompID");

                    sessionCategory->set(session); // that's wrong but supposedly needed
                    if (BeginString && SenderCompID && TargetCompID) {
                        session.setString(FIX::BEGINSTRING, *BeginString);
                        session.setString(FIX::SENDERCOMPID, *SenderCompID);
                        session.setString(FIX::TARGETCOMPID, *TargetCompID);
                        session.setString(FIX::CONNECTION_TYPE, *connectionType); // supposed only initiator or acceptor
                        sessionCategory->set(FIX::SessionID(*BeginString, *SenderCompID, *TargetCompID), session);
                    }
                }

            } // connectionType

        } // fixsession

    } // loop
}
