vlib work

vmap work work

# bin-lin file compilation
#decommenter la ligne de la description souhaitee
#vcom -reportprogress 300 -work work   ../src/bin_lin_2to4_logic.vhd
#vcom -reportprogress 300 -work work   ../src/bin_lin_2to4_tdv.vhd
vcom -reportprogress 300 -work work   ./src/axi4lite_slave.vhd

# test-bench compilation
vcom -reportprogress 300 -work work   ./tb/axi4lite_slave_tb.vhd

#Chargement fichier pour la simulation
vsim -novopt work.axi4lite_slave_tb

#ajout signaux composant simuler dans la fenetre wave
#add wave UUT/*

#ouvre le fichier format predefini
do wave.do
