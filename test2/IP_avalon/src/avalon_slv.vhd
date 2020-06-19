-----------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- Fichier      : avalon_slv
-- Description  : Interface avalon slave
--
-- Auteur       : E. Messerli / S. Masle
-- Date         : 17.06.2020
--
-- Utilise      : IP Avalon pour test SoCF du 19 juin 2020
--
--| Modifications |-----------------------------------------------------------
-- Ver   Auteur Date         Description
--
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity avalon_slv is
  generic (
    -- Users to add parameters here

    -- User parameters ends

    -- Width of Avalon address bus
    AVALON_ADDR_WIDTH  : integer   := 8
  );
  port (
    clk_i                : in  std_logic;
    reset_i              : in  std_logic;
    avalon_slv_add_i     : in  std_logic_vector(AVALON_ADDR_WIDTH-1 downto 0);
    avalon_slv_read_i    : in  std_logic;
    avalon_slv_rvalide_o : out std_logic;
    avalon_slv_rdata_o   : out std_logic_vector(31 downto 0);
    avalon_slv_write_i   : in  std_logic;
    avalon_slv_wdata_i   : in  std_logic_vector(31 downto 0);
    avalon_slv_waitreq_o : out std_logic;
    
    -- add user signals here
    hex03_o      : out std_logic_vector(31 downto 0);
    hex54_o      : out std_logic_vector(31 downto 0);
    
    
    -- interrupt
    irq_o                : out std_logic
    
  );
end avalon_slv;

