cd sim
qverilog -sv +define+stack_size=4 +incdir+.. ../*.v
