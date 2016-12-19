# CP_projectB

This project is written primarily in C, with analysis written in Python.

---Options for program---
main.o must be proceeded by 5 options in order for the code to run.
These are:
$1 - either 'edm' or 'cable' (without quotes) specify the model being simulated.
$2 - the scaling factor, typically 100 for edm or 1000 for cable.
$3 - tolerance for convergence, typically 1e-12.
$4 - dust, 1 to add dust, 0 for clean plates.
$5 - number of cores to be used.

NOTE: If you are running the code through the shell script, the last option is
calculated automatically and does not need to be specified.

Example:
./compile_run.sh edm 100 1e-12 0

---Running the code---

Linux:
   1. Unpack into directory
   2. run './compile_run.sh [options]'