architecture comport of avalon_slv is

  -- With data 32 bits, the 2 low address bits are not used for
  -- the address decoder
  constant ADDR_LSB   : integer    := 2;


  --internal signals declarations
  constant CST_ADDR_0_FOR_TST  : std_logic_vector(31 downto 0) := x"CAFE1234";
  signal tst_reg_interne_s     : std_logic_vector(31 downto 0);  --registre de test rd/wr
  signal reg_status_s          : std_logic_vector( 0 downto 0);  --bit[0]: interrupt capture, en rd bit[16]: busy_s
  signal reg_control_s         : std_logic_vector( 7 downto 0);  --bit[0]: com_start, bit[1]: direction, bits[7..4]: step
  signal reg_val_init_s        : std_logic_vector(29 downto 0);  --bits[29..0]: val init cpt
  signal reg_cpt_s             : std_logic_vector(29 downto 0);  --bits[29..0]: etat cpt
  signal busy_s                : std_logic;
  signal busy_prev_s           : std_logic;
  signal enable_s              : std_logic;
  signal com_start_s           : std_logic;
  signal cpt_complete_s        : std_logic;

  signal readdata_next  : std_logic_vector(7 downto 0);
  signal writedata      : std_logic_vector(7 downto 0);
  
  signal addr_interne_s : std_logic_vector(AVALON_ADDR_WIDTH-1 downto ADDR_LSB);

  --add user declarations here 
  signal hex03_register_s      : std_logic_vector(31 downto 0);
  signal hex54_register_s      : std_logic_vector(31 downto 0);
 
 
  --La taille du compteur est de 30 bits
  --  permet de realiser des delais jusqu'a t = 21,47 sec
  --  (comptage de 2**30= 1'073M periodes avec horloge a 50Mhz) 
  
  component cpt_socf
   port(reset_i        : in   std_logic;
        clk_i          : in   std_logic;
        enable_i       : in   std_logic;
        start_i        : in   std_logic;
        step_i         : in   std_logic_vector( 3 downto 0);   -- pas du compteur
        val_init_i     : in   std_logic_vector( 29 downto 0);  -- valeur d'initialisation
        cpt_o          : out  std_logic_vector( 29 downto 0);  -- etat du compteur
        busy_o         : out  std_logic                        -- comptage en cours
   );
  end component;
  for all: cpt_socf use entity work.cpt_socf;


begin
    --wait not used
    avalon_slv_waitreq_o <= '0';
   
    -----------------------------------------------------------------------------
    --process for read transfert
    -----------------------------------------------------------------------------
    process(clk_i, reset_i)
        variable addr_interne_rd_v  : std_logic_vector(AVALON_ADDR_WIDTH-1 downto ADDR_LSB);
        variable loc_addr_rd_v  : natural;
    begin
        if reset_i = '1' then
            avalon_slv_rdata_o   <= (others => '0');
            avalon_slv_rvalide_o <= '0';
        elsif rising_edge(clk_i) then
            if avalon_slv_read_i = '1' then
                -- Data with is 32 bits, the 2 low address bits are not used
                --loc_addr_rd_v  := to_integer(unsigned(avalon_slv_add_i(AVALON_ADDR_WIDTH-1 downto ADDR_LSB)));   -- EMI 17 avril 2018
                addr_interne_rd_v  := avalon_slv_add_i(AVALON_ADDR_WIDTH-1 downto ADDR_LSB);                         -- EMI 17 avril 2018
                loc_addr_rd_v  := to_integer(unsigned(addr_interne_rd_v));                                            -- EMI 17 avril 2018
                avalon_slv_rdata_o <= x"A5A5A5A5"; --default value
                avalon_slv_rvalide_o <= '1';
                case loc_addr_rd_v is
                    --user need to complet action for each address
                    when 0 =>
                        avalon_slv_rdata_o <= CST_ADDR_0_FOR_TST;
                    when 1 =>
                        avalon_slv_rdata_o <= tst_reg_interne_s;
                    when 2 =>
                        avalon_slv_rdata_o <= (others => '0'); --'0' for others bit
                        avalon_slv_rdata_o(16) <= busy_s;                                   --bit[16]: busy_s
                        avalon_slv_rdata_o(reg_status_s'high downto 0) <= reg_status_s;     --bit[0]:  interrupt capture
                    when 3 =>
                        avalon_slv_rdata_o <= (others => '0'); --'0' for others bit
                        avalon_slv_rdata_o(reg_control_s'high downto 0) <= reg_control_s;   --bit[0]: com_start, bits[7..4]: step
                    when 4 =>
                        avalon_slv_rdata_o <= (others => '0'); --'0' for others bit
                        avalon_slv_rdata_o(0) <= enable_s;     --bit[0]: enable_s
                    when 5 =>
                        avalon_slv_rdata_o <= (others => '0'); --'0' for others bit
                        avalon_slv_rdata_o(reg_val_init_s'high downto 0) <= reg_val_init_s; --bits[29..0]: reg_val_init_s
                    when 6 =>
                        avalon_slv_rdata_o <= (others => '0'); --'0' for others bit
                        avalon_slv_rdata_o(reg_cpt_s'high downto 0) <= reg_cpt_s;           --bits[29..0]: etat cpt
                   
                   --user can complete address map from here 
                    when 7 => 
                        avalon_slv_rdata_o <= hex03_register_s;
                    when 8 => 
                        avalon_slv_rdata_o <= hex54_register_s;
                    
                    when others => 
                        -- default value for address not used
                        avalon_slv_rdata_o <= x"A5A5A5A5";
                end case;
            else
                -- default value when no read access
                avalon_slv_rdata_o   <= x"AA554321";
                avalon_slv_rvalide_o <= '0';
            end if;
        end if;
    end process;
   
    -----------------------------------------------------------------------------
    --process for write transfert
    -----------------------------------------------------------------------------
    process(clk_i, reset_i)
        variable addr_interne_wr_v  : std_logic_vector(AVALON_ADDR_WIDTH-1 downto ADDR_LSB);
        variable loc_addr_wr_v  : natural;
    begin
        if reset_i = '1' then
            --user need to complete register to be cleared
            tst_reg_interne_s <= (others => '0');
            reg_status_s      <= (others => '0');
            reg_control_s     <= (others => '0');
            reg_val_init_s    <= (others => '0');
        elsif rising_edge(clk_i) then
            -- default values
            com_start_s <= '0';
            if avalon_slv_write_i = '1' then
                --loc_addr_int_v  := to_integer(unsigned(avalon_slv_add_i(AVALON_ADDR_WIDTH-1 downto ADDR_LSB)));  -- EMI 17 avril 2018
                addr_interne_wr_v  := avalon_slv_add_i(AVALON_ADDR_WIDTH-1 downto ADDR_LSB);                         -- EMI 17 avril 2018
                loc_addr_wr_v  := to_integer(unsigned(addr_interne_wr_v));                                            -- EMI 17 avril 2018
                case loc_addr_wr_v is
                --user need to fill action for each address
                    when 0 => null; --no write at cst address
                    when 1 => tst_reg_interne_s <= avalon_slv_wdata_i;
                    when 2 => if avalon_slv_wdata_i(0) = '1' then
                                reg_status_s(0) <= '0';
                            end if;
                    when 3 => reg_control_s(reg_control_s'high downto 1) <= avalon_slv_wdata_i(reg_control_s'high downto 1);
                              com_start_s <= avalon_slv_wdata_i(0);
                    when 4 => enable_s <= avalon_slv_wdata_i(0);
                    when 5 => reg_val_init_s <= avalon_slv_wdata_i(reg_val_init_s'high downto 0);
                    when 6 => null;  --no write for cpt_o
                    
                    --user can complete address map from here 
                    when 7 => 
                        hex03_register_s <= avalon_slv_wdata_i;
                    when 8 => 
                        hex54_register_s <= avalon_slv_wdata_i;
                    

                    when others => null;
                end case;
            end if;
            -- assign interrupt register
            if cpt_complete_s = '1' then
                reg_status_s(0) <= '1';
            end if;
        end if;
    end process;
    
    -----------------------------------------------------------------------------
    --connection horloge minute-seconde
    -----------------------------------------------------------------------------  
    inst_cpt: cpt_socf port map( 
        reset_i      => reset_i,
        clk_i        => clk_i,
        enable_i     => enable_s,
        start_i      => com_start_s,
        step_i       => reg_control_s(7 downto 4),
        val_init_i   => reg_val_init_s,
        cpt_o        => reg_cpt_s,
        busy_o       => busy_s
        );

    -----------------------------------------------------------------------------
    -- falling edge detection
    -----------------------------------------------------------------------------
    process(clk_i, reset_i)
    begin
      if reset_i = '1' then
        cpt_complete_s <= '0';
        busy_prev_s    <= '0';
      elsif rising_edge(clk_i) then
        busy_prev_s    <= busy_s;
        cpt_complete_s <= busy_prev_s and not busy_s;
      end if;
    end process;

    irq_o <= reg_status_s(0);
    hex03_o     <= hex03_register_s; 
    hex54_o     <= hex54_register_s;
 
end comport;
