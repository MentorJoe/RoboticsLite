pkgname=roboticslite
pkgver=b0.2
pkgrel=1
pkgdesc="Local Multiplayer Arcade game"
arch=('x86_64')
depends=('sdl2' 'sdl2_image' 'sdl2_ttf')
sha1sums=('SKIP')
pkgdir=./rr_archpkg

build() {
	test -x ./roboticslite || make
}

package() {
	mkdir -p $(pkgdir)/opt/roboticslite/{ttf,imgs}
	mkdir -p $(pkgdir)/usr/local/bin
	mkdir -p $(pkgdir)/usr/local/share/applications
	install -m 555 roboticslite $(pkgdir)/opt/roboticslite/
	install -m 444 ttf/* $(pkgdir)/opt/roboticslite/ttf
	install -m 444 imgs/* $(pkgdir)/opt/roboticslite/imgs
	install -m 555 etc/launch.sh $(pkgdir)/usr/local/bin/roboticslite
	install -m 444 etc/roboticslite.desktop $(pkgdir)/usr/local/share/applications/roboticslite.desktop
}
