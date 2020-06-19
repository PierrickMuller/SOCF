-------------------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- Fichier      : cpt_socf.vhd
--
-- Description  : compteur configurable pour test SoCF du 12 juin 2018
-- 
-- Auteur       : Etienne Messerli
-- Date         : 11.06.2018
-- Version      : 0.0
-- 
-- 
--| Modifications |------------------------------------------------------------
-- Vers   Qui   Date      Description
--
--
-------------------------------------------------------------------------------

library ieee;
  use ieee.Std_Logic_1164.all;
  use ieee.numeric_std.all;

entity cpt_socf is
    port( 
        reset_i        : in   std_logic;
        clk_i          : in   std_logic;
        enable_i       : in   std_logic;
        start_i        : in   std_logic;
        step_i         : in   std_logic_vector(  3 downto 0);   -- pas du compteur
        val_init_i     : in   std_logic_vector( 29 downto 0);  -- valeur d'initialisation 
        cpt_o          : out  std_logic_vector( 29 downto 0);  -- etat du compteur 
        busy_o         : out  std_logic                        -- comptage en cours
    );
end cpt_socf ;

architecture comport of cpt_socf is
    -- Internal Signal Declarations
    signal cpt_fut, cpt_pres : unsigned(29 downto 0);
    signal val_init_s        : unsigned(29 downto 0);
    signal step_s            : unsigned( 3 downto 0);
    signal start_s     : std_logic;   --synchronisation du start
    signal start_mem_s : std_logic;   --etat precedent de start
    signal init_s      : std_logic;   --init compteur
    signal run_s       : std_logic;   --comptage en cours
    signal det_end_s   : std_logic;   --detection seuil atteint

begin

    --adaptations
    step_s     <= unsigned(step_i);
    val_init_s <= unsigned(val_init_i);
  
    --synchronisation et memorisation start
    sync_start: process (clk_i, reset_i)
    begin
        if (reset_i = '1') then
            start_s     <= '0';
            start_mem_s <= '0';
        elsif rising_edge(clk_i) then
            start_s     <= start_i;
            start_mem_s <= start_s;
        end if;
    end process;

    --Gestion du start avec detection de flanc
    bloc_start: process (clk_i, reset_i)
    begin
        if (reset_i = '1') then
            init_s      <= '0';
            run_s       <= '0';
        elsif rising_edge(clk_i) then
            init_s  <= start_s and not start_mem_s;         --init lors det flanc start
            if (enable_i = '1') then                        -- ici pour inactiver le signal start si enable a 0
                if (start_s = '1') and (start_mem_s = '0') then --detection flanc montant
                    run_s   <= '1';
                elsif det_end_s = '1' then
                    run_s  <= '0';
                end if;
            end if;
        end if;
    end process;
  

    cpt_fut <= cpt_pres          when (enable_i = '0') else
               val_init_s        when (init_s = '1') else
               cpt_pres - step_s when (run_s = '1') else
               cpt_pres;

    cpt_mem: process (clk_i, reset_i)
    begin
        if (reset_i = '1') then
            cpt_pres <= (others => '0');
        elsif rising_edge(clk_i) then
            cpt_pres <= cpt_fut;
        end if;
    end process;

    --detection seuil du compteur
    det_end_s <= '1' when (run_s = '1') and (cpt_pres = 0) else
                 '0';
  
    --affectation sortie
    busy_o <= run_s;
    cpt_o  <= std_logic_vector(cpt_pres);
  
end comport;
