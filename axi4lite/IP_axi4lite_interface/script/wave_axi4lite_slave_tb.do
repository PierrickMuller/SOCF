onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /axi4lite_slave_tb/axi_instance/reset_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/clk_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/axi_awaddr_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_awprot_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_awvalid_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_awready_o
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/axi_wdata_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_wstrb_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_wvalid_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_wready_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_bresp_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_bvalid_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_bready_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/axi_araddr_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_arprot_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_arvalid_i
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_arready_o
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/axi_rdata_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_rresp_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_rvalid_o
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_rready_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/vect_input_A_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/vect_input_B_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/vect_input_C_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/vect_input_D_i
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/output_reg_A_o
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/output_reg_B_o
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/output_reg_C_o
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/output_reg_D_o
add wave -noupdate -divider {test reg internes}
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/internal_reg_1_s
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/internal_reg_2_s
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/internal_reg_3_s
add wave -noupdate -divider {Signaux internes}
add wave -noupdate /axi4lite_slave_tb/axi_instance/reset_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_awready_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_wready_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_bresp_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_waddr_done_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_bvalid_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_arready_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_rresp_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_raddr_done_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_rvalid_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_waddr_mem_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_data_wren_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_araddr_mem_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/axi_data_rden_s
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/axi_rdata_s
add wave -noupdate /axi4lite_slave_tb/axi_instance/dummy_cnt
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/byte_index
add wave -noupdate /axi4lite_slave_tb/axi_instance/local_addr_s
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/ADDR_LSB
add wave -noupdate -radix hexadecimal /axi4lite_slave_tb/axi_instance/CST_ADDR_0_FOR_TST
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {10 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 162
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {5 ns} {129 ns}
