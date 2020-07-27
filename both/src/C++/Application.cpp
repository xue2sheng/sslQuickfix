/* -*- C++ -*- */

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

#include "Application.h"
#include "quickfix/Session.h"
#include <cctype>
#include <iostream>
#include <sstream>

void Application::onLogon(const FIX::SessionID& sessionID)
{
    std::cout << std::endl
              << "Logon - " << sessionID << std::endl;
}

void Application::onLogout(const FIX::SessionID& sessionID)
{
    std::cout << std::endl
              << "Logout - " << sessionID << std::endl;
}

void Application::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
    crack(message, sessionID);

    std::cout << std::endl
              << "IN:" << std::endl;
    log(std::cout, message);
}

void Application::toApp(FIX::Message& message, const FIX::SessionID& sessionID)
    EXCEPT(FIX::DoNotSend)
{
    try {
        FIX::PossDupFlag possDupFlag;
        message.getHeader().getField(possDupFlag);
        if (possDupFlag)
            throw FIX::DoNotSend();
    } catch (FIX::FieldNotFound&) {
    }

    std::cout << std::endl
              << "OUT:" << std::endl;
    log(std::cout, message);
}

void Application::run(const std::string& senderCompID, const std::string& targetCompID)
{
    try {
        std::cout << std::endl
                  << "Sending default NewOrderSingle for FIX 4.2" << std::endl;
        FIX::Message order = launchDefaultNewOrderSingle42(senderCompID, targetCompID);
        FIX::Session::sendToTarget(order);
    } catch (std::exception& e) {
        std::cout << "Message Not Sent: " << e.what() << std::endl;
    }
}

FIX42::NewOrderSingle Application::launchDefaultNewOrderSingle42(const std::string& senderCompID, const std::string& targetCompID)
{
    FIX42::NewOrderSingle newOrderSingle(
        FIX::ClOrdID("12345"),
        FIX::HandlInst('1'),
        FIX::Symbol("EUR/USD"),
        FIX::Side(FIX::Side_BUY),
        FIX::TransactTime(),
        FIX::OrdType_LIMIT);

    newOrderSingle.set(FIX::OrderQty(1000));
    newOrderSingle.set(FIX::TimeInForce(FIX::TimeInForce_IMMEDIATE_OR_CANCEL));
    newOrderSingle.set(FIX::Price(1.3));

    newOrderSingle.getHeader().setField(FIX::SenderCompID(senderCompID));
    newOrderSingle.getHeader().setField(FIX::TargetCompID(targetCompID));

    return newOrderSingle;
}

void Application::log(std::ostream& out, const FIX::Message& message)
{
    std::stringstream ss;
    ss << message;
    char c;
    while (ss.get(c))
        out << (isprint(c) ? c : '\n');
    out << std::endl;
}
