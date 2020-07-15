# Install quickfix library

In order to use that [quickfix library](quickfix-1.15.1.tar.gz) on Archlinux boxes, a humble [yay](https://github.com/Jguer/yay) might do the trick for non-ssl projects:

      yay -S quickfix

Take into account that openssl, boost and xml2 thirdparty libraries could be needed to be installed before builidng quickfix binaries from its [source code](http://prdownloads.sourceforge.net/quickfix/quickfix-1.15.1.tar.gz) at [GitHub](https://github.com/quickfix/quickfix):

     ./bootstrap
     ./configure --with-openssl --with-boost --enable-static --disable-shared --prefix=/opt/quickfix --exec-prefix=/opt/quickfix
     CXXFLAGS="-O2 -DNDEBUG -fPIC" CFLAGS="-O2 -DNDEBUG -fPIC" make -j 4
     make check # FieldMapTestCase may fail
     sudo make install

