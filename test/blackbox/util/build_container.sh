#    build_container.sh -- Script to populate an LXC Container with the files
#		           required to run a Meshlink Node Simulation.
#			   Designed to run on unprivileged Containers.
#    Copyright (C) 2017  Guus Sliepen <guus@meshlink.io>
#                        Manav Kumar Mehta <manavkumarm@yahoo.com>
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Read command-line arguments
testcase=$1
nodename=$2
meshlinkrootpath=$3

# Set configuration for blackbox folders
blackboxrelpath="test/blackbox"
nodesimprefix="node_sim_"
geninvitepgm="gen_invite"
mirrorfolder="test"
ltlibfolder=".libs"
ltprefix="lt-"
meshsrcrelpath="src"
cattasrcrelpath="catta/src"
blackboxutilrelpath="test/blackbox/util"
containerdstpath="/home/ubuntu"
lxccopydirscript="lxc_copy_dir.sh"
lxcrunscript="lxc_run.sh"
nodestepscript="node_step.sh"

# Run Libtool Wrapper Scripts once in their built paths in order to generate lt-<program> script inside .libs directory
eval "${meshlinkrootpath}/${blackboxrelpath}/${geninvitepgm}"
eval "${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${nodesimprefix}${nodename}"

# Create Meshlink Container Mirror Folder (Delete any existing folder before creating new folder)
eval "rm -rf ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"
eval "mkdir ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"

# Populate Mirror Folder
#   Copy Wrapper Scripts for Utility Programs
eval "cp ${meshlinkrootpath}/${blackboxrelpath}/${geninvitepgm} ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"
eval "cp ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${nodesimprefix}${nodename} ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"
#   Copy Utility Scripts
echo "cp ${meshlinkrootpath}/${blackboxutilrelpath}/${nodestepscript} ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"
eval "cp ${meshlinkrootpath}/${blackboxutilrelpath}/${nodestepscript} ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}"
#   Copy Binaries, lt- Scripts and Required Libraries
eval "mkdir ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}/${ltlibfolder}"
eval "cp ${meshlinkrootpath}/${blackboxrelpath}/${ltlibfolder}/* ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}/${ltlibfolder}"
eval "cp ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${ltlibfolder}/*${nodesimprefix}${nodename}* ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}/${ltlibfolder}"
eval "cp ${meshlinkrootpath}/${meshsrcrelpath}/${ltlibfolder}/* ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}/${ltlibfolder}"
eval "cp ${meshlinkrootpath}/${cattasrcrelpath}/${ltlibfolder}/* ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder}/${ltlibfolder}"

# Copy mirror folder into LXC Container
#   Delete Destination Folder
eval "${meshlinkrootpath}/${blackboxutilrelpath}/${lxcrunscript} \"rm -rf ${containerdstpath}/${mirrorfolder}\" ${testcase}_${nodename}"
#eval "${meshlinkrootpath}/${blackboxutilrelpath}/${lxcrunscript} \"rm -rf ${containerdstpath}/${mirrorfolder}\" ${testcase}_${nodename}"
#   Create Destination Folder and Copy Files
eval "${meshlinkrootpath}/${blackboxutilrelpath}/${lxccopydirscript} ${meshlinkrootpath}/${blackboxrelpath}/${testcase}/${mirrorfolder} ${testcase}_${nodename} ${containerdstpath}/${mirrorfolder}"
