rd /s /q sim
md sim
cd sim

vlib work
vlog +define+ISQRT=isqrt_comb_rigid -vlog01compat +incdir+.. ../*.v >> z
type z
rem vsim -c -do "run -all" isqrt_pipe_test
vsim -c -do "run -all" isqrt_test
