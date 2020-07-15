## Check quickfix library

A [simplest project](simplest/README.md), inspired from [Mike Gatny's quickfix 101](https://github.com/mgatny/quickfix_101), should test that our previous quickfix installation.

More effort on the C++ "client" than on the Go "server". In fact, the reason to choose Go as language for the server mocks was to save time and simplify deployment on testing environments.

Extending [C++ tradeclient]() and [executor Go](https://github.com/quickfixgo/examples) examples, mainly its internal configuration, is the fastest way to double check your system. All credit to [Chris Busbey](https://github.com/cbusbey), [Michael Ackley](https://github.com/ackleymi) and all the contributors at [quickfix repository](https://github.com/quickfix/quickfix).

Take into account that C++ localhost SSL certificates are used. No good enough to consider this proof of concept done.

## Server

Running in **acceptor** mode, just wait for client messages. You should stop it once your test is over.

./executorgo executorgo_both.xml

Its output, once the client message is received, should seem as:

    <2020-07-15 13:58:10.044963851 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Created session)
    <2020-07-15 13:58:29.050561125 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
(8=FIX.4.29=7135=A34=149=INIT52=20200715-13:58:29.05056=ACCEPT98=0108=60141=Y10=018)
    <2020-07-15 13:58:29.050593516 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Received logon request)
    <2020-07-15 13:58:29.050625247 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Logon contains ResetSeqNumFlag=Y, resetting sequence numbers to 1)
    <2020-07-15 13:58:29.05063237 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Responding to logon request)
    <2020-07-15 13:58:29.050651172 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
(8=FIX.4.29=7135=A34=149=ACCEPT52=20200715-13:58:29.05056=INIT98=0108=60141=Y10=018)
    <2020-07-15 13:58:34.041968765 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
(8=FIX.4.29=12935=D34=249=INIT52=20200715-13:58:34.04156=ACCEPT11=1234521=138=100040=244=1.354=155=EUR/USD59=360=20200715-13:58:3410=088)
    <2020-07-15 13:58:34.042293982 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
(8=FIX.4.29=16135=834=249=ACCEPT52=20200715-13:58:34.04256=INIT6=1.3011=1234514=1000.0017=120=031=1.3032=1000.0037=138=1000.0039=254=155=EUR/USD150=2151=0.0010=127)
    <2020-07-15 13:58:34.048012636 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
(8=FIX.4.29=5335=534=349=INIT52=20200715-13:58:34.04256=ACCEPT10=188)
    <2020-07-15 13:58:34.04804728 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Received logout request)
    <2020-07-15 13:58:34.048056933 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Sending logout response)
    <2020-07-15 13:58:34.048087651 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
(8=FIX.4.29=5335=534=349=ACCEPT52=20200715-13:58:34.04856=INIT10=194)
    <2020-07-15 13:58:34.048103062 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
(Disconnected)

## Client

    Similarly, the client sent, by default, a simple message and quit. Take into account that it contains a time delay internally as a cheap way to wait for TLS handshanking.

    ./tradeclient both.xml

    Expected output:

    initiator -> 0
    SSL initiator -> 1
    acceptor -> 0
    SSL acceptor -> 0
    Multithreaded SSL
    <20200715-13:58:29.040308000, FIX.4.2:INIT->ACCEPT, event>
(Created session)
    <20200715-13:58:29.041304000, GLOBAL, event>
(Loading SSL certificate)
    <20200715-13:58:29.041310000, GLOBAL, event>
(No SSL certificate configured for client.)
    <20200715-13:58:29.041353000, GLOBAL, event>
(Loading CA info)
    <20200715-13:58:29.041438000, FIX.4.2:INIT->ACCEPT, event>
(Connecting to localhost on port 9880)
    <20200715-13:58:29.050211000, GLOBAL, event>
(Connection succeeded)
    <20200715-13:58:29.050448000, FIX.4.2:INIT->ACCEPT, outgoing>
(8=FIX.4.29=7135=A34=149=INIT52=20200715-13:58:29.05056=ACCEPT98=0108=60141=Y10=018)
    <20200715-13:58:29.050509000, FIX.4.2:INIT->ACCEPT, event>
(Initiated logon request)
    <20200715-13:58:29.050744000, FIX.4.2:INIT->ACCEPT, incoming>
(8=FIX.4.29=7135=A34=149=ACCEPT52=20200715-13:58:29.05056=INIT98=0108=60141=Y10=018)
    <20200715-13:58:29.050780000, FIX.4.2:INIT->ACCEPT, event>
(Logon contains ResetSeqNumFlag=Y, reseting sequence numbers to 1)
    <20200715-13:58:29.050788000, FIX.4.2:INIT->ACCEPT, event>
(Received logon response)
    Logon - FIX.4.2:INIT->ACCEPT
    Sending default NewOrderSingle for FIX 4.2
    OUT: 8=FIX.4.29=12935=D34=249=INIT52=20200715-13:58:34.04156=ACCEPT11=1234521=138=100040=244=1.354=155=EUR/USD59=360=20200715-13:58:3410=088
    <20200715-13:58:34.041760000, FIX.4.2:INIT->ACCEPT, outgoing>
(8=FIX.4.29=12935=D34=249=INIT52=20200715-13:58:34.04156=ACCEPT11=1234521=138=100040=244=1.354=155=EUR/USD59=360=20200715-13:58:3410=088)
    <20200715-13:58:34.042434000, FIX.4.2:INIT->ACCEPT, incoming>
(8=FIX.4.29=16135=834=249=ACCEPT52=20200715-13:58:34.04256=INIT6=1.3011=1234514=1000.0017=120=031=1.3032=1000.0037=138=1000.0039=254=155=EUR/USD150=2151=0.0010=127)
    IN: 8=FIX.4.29=16135=834=249=ACCEPT52=20200715-13:58:34.04256=INIT6=1.3011=1234514=1000.0017=120=031=1.3032=1000.0037=138=1000.0039=254=155=EUR/USD150=2151=0.0010=127
    <20200715-13:58:34.042548000, FIX.4.2:INIT->ACCEPT, event>
(Initiated logout request)
    <20200715-13:58:34.047838000, FIX.4.2:INIT->ACCEPT, outgoing>
(8=FIX.4.29=5335=534=349=INIT52=20200715-13:58:34.04256=ACCEPT10=188)
    <20200715-13:58:34.048231000, FIX.4.2:INIT->ACCEPT, incoming>
(8=FIX.4.29=5335=534=349=ACCEPT52=20200715-13:58:34.04856=INIT10=194)
    <20200715-13:58:34.048278000, FIX.4.2:INIT->ACCEPT, event>
(Received logout response)
    <20200715-13:58:34.048307000, FIX.4.2:INIT->ACCEPT, event>
(Disconnecting)
    Logout - FIX.4.2:INIT->ACCEPT

