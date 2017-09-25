#!/bin/sh
meshlinkrootpath="/home/manavkumarm/meshlink"
lxcpath="/home/manavkumarm/.local/share/lxc"
lxcbridge="lxcbr0"

blackbox/run_blackbox_tests/run_blackbox_tests ${meshlinkrootpath} ${lxcpath} ${lxcbridge} 2> run_blackbox_test_cases.log
