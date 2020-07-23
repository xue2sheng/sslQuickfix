package main

// split session settings into four categories

import (
	"encoding/xml"
	"errors"
	"fmt"
	"io/ioutil"
	"os"

	"github.com/quickfixgo/quickfix"
	"github.com/quickfixgo/quickfix/config"
)

type XMLSettings struct {
	Info         Settings
	Initiator    quickfix.Settings
	SSLInitiator quickfix.Settings
	Acceptor     quickfix.Settings
	SSLAcceptor  quickfix.Settings
}

func (xmlsettings *XMLSettings) LoadConfiguration(configFile string) error {

	xmlsettings.Init()

	cfg, err := os.Open(configFile)
	if err != nil {
		fmt.Printf("Error opening %v, %v\n", configFile, err)
		return err
	}
	defer cfg.Close()

	// read our opened xmlFile as a byte array.
	byteValue, err := ioutil.ReadAll(cfg)
	if err != nil {
		fmt.Printf("Error reading %v, %v\n", configFile, err)
		return err
	}

	xml.Unmarshal(byteValue, &xmlsettings.Info)

	for index, fixsession := range xmlsettings.Info.FixSessionInfo.FixSessions {
		fmt.Printf("FixSession %v TLS=%v connectionType=%v\n", index, fixsession.TLS, fixsession.ConnectionType)
		if fixsession.TLS == "true" {
			if fixsession.ConnectionType == "initiator" {
				xmlsettings.sslinitiator(&fixsession)
			} else if fixsession.ConnectionType == "acceptor" {
				xmlsettings.sslacceptor(&fixsession)
			}
		} else {
			if fixsession.ConnectionType == "initiator" {
				xmlsettings.initiator(&fixsession)
			} else if fixsession.ConnectionType == "acceptor" {
				xmlsettings.acceptor(&fixsession)
			}
		}
	}

	return errors.New("Did nothing")
}

func (xmlsettings *XMLSettings) Init() {
	xmlsettings.Initiator.Init()
	xmlsettings.SSLInitiator.Init()
	xmlsettings.Acceptor.Init()
	xmlsettings.SSLAcceptor.Init()
}

func (xmlsettings *XMLSettings) initiator(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.Initiator)
	fixsession.update(&xmlsettings.Initiator)
}

func (xmlsettings *XMLSettings) sslinitiator(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.SSLInitiator)
	fixsession.update(&xmlsettings.SSLInitiator)
}

func (xmlsettings *XMLSettings) acceptor(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.Acceptor)
	fixsession.update(&xmlsettings.Acceptor)
}

func (xmlsettings *XMLSettings) sslacceptor(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.SSLAcceptor)
	fixsession.update(&xmlsettings.SSLAcceptor)
}

type Settings struct {
	XMLName        xml.Name    `xml:"settings"`
	DefaultInfo    Default     `xml:"default"`
	FixSessionInfo FixSessions `xml:"fixsessions"`
}

type Default struct {
	XMLName                           xml.Name `xml:"default"`
	SocketConnectHost                 string   `xml:"SocketConnectHost,attr"`
	HeartBtInt                        string   `xml:"HeartBtInt,attr"`
	ResetOnLogon                      string   `xml:"ResetOnLogon,attr"`
	FileLogPath                       string   `xml:"FileLogPath,attr"`
	FileStorePath                     string   `xml:"FileStorePath,attr"`
	StartTime                         string   `xml:"StartTime,attr"`
	EndTime                           string   `xml:"EndTime,attr"`
	DataDictionary                    string   `xml:"DataDictionary,attr"`
	SocketPrivateKeyFile              string   `xml:"SocketPrivateKeyFile,attr"`
	SocketCertificateFile             string   `xml:"SocketCertificateFile,attr"`
	SSLProtocol                       string   `xml:"SSLProtocol,attr"`
	SSLCipherSuite                    string   `xml:"SSLCipherSuite,attr"`
	CertificationAuthoritiesFile      string   `xml:"CertificationAuthoritiesFile,attr"`
	CertificationAuthoritiesDirectory string   `xml:"CertificationAuthoritiesDirectory,attr"`
}

