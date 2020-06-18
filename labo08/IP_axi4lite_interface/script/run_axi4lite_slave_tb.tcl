###########################################################################
# HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
# Institut REDS, Reconfigurable & Embedded Digital Systems
#
# Fichier      : axi4lite_slave_tb.tcl
# Description  : Script de compilation fichiers et lancement tb
#
# Auteur       : Etienne Messerli
# Date         : 13.08.2017
# Version      : 0.0
#
# Utilise      : Compilation/simulation axi4lite_slave_tb.vhd
#
#--| Modifications |--------------------------------------------------------
# Ver  Aut.  Date   Description
#                         
############################################################################

#this file need to be run into directory ./comp/

#create library work        
vlib work
#map library work to work
vmap work work

#compile files
vcom -reportprogress 300 -work work   ../src/axi4lite_slave.vhd

#compile files for simulation
vcom -reportprogress 300 -work work   ../src_tb/axi4lite_slave_tb.vhd


#Chargement fichier pour la simulation
vsim -novopt work.axi4lite_slave_tb 

#ajout signaux composant simuler dans la fenetre wave
#do ../wave_axi_dummy_slv_tb.do
do ./wave_axi4lite_slave_tb.do
# Run simulation



