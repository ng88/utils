min:
	$(MAKE) -C daemon
	$(MAKE) -C max_uptime
	$(MAKE) -C range
	$(MAKE) -C rsh
	$(MAKE) -C xoror


all: min
	$(MAKE) -C win2unix
	$(MAKE) -C cpufreq
	$(MAKE) -C scanp

clean:
	$(MAKE) -C cpufreq clean
	$(MAKE) -C daemon clean
	$(MAKE) -C max_uptime clean
	$(MAKE) -C range clean
	$(MAKE) -C rsh clean
	$(MAKE) -C scanp clean
	$(MAKE) -C win2unix clean
	$(MAKE) -C xoror clean