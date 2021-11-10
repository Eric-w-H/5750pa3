#!/bin/bash
#PBS -k eo
#PBS -m abe
#PBS -M ewh73@cornell.edu
#PBS -N PA3
getconf _NPROCESSORS_ONLN
getconf LEVEL1_DCACHE_LINESIZE
