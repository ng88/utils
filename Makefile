
min: daemon max_uptime range rsh xoror btun tcpmux

all: min cpufreq scanp win2unix sudoku-solver

cpufreq:
	$(MAKE) -C $@

daemon:
	$(MAKE) -C $@

max_uptime:
	$(MAKE) -C $@

range:
	$(MAKE) -C $@

rsh:
	$(MAKE) -C $@

scanp:
	$(MAKE) -C $@

win2unix:
	$(MAKE) -C $@

xoror:
	$(MAKE) -C $@

sudoku-solver:
	$(MAKE) -C $@

btun:
	$(MAKE) -C $@

tcpmux: btun
	$(MAKE) -C tcp_multiplexer

installnobtun:
	chmod 755 $(prefix)/
	install -m 755 -d $(prefix)/usr/bin/
	install -m 755 -d $(prefix)/bin/
	install -m 755 -d $(prefix)/etc/
	install -sm 755 daemon/daemon max_uptime/max_uptime range/range xoror/xoror $(prefix)/usr/bin/
	install -sm 755 rsh/rsh $(prefix)/bin/
	install -m 644 rsh/rsh.conf $(prefix)/etc/

installbtun:
	chmod 755 $(prefix)/
	install -m 755 -d $(prefix)/usr/sbin/
	install -m 755 -d $(prefix)/usr/bin/
	install -m 755 -d $(prefix)/etc/btund/
	install -m 755 -d $(prefix)/usr/lib/btun/plugins/
	install -m 644 btun/plugins/libbtpcompress.so btun/plugins/libbtptee.so btun/plugins/libbtpxoror.so $(prefix)/usr/lib/btun/plugins/
	install -sm 755 btun/btun $(prefix)/usr/bin/
	install -sm 755 tcp_multiplexer/tcpmux $(prefix)/usr/bin/
	install -sm 755 btun/btund $(prefix)/usr/sbin/
	install -m 755 btun/scripts/btund_add_user.sh btun/scripts/create_passfile.sh $(prefix)/usr/sbin/
	install -m 644 btun/config/users.sample $(prefix)/etc/btund/

install: installbtun installnobtun

package:
	./make_slack_package_btun.sh
	./make_slack_package_utils.sh ENABLE_READLINE=1
clean:
	$(MAKE) -C cpufreq clean
	$(MAKE) -C daemon clean
	$(MAKE) -C max_uptime clean
	$(MAKE) -C range clean
	$(MAKE) -C rsh clean
	$(MAKE) -C scanp clean
	$(MAKE) -C win2unix clean
	$(MAKE) -C xoror clean
	$(MAKE) -C btun clean
	$(MAKE) -C sudoku-solver clean
	$(MAKE) -C tcp_multiplexer clean

mrproper: clean
	rm -f cpufreq/cpufreq2 daemon/daemon max_uptime/max_uptime range/range rsh/rsh scanp/scanp win2unix/win2unix xoror/xoror sudoku-solver/sudoku-solver btun/btund btun/btun tcp_multiplexer/tcpmux

.PHONY: clean mrproper all min cpufreq daemon max_uptime range rsh scanp win2unix xoror sudoku-solver btun install package installbtun installnobtun tcp_multiplexer
