# sslQuickfix

Proof of concept about SSL configuration for [quickfix](http://www.quickfixengine.org/) based projects. That library wwas cretated by [Oren Miller](oren@quickfixengine.org) and its [documentation](http://www.quickfixengine.org/quickfix/doc/html/) can be found at their side.

In order to use that quickfix library on Archlinux boxes, a humble [yay](https://github.com/Jguer/yay) might do the trick:

           yay -S quickfix

TAke into account that openssl, boost and xml2 thirdparty libraries could be needed to be installed before builidng quickfix binaries from its [source code](http://prdownloads.sourceforge.net/quickfix/quickfix-1.15.1.tar.gz) at [GitHub](https://github.com/quickfix/quickfix). 

