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

# Function to print command on terminal before executing it
#   This is preferable to set -x before it causes a double echo
#   of the same command when run with eval
evalx() { echo "Command: $@"; eval $@; }

# Read command-line arguments
testcase=$1
nodename=$2
meshlinkrootpath=$3

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
containername="${testcase}_${nodename}"


# Run Libtool Wrapper Scripts once in their built paths in order to generate lt-<program> script inside .libs directory
evalx "${blackboxpath}/${geninvitepgm} >/dev/null 2>/dev/null"
evalx "${testcasepath}/${nodesimpgm} >/dev/null 2>/dev/null"

# Create Meshlink Container Mirror Folder (Delete any existing folder before creating new folder)
evalx "rm -rf ${mirrorfolderpath} >/dev/null 2>/dev/null"
evalx "mkdir ${mirrorfolderpath}"

# Populate Mirror Folder
#   Copy Wrapper Scripts for Utility Programs
evalx "cp ${blackboxpath}/${geninvitepgm} ${mirrorfolderpath}"
evalx "cp ${testcasepath}/${nodesimpgm} ${mirrorfolderpath}"
#   Copy Utility Scripts
evalx "cp ${blackboxutilpath}/${nodestepscript} ${mirrorfolderpath}"
#    Set Script Permissions
evalx "chmod 755 ${mirrorfolderpath}/*"
#   Copy Binaries, lt- Scripts and Required Libraries
evalx "mkdir ${mirrorfolderlibpath}"
evalx "cp ${blackboxlibpath}/* ${mirrorfolderlibpath}"
evalx "cp ${testcaselibpath}/*${nodesimpgm}* ${mirrorfolderlibpath}"
evalx "cp ${meshlinksrclibpath}/* ${mirrorfolderlibpath}"
evalx "cp ${cattasrclibpath}/* ${mirrorfolderlibpath}"

# Copy mirror folder into LXC Container
#   Delete Destination Folder
evalx "${blackboxutilpath}/${lxcrunscript} \"rm -rf ${containerdstpath}\" ${containername}"
#   Create Destination Folder and Copy Files
evalx "${blackboxutilpath}/${lxccopydirscript} ${mirrorfolderpath} ${containername} ${containerdstpath}"
