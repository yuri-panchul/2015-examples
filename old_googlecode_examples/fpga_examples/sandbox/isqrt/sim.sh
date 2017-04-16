#!/bin/bash

rm -rf sim
mkdir sim
cd sim

ncverilog -Q +access+r +incdir+.. ../*.v &> z.ncv
vcs +v2k -R ../*.v &> z.vcs
qverilog ../*.v &> z.qv
