
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

install: min
	install -sm 755 daemon/daemon max_uptime/max_uptime range/range xoror/xoror btun/btun /usr/bin/
	install -sm 755 rsh/rsh /bin/
	install -sm 755 btun/btund /usr/sbin/
	install -m 644 rsh/rsh.conf /etc/
	install -m 644 -d /etc/btund/
	install -m 644 btun/config/users /etc/btund/

package:
	./make_slack_package_btun.sh DISABLE_DEBUG_PRINT=1
	./make_slack_package_main.sh ENABLE_READLINE=1
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

.PHONY: clean mrproper all min cpufreq daemon max_uptime range rsh scanp win2unix xoror sudoku-solver btun install package
