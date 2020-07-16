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
