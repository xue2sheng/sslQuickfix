package main

// split session settings into four categories

import (
	"encoding/xml"
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

	for _, fixsession := range xmlsettings.Info.FixSessionInfo.FixSessions {
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

	return nil
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
	var fs quickfix.SessionSettings
	if len(fixsession.SocketConnectHost) > 0 {
		fs.Set(config.SocketConnectHost, fixsession.SocketConnectHost)
	}
	if len(fixsession.SocketConnectPort) > 0 {
		fs.Set(config.SocketConnectPort, fixsession.SocketConnectPort)
	}
	xmlsettings.Initiator.AddSession(&fs)
}

func (xmlsettings *XMLSettings) sslinitiator(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.SSLInitiator)
	fixsession.update(&xmlsettings.SSLInitiator)
	var fs quickfix.SessionSettings
	if len(fixsession.SocketConnectHost) > 0 {
		fs.Set(config.SocketConnectHost, fixsession.SocketConnectHost)
	}
	if len(fixsession.SocketConnectPort) > 0 {
		fs.Set(config.SocketConnectPort, fixsession.SocketConnectPort)
	}
	if len(fixsession.SocketUseSSL) > 0 {
		fs.Set(config.SocketUseSSL, fixsession.SocketUseSSL)
	}
	xmlsettings.SSLInitiator.AddSession(&fs)
}

func (xmlsettings *XMLSettings) acceptor(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.Acceptor)
	fixsession.update(&xmlsettings.Acceptor)
	var fs quickfix.SessionSettings
	// specific session seems not to work
	if len(fixsession.SocketAcceptPort) > 0 {
		fs.Set(config.SocketAcceptPort, fixsession.SocketAcceptPort)
	}
	// it seems to need to be defined at a global level. No good at all
	if len(fixsession.SocketAcceptPort) > 0 {
		xmlsettings.Acceptor.GlobalSettings().Set(config.SocketAcceptPort, fixsession.SocketAcceptPort)
	}
	xmlsettings.Acceptor.AddSession(&fs)
}

func (xmlsettings *XMLSettings) sslacceptor(fixsession *FixSession) {
	xmlsettings.Info.DefaultInfo.update(&xmlsettings.SSLAcceptor)
	fixsession.update(&xmlsettings.SSLAcceptor)
	var fs quickfix.SessionSettings
	// specific session seems not to work
	if len(fixsession.SocketAcceptPort) > 0 {
		fs.Set(config.SocketAcceptPort, fixsession.SocketAcceptPort)
	}
	// it seems to need to be defined at a global level. No good at all
	if len(fixsession.SocketAcceptPort) > 0 {
		xmlsettings.SSLAcceptor.GlobalSettings().Set(config.SocketAcceptPort, fixsession.SocketAcceptPort)
	}
	if len(fixsession.SocketUseSSL) > 0 {
		fs.Set(config.SocketUseSSL, fixsession.SocketUseSSL)
	}
	xmlsettings.SSLAcceptor.AddSession(&fs)
}

type Settings struct {
	XMLName        xml.Name    `xml:"settings"`
	DefaultInfo    Default     `xml:"default"`
	FixSessionInfo FixSessions `xml:"fixsessions"`
}

type Default struct {
	XMLName        xml.Name `xml:"default"`
	HeartBtInt     string   `xml:"HeartBtInt,attr"`
	FileLogPath    string   `xml:"FileLogPath,attr"`
	FileStorePath  string   `xml:"FileStorePath,attr"`
	StartTime      string   `xml:"StartTime,attr"`
	EndTime        string   `xml:"EndTime,attr"`
	DataDictionary string   `xml:"DataDictionary,attr"`
}

func (def *Default) update(qf *quickfix.Settings) {
	if len(def.HeartBtInt) > 0 {
		qf.GlobalSettings().Set(config.HeartBtInt, def.HeartBtInt)
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
}

type FixSessions struct {
	XMLName     xml.Name     `xml:"fixsessions"`
	FixSessions []FixSession `xml:"fixsession"`
}

type FixSession struct {
	XMLName                  xml.Name `xml:"fixsession"`
	TLS                      string   `xml:"TLS,attr"`
	ConnectionType           string   `xml:"ConnectionType,attr"`
	ResetOnLogon             string   `xml:"ResetOnLogon,attr"`
	SocketConnectHost        string   `xml:"SocketConnectHost,attr"`
	SocketConnectPort        string   `xml:"SocketConnectPort,attr"`
	SenderCompID             string   `xml:"SenderCompID,attr"`
	TargetCompID             string   `xml:"TargetCompID,attr"`
	BeginString              string   `xml:"BeginString,attr"`
	SocketInitiatorPort      string   `xml:"SocketInitiatorPort,attr"`
	SocketAcceptPort         string   `xml:"SocketAcceptPort,attr"`
	ServerCertificateFile    string   `xml:"ServerCertificateFile,attr"`
	ServerCertificateKeyFile string   `xml:"ServerCertificateKeyFile,attr"`
	SocketPrivateKeyFile     string   `xml:"SocketPrivateKeyFile,attr"`
	SocketCertificateFile    string   `xml:"SocketCertificateFile,attr"`
	SocketCAFile             string   `xml:"SocketCAFile"`
	SocketInsecureSkipVerify string   `xml:"SocketInsecureSkipVerify"`
	SocketServerName         string   `xml:"SocketServerName"`
	SocketMinimumTLSVersion  string   `xml:"SocketMinimumTLSVersion"`
	SocketUseSSL             string   `xml:"SocketUseSSL"`
}

func (fs *FixSession) update(qf *quickfix.Settings) {
	if len(fs.SocketConnectPort) > 0 {
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
	if len(fs.ResetOnLogon) > 0 {
		qf.GlobalSettings().Set(config.ResetOnLogon, fs.ResetOnLogon)
	}
}
