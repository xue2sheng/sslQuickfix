# Check certicates 

Just look for some specific type of certificate and double check the simplest connection works with it. For example, this subsection will deal with:

	Expiration: 2 years (maximum)
	Auth Algo: RSA
	Key Length: 2048 (minimum), 4096 (preferred)
	Key Exchange: ECDHE
	TLS Level: 1.2
	Descriptor Cypher: TLS_ECDHE_RSA_AES_256_GCM_SHA384
	Mode: GCM
	Symmetric: AES
	Encryption Algo: 256-Bit
	Hash: SHA-384

## Double check your system support those certificates

The usual openssl command 

	openssl ciphers -v ALL | grep TLSv1.2 | grep -i ECDHE-RSA-AES256-GCM-SHA384

retuns the following result on my development environment:

	ECDHE-RSA-AES256-GCM-SHA384 TLSv1.2 Kx=ECDH     Au=RSA  Enc=AESGCM(256) Mac=AEAD

## Generate your own certificates

Have a look to [quickfix certificates](https://github.com/quickfix/quickfix/tree/master/bin/cfg/certs) and adapt them to your needs:

	openssl req -x509 -newkey rsa:4096 -sha256 -days 730 -nodes -keyout cakey.pem -out cacert.pem -subj '/CN=xue2sheng.com' -extensions san -config <(echo '[req]'; echo 'distinguished_name=req'; echo '[san]'; echo 'subjectAltName=DNS:*.xue2sheng.com,DNS:localhost')

Double check that subject alternatives names are the ones you're expected to use: 

	openssl x509 -text -in cacert.pem -noout

We're going to use the same certificates for the server and for the client side. The point is double check why only **default** certificate configurations seem to make it to **SSL Utility** classes. Therefore, this is not the definitive cert check.

## Expected outputs

Server -> executor in Go

	<2020-07-21 10:29:28.444950954 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
	  (8=FIX.4.29=7135=A34=149=INIT52=20200721-10:29:28.44456=ACCEPT98=0108=60141=Y10=016)
	<2020-07-21 10:29:28.444965866 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Received logon request)
	<2020-07-21 10:29:28.444993018 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Logon contains ResetSeqNumFlag=Y, resetting sequence numbers to 1)
	<2020-07-21 10:29:28.445017681 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Responding to logon request)
	<2020-07-21 10:29:28.445064836 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
	  (8=FIX.4.29=7135=A34=149=ACCEPT52=20200721-10:29:28.44556=INIT98=0108=60141=Y10=017)
	<2020-07-21 10:29:33.436248506 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
	  (8=FIX.4.29=12935=D34=249=INIT52=20200721-10:29:33.43556=ACCEPT11=1234521=138=100040=244=1.354=155=EUR/USD59=360=20200721-10:29:3310=077)
	<2020-07-21 10:29:33.436525853 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
	  (8=FIX.4.29=16135=834=249=ACCEPT52=20200721-10:29:33.43656=INIT6=1.3011=1234514=1000.0017=220=031=1.3032=1000.0037=238=1000.0039=254=155=EUR/USD150=2151=0.0010=127)
	<2020-07-21 10:29:33.437532003 +0000 UTC, FIX.4.2:ACCEPT->INIT, incoming>
	  (8=FIX.4.29=5335=534=349=INIT52=20200721-10:29:33.43756=ACCEPT10=187)
	<2020-07-21 10:29:33.437593164 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Received logout request)
	<2020-07-21 10:29:33.43762144 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Sending logout response)
	<2020-07-21 10:29:33.437710504 +0000 UTC, FIX.4.2:ACCEPT->INIT, outgoing>
	  (8=FIX.4.29=5335=534=349=ACCEPT52=20200721-10:29:33.43756=INIT10=187)
	<2020-07-21 10:29:33.437751227 +0000 UTC, FIX.4.2:ACCEPT->INIT, event>
	  (Disconnected)

