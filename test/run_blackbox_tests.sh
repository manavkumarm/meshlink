#!/bin/sh
meshlinkrootpath="/home/manavkumarm/meshlink"
lxcpath="/var/lib/lxc"
lxcbridge="lxcbr0"
ethifname="eth0"

blackbox/run_blackbox_tests/run_blackbox_tests ${meshlinkrootpath} ${lxcpath} ${lxcbridge} ${ethifname} 2> run_blackbox_test_cases.log
