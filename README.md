# sslQuickfix

Proof of concept about SSL configuration for [quickfix](http://www.quickfixengine.org/) based projects. That library was cretated by [Oren Miller](oren@quickfixengine.org) and its [documentation](http://www.quickfixengine.org/quickfix/doc/html/) can be found at their side.

## Install quickfix library

In order to use that quickfix library on Archlinux boxes, a humble [yay](https://github.com/Jguer/yay) might do the trick for non-ssl projects:

      yay -S quickfix

Take into account that openssl, boost and xml2 thirdparty libraries could be needed to be installed before builidng quickfix binaries from its [source code](http://prdownloads.sourceforge.net/quickfix/quickfix-1.15.1.tar.gz) at [GitHub](https://github.com/quickfix/quickfix):

      ./bootstrap
      ./configure --with-openssl --enable-static --prefix=/opt/quickfix --exec-prefix=/opt/quickfix
      make
      make check # FieldMapTestCase may fail
      sudo make install

Library dependencies should looks like:

       ArchLinux /opt/quickfix/lib $ ldd libquickfix.so
       linux-vdso.so.1 (0x00007fff87500000)
       libpthread.so.0 => /usr/lib/libpthread.so.0 (0x00007f7e03b04000)
       libssl.so.1.1 => /usr/lib/libssl.so.1.1 (0x00007f7e03a74000)
       libcrypto.so.1.1 => /usr/lib/libcrypto.so.1.1 (0x00007f7e03797000)
       libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007f7e035ba000)
       libm.so.6 => /usr/lib/libm.so.6 (0x00007f7e03475000)
       libc.so.6 => /usr/lib/libc.so.6 (0x00007f7e032ae000)
       libgcc_s.so.1 => /usr/lib/libgcc_s.so.1 (0x00007f7e03292000)
       /usr/lib64/ld-linux-x86-64.so.2 (0x00007f7e03cc2000)
       libdl.so.2 => /usr/lib/libdl.so.2 (0x00007f7e0328c000)
 