Client -> tradeclient in C++

	SETTINGS:

	Initiator -> 0

	SSL Initiator -> 1
	[DEFAULT]
	CLIENTCERTIFICATEFILE=tls/cacert.pem
	CLIENTCERTIFICATEKEYFILE=tls/cakey.pem
	DATADICTIONARY=resources/FIX42.xml
	ENDTIME=00:00:00
	FILELOGPATH=logs
	FILESTOREPATH=logs
	HEARTBTINT=60
	RESETONLOGON=Y
	SOCKETCONNECTHOST=localhost
	SOCKETCONNECTPORT=9880
	STARTTIME=00:00:00

	[SESSION]
	BEGINSTRING=FIX.4.2
	CONNECTIONTYPE=initiator
	SENDERCOMPID=INIT
	TARGETCOMPID=ACCEPT

	SSL Initiator got CLIENT_CERT_FILE
	SSL Initiator got CLIENT_CERT_KEY_FILE

	Acceptor -> 0

	SSL Acceptor -> 0


	Multithreaded SSL

	<20200721-10:29:28.433985000, FIX.4.2:INIT->ACCEPT, event>
	  (Created session)
	<20200721-10:29:28.435057000, GLOBAL, event>
	  (Loading SSL certificate)
	<20200721-10:29:28.435110000, GLOBAL, event>
	  (Configuring RSA client certificate)
	<20200721-10:29:28.435166000, GLOBAL, event>
	  (Configuring RSA client private key)
	<20200721-10:29:28.435217000, GLOBAL, event>
	  (Loading CA info)
	<20200721-10:29:28.435348000, FIX.4.2:INIT->ACCEPT, event>
	  (Connecting to localhost on port 9880)
	<20200721-10:29:28.444516000, GLOBAL, event>
	  (Connection succeeded)
	<20200721-10:29:28.444813000, FIX.4.2:INIT->ACCEPT, outgoing>
	  (8=FIX.4.29=7135=A34=149=INIT52=20200721-10:29:28.44456=ACCEPT98=0108=60141=Y10=016)
	<20200721-10:29:28.444873000, FIX.4.2:INIT->ACCEPT, event>
	  (Initiated logon request)
	<20200721-10:29:28.445166000, FIX.4.2:INIT->ACCEPT, incoming>
	  (8=FIX.4.29=7135=A34=149=ACCEPT52=20200721-10:29:28.44556=INIT98=0108=60141=Y10=017)
	<20200721-10:29:28.445190000, FIX.4.2:INIT->ACCEPT, event>
	  (Logon contains ResetSeqNumFlag=Y, reseting sequence numbers to 1)
	<20200721-10:29:28.445195000, FIX.4.2:INIT->ACCEPT, event>
	  (Received logon response)

	Logon - FIX.4.2:INIT->ACCEPT

	Sending default NewOrderSingle for FIX 4.2

	OUT:
	8=FIX.4.2
	9=129
	35=D
	34=2
	49=INIT
	52=20200721-10:29:33.435
	56=ACCEPT
	11=12345
	21=1
	38=1000
	40=2
	44=1.3
	54=1
	55=EUR/USD
	59=3
	60=20200721-10:29:33
	10=077

	<20200721-10:29:33.435990000, FIX.4.2:INIT->ACCEPT, outgoing>
	  (8=FIX.4.29=12935=D34=249=INIT52=20200721-10:29:33.43556=ACCEPT11=1234521=138=100040=244=1.354=155=EUR/USD59=360=20200721-10:29:3310=077)
	<20200721-10:29:33.436712000, FIX.4.2:INIT->ACCEPT, incoming>
	  (8=FIX.4.29=16135=834=249=ACCEPT52=20200721-10:29:33.43656=INIT6=1.3011=1234514=1000.0017=220=031=1.3032=1000.0037=238=1000.0039=254=155=EUR/USD150=2151=0.0010=127)

	IN:
	8=FIX.4.2
	9=161
	35=8
	34=2
	49=ACCEPT
	52=20200721-10:29:33.436
	56=INIT
	6=1.30
	11=12345
	14=1000.00
	17=2
	20=0
	31=1.30
	32=1000.00
	37=2
	38=1000.00
	39=2
	54=1
	55=EUR/USD
	150=2
	151=0.00
	10=127

	<20200721-10:29:33.437204000, FIX.4.2:INIT->ACCEPT, event>
	  (Initiated logout request)
	<20200721-10:29:33.437340000, FIX.4.2:INIT->ACCEPT, outgoing>
	  (8=FIX.4.29=5335=534=349=INIT52=20200721-10:29:33.43756=ACCEPT10=187)
	<20200721-10:29:33.437928000, FIX.4.2:INIT->ACCEPT, incoming>
	  (8=FIX.4.29=5335=534=349=ACCEPT52=20200721-10:29:33.43756=INIT10=187)
	<20200721-10:29:33.438003000, FIX.4.2:INIT->ACCEPT, event>
	  (Received logout response)
	<20200721-10:29:33.438045000, FIX.4.2:INIT->ACCEPT, event>
	  (Disconnecting)

	Logout - FIX.4.2:INIT->ACCEPT 
