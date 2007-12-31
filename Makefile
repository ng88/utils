
min: daemon max_uptime range rsh xoror btun

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

installnobtun:
	install -m 755 -d $(prefix)/usr/bin/
	install -m 755 -d $(prefix)/bin/
	install -m 755 -d $(prefix)/etc/
	install -sm 755 daemon/daemon max_uptime/max_uptime range/range xoror/xoror $(prefix)/usr/bin/
	install -sm 755 rsh/rsh $(prefix)/bin/
	install -m 644 rsh/rsh.conf $(prefix)/etc/

installbtun:
	install -m 755 -d $(prefix)/usr/sbin/
	install -m 755 -d $(prefix)/usr/bin/
	install -m 755 -d $(prefix)/etc/btund/
	install -sm 755 btun/btun $(prefix)/usr/bin/
	install -sm 755 btun/btund $(prefix)/usr/sbin/
	install -m 644 btun/config/users.sample $(prefix)/etc/btund/

install: installbtun installnobtun

package:
	./make_slack_package_btun.sh DISABLE_DEBUG_PRINT=1
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

mrproper: clean
	rm -f cpufreq/cpufreq2 daemon/daemon max_uptime/max_uptime range/range rsh/rsh scanp/scanp win2unix/win2unix xoror/xoror sudoku-solver/sudoku-solver btun/btund btun/btun

.PHONY: clean mrproper all min cpufreq daemon max_uptime range rsh scanp win2unix xoror sudoku-solver btun install package installbtun installnobtun
