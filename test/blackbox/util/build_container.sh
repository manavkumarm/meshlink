#!/bin/sh
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
setx=$4

# Set configuration for required folders, programs and scripts
#   Folder Paths
ltlibfolder=".libs"
meshlinksrclibpath="${meshlinkrootpath}/src/${ltlibfolder}"
cattasrclibpath="${meshlinkrootpath}/catta/src/${ltlibfolder}"
blackboxpath="${meshlinkrootpath}/test/blackbox"
blackboxlibpath="${meshlinkrootpath}/test/blackbox/${ltlibfolder}"
blackboxutilpath="${blackboxpath}/util"
testcasepath="${blackboxpath}/${testcase}"
testcaselibpath="${blackboxpath}/${testcase}/${ltlibfolder}"
mirrorfolder="test"
mirrorfolderpath="${testcasepath}/${mirrorfolder}"
mirrorfolderlibpath="${mirrorfolderpath}/${ltlibfolder}"
containerdstpath="/home/ubuntu/${mirrorfolder}"
#   Program/Script Names
ltprefix="lt-"
nodestepscript="node_step.sh"
nodesimpgm="node_sim_${nodename}"
nodesimltscript="${ltprefix}${nodesimpgm}"
geninvitepgm="gen_invite"
geninviteltscript="${ltprefix}${geninvitepgm}"
lxccopydirscript="lxc_copy_dir.sh"
lxcrunscript="lxc_run.sh"
#   Container Name
containername="run_${testcase}_${nodename}"

# Run Libtool Wrapper Scripts once in their built paths in order to generate lt-<program> script inside .libs directory
${blackboxpath}/${geninvitepgm} >/dev/null 2>/dev/null
${testcasepath}/${nodesimpgm} >/dev/null 2>/dev/null

set ${setx}

# Create Meshlink Container Mirror Folder (Delete any existing folder before creating new folder)
rm -rf ${mirrorfolderpath} >/dev/null 2>/dev/null
mkdir ${mirrorfolderpath}

# Populate Mirror Folder
#   Copy Wrapper Scripts for Utility Programs
cp ${blackboxpath}/${geninvitepgm} ${mirrorfolderpath}
cp ${testcasepath}/${nodesimpgm} ${mirrorfolderpath}
#   Copy Utility Scripts
cp ${blackboxutilpath}/${nodestepscript} ${mirrorfolderpath}
#    Set Script Permissions
chmod 755 ${mirrorfolderpath}/*
#   Copy Binaries, lt- Scripts and Required Libraries
mkdir ${mirrorfolderlibpath}
cp ${blackboxlibpath}/* ${mirrorfolderlibpath}
cp ${testcaselibpath}/*${nodesimpgm}* ${mirrorfolderlibpath}
cp ${meshlinksrclibpath}/* ${mirrorfolderlibpath}
cp ${cattasrclibpath}/* ${mirrorfolderlibpath}
#   Find liblxc shared library files and copy them
lxcso=`sudo find / -name "liblxc.so*"`
cp ${lxcso} ${mirrorfolderlibpath}

# Copy mirror folder into LXC Container
#   Delete Destination Folder
${blackboxutilpath}/${lxcrunscript} "rm -rf ${containerdstpath}" ${containername}
#   Create Destination Folder and Copy Files
${blackboxutilpath}/${lxccopydirscript} ${mirrorfolderpath} ${containername} ${containerdstpath}

set +x
