pkgname=cli-file-explorer
pkgver=1.0.0
pkgrel=1
pkgdesc="Cross-platform CLI File Explorer"
arch=('x86_64')
url="https://github.com/MengAiDev/cli-file-explorer"
license=('MIT')
depends=('gcc-libs' 'glibc')
makedepends=('cmake' 'gcc' 'make')
source=("${pkgname}-${pkgver}.tar.gz::https://github.com/MengAiDev/cli-file-explorer/archive/v${pkgver}.tar.gz")
sha256sums=('SKIP')

build() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  cmake -DCMAKE_INSTALL_PREFIX=/usr .
  make
}

check() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  ctest
}

package() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  make DESTDIR="${pkgdir}" install
}