func (def *Default) update(qf *quickfix.Settings) {
	if len(def.SocketConnectHost) > 0 {
		qf.GlobalSettings().Set(config.SocketConnectHost, def.SocketConnectHost)
	}
	if len(def.HeartBtInt) > 0 {
		qf.GlobalSettings().Set(config.HeartBtInt, def.HeartBtInt)
	}
	if len(def.ResetOnLogon) > 0 {
		qf.GlobalSettings().Set(config.ResetOnLogon, def.ResetOnLogon)
	}
	if len(def.FileLogPath) > 0 {
		qf.GlobalSettings().Set(config.FileLogPath, def.FileLogPath)
	}
	if len(def.FileStorePath) > 0 {
		qf.GlobalSettings().Set(config.FileStorePath, def.FileStorePath)
	}
	if len(def.StartTime) > 0 {
		qf.GlobalSettings().Set(config.StartTime, def.StartTime)
	}
	if len(def.EndTime) > 0 {
		qf.GlobalSettings().Set(config.EndTime, def.EndTime)
	}
	if len(def.DataDictionary) > 0 {
		qf.GlobalSettings().Set(config.DataDictionary, def.DataDictionary)
	}
	if len(def.SocketPrivateKeyFile) > 0 {
		qf.GlobalSettings().Set(config.SocketPrivateKeyFile, def.SocketPrivateKeyFile)
	}
	if len(def.SocketCertificateFile) > 0 {
		qf.GlobalSettings().Set(config.SocketCertificateFile, def.SocketCertificateFile)
	}
}

type FixSessions struct {
	XMLName     xml.Name     `xml:"fixsessions"`
	FixSessions []FixSession `xml:"fixsession"`
}

type FixSession struct {
	XMLName                            xml.Name `xml:"fixsession"`
	TLS                                string   `xml:"TLS,attr"`
	ConnectionType                     string   `xml:"ConnectionType,attr"`
	SocketConnectHost                  string   `xml:"SocketConnectHost,attr"`
	SocketConnectPort                  string   `xml:"SocketConnectPort,attr"`
	SenderCompID                       string   `xml:"SenderCompID,attr"`
	TargetCompID                       string   `xml:"TargetCompID,attr"`
	BeginString                        string   `xml:"BeginString,attr"`
	SocketInitiatorPort                string   `xml:"SocketInitiatorPort,attr"`
	SocketAcceptorPort                 string   `xml:"SocketAcceptorPort,attr"`
	ClientCertificateKeyFile           string   `xml:"ClientCertificateKeyFile,attr"`
	ClientCertificateFile              string   `xml:"ClientCertificateFile,attr"`
	ServerCertificateFile              string   `xml:"ServerCertificateFile,attr"`
	ServerCertificateKeyFile           string   `xml:"ServerCertificateKeyFile,attr"`
	CertificateVerifyLevel             string   `xml:"CertificateVerifyLevel,attr"`
	CertificateRevocationListFile      string   `xml:"CertificateRevocationListFile,attr"`
	CertificateRevocationListDirectory string   `xml:"CertificateRevocationListDirectory,attr"`
}

func (fs *FixSession) update(qf *quickfix.Settings) {
	if len(fs.SocketConnectPort) > 0 {
		qf.GlobalSettings().Set(config.SocketConnectPort, fs.SocketConnectPort)
	}
	if len(fs.SenderCompID) > 0 {
		qf.GlobalSettings().Set(config.SenderCompID, fs.SenderCompID)
	}
	if len(fs.TargetCompID) > 0 {
		qf.GlobalSettings().Set(config.TargetCompID, fs.TargetCompID)
	}
	if len(fs.BeginString) > 0 {
		qf.GlobalSettings().Set(config.BeginString, fs.BeginString)
	}
}
