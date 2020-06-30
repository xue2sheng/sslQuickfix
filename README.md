# sslQuickfix

Proof of concept about SSL configuration for [quickfix](http://www.quickfixengine.org/) based projects. That library was cretated by [Oren Miller](oren@quickfixengine.org) and its [documentation](http://www.quickfixengine.org/quickfix/doc/html/) can be found at their side.

## Install quickfix library

In order to use that quickfix library on Archlinux boxes, a humble [yay](https://github.com/Jguer/yay) might do the trick for non-ssl projects:

yay -S quickfix

Take into account that openssl, boost and xml2 thirdparty libraries could be needed to be installed before builidng quickfix binaries from its [source code](http://prdownloads.sourceforge.net/quickfix/quickfix-1.15.1.tar.gz) at [GitHub](https://github.com/quickfix/quickfix):

./bootstrap
./configure --with-openssl --with-boost --enable-static --disable-shared --prefix=/opt/quickfix --exec-prefix=/opt/quickfix
make
make check # FieldMapTestCase may fail
sudo make install

## Check quickfix library

A simple project, inspired from [Mike Gatny's quickfix 101](https://github.com/mgatny/quickfix_101), should test that our previous statically built library support SSL commands
