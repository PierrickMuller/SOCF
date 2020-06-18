#add wave -position end  sim:/axi4lite_slave_tb/C_S_AXI_DATA_WIDTH
#add wave -position end  sim:/axi4lite_slave_tb/C_S_AXI_ADDR_WIDTH
add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ACLK
add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ARESET

add wave -group write_address sim:/axi4lite_slave_tb/S_AXI_AW*
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_AWADDR
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_AWVALID
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_AWREADY
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_AWPROT

add wave -group write_data sim:/axi4lite_slave_tb/S_AXI_W*
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_WDATA
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_WLAST
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_WSTRB
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_WVALID
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_WREADY

add wave -group write_response sim:/axi4lite_slave_tb/S_AXI_B*
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_BRESP
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_BVALID
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_BREADY

add wave -group read_address sim:/axi4lite_slave_tb/S_AXI_AR*
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ARADDR
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ARVALID
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ARREADY
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_ARPROT

add wave -group read_data sim:/axi4lite_slave_tb/S_AXI_R*
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_RDATA
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_RRESP
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_RREADY
# add wave -position end  sim:/axi4lite_slave_tb/S_AXI_RVALID


add wave -group register_values sim:/axi4lite_slave_tb/tst_reg*
# add wave -position end  sim:/axi4lite_slave_tb/tst_reg4_obs
# add wave -position end  sim:/axi4lite_slave_tb/tst_reg5_obs
# add wave -position end  sim:/axi4lite_slave_tb/tst_reg6_obs
# add wave -position end  sim:/axi4lite_slave_tb/tst_reg7_obs




#add wave -position end  sim:/axi4lite_slave_tb/sim_end_s
#add wave -position end  sim:/axi4lite_slave_tb/ClockCount
#add wave -position end  sim:/axi4lite_slave_tb/sendIt
#add wave -position end  sim:/axi4lite_slave_tb/readIt
#add wave -position end  sim:/axi4lite_slave_tb/ClockPeriod
#add wave -position end  sim:/axi4lite_slave_tb/ClockPeriod2
#add wave -position end  sim:/axi4lite_slave_tb/TAB_STI_AXI_WRITE
#add wave -position end  sim:/axi4lite_slave_tb/TAB_STI_AXI_READ
