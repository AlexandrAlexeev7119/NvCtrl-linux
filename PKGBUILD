pkgname="gwepp"
pkgver="0.9.0"
pkgrel=1
pkgdesc="GreenWithEnvy re-implementation using C, C++ and Qt only"
arch=("x86_64")
url="https://codeberg.org/AlexCr4ckPentest/GWEpp"
source=("git+https://codeberg.org/AlexCr4ckPentest/GWEpp.git")
md5sums=("SKIP")
license=("MIT")
depends=("nvidia" "cmake" "qt5-base")

prepare() {
    cd $srcdir/GWEpp
    git submodule update --init --recursive
}


build() {
    cmake -DCMAKE_BUILD_TYPE=Release \
        -B build \
        -S $srcdir/GWEpp
    make -C build
}

package() {
    #
    # /usr/share/gwepp/gwepp.json - config file
    # /usr/bin/gwepp - executable file
    #

    mkdir -p $pkgdir/usr/bin/
    mkdir -p $pkgdir/usr/share/

    cp $srcdir/build/gwepp $pkgdir/usr/bin/.

    if [ -d /usr/share/gwepp ] && [ -f /usr/share/gwepp/gwepp.json ]; then
        cp -r /usr/share/gwepp $pkgdir/usr/share/.
    else
        cp -r $srcdir/GWEpp/gwepp $pkgdir/usr/share/.
        chmod 755 $pkgdir/usr/share/gwepp
        chmod 666 $pkgdir/usr/share/gwepp/gwepp.json
    fi
}
