-----------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- File         : axi4lite_slave.vhd
-- Author       : E. Messerli    27.07.2017
-- Description  : slave interface AXI  (without burst)
-- used for     : SOCF lab
--| Modifications |-----------------------------------------------------------
-- Ver  Date       Auteur  Description
-- 1.0  26.03.2019  EMI    Adaptation du chablon pour les etudiants  
-- 1.1  08.05.2029  PMR    Complétion du laboratoire
------------------------------------------------------------------------------


library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    use ieee.std_logic_unsigned.all;

entity axi4lite_slave is
    generic (
        -- Users to add parameters here

        -- User parameters ends

        -- Width of S_AXI data bus
        AXI_DATA_WIDTH  : integer   := 32;  -- 32 or 64 bits
        -- Width of S_AXI address bus
        AXI_ADDR_WIDTH  : integer   := 12
    );
    port (
        axi_clk_i       : in  std_logic;
        axi_reset_i     : in  std_logic;
        -- AXI4-Lite 
        axi_awaddr_i    : in  std_logic_vector(AXI_ADDR_WIDTH-1 downto 0);
        axi_awprot_i    : in  std_logic_vector( 2 downto 0); -- Pas utilisés
        axi_awvalid_i   : in  std_logic;
        axi_awready_o   : out std_logic;
        axi_wdata_i     : in  std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
        axi_wstrb_i     : in std_logic_vector((AXI_DATA_WIDTH/8)-1 downto 0); 
        axi_wvalid_i    : in  std_logic;
        axi_wready_o    : out std_logic;
        axi_bresp_o     : out std_logic_vector(1 downto 0);
        axi_bvalid_o    : out std_logic;
        axi_bready_i    : in  std_logic;
        axi_araddr_i    : in  std_logic_vector(AXI_ADDR_WIDTH-1 downto 0);
        axi_arprot_i    : in  std_logic_vector( 2 downto 0); -- Pas utilisés
        axi_arvalid_i   : in  std_logic;
        axi_arready_o   : out std_logic;
        axi_rdata_o     : out std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
        axi_rresp_o     : out std_logic_vector(1 downto 0);
        axi_rvalid_o    : out std_logic;
        axi_rready_i    : in  std_logic
        
        -- User input-output
    
        
    );
end entity axi4lite_slave;

architecture rtl of axi4lite_slave is

    signal reset_s : std_logic;

    -- local parameter for addressing 32 bit / 64 bits, cst: AXI_DATA_WIDTH
    -- ADDR_LSB is used for addressing word 32/64 bits registers/memories
    -- ADDR_LSB = 2 for 32 bits (n-1 downto 2)
    -- ADDR_LSB = 3 for 64 bits (n-1 downto 3)
    constant ADDR_LSB  : integer := (AXI_DATA_WIDTH/32)+ 1;
    
    --signal for the AXI slave
    --intern signal for output
    signal axi_awready_s       : std_logic;
    signal axi_wready_s        : std_logic;
    signal axi_arready_s       : std_logic;
    signal axi_rvalid_s        : std_logic;
    signal axi_bvalid_s        : std_logic;
    signal axi_bresp_s         : std_logic_vector(1 downto 0);
    signal axi_rdata_s          : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal axi_wdata_s    : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
     --intern signal for the axi interface
    signal axi_waddr_mem_s     : std_logic_vector(AXI_ADDR_WIDTH-1 downto ADDR_LSB);
    signal axi_araddr_mem_s    : std_logic_vector(AXI_ADDR_WIDTH-1 downto ADDR_LSB);
    signal axi_data_wren_s     : std_logic;
    signal axi_data_reen_s     : std_logic;
    
    -- intern signal for MD5 control
    signal wb_s                : std_logic_vector(511 downto 0);
    signal footprint_s         : std_logic_vector(127 downto 0);
    signal footprint_temp_s    : std_logic_vector(127 downto 0);
    signal init_val_s          : std_logic_vector(127 downto 0); 
    signal en_s                : std_logic;
    signal valid_i_s           : std_logic;
    signal valid_o_s           : std_logic;
    signal valid_footprint_s    : std_logic;
    signal footprint1_s        : std_logic_vector(31 downto 0);
    signal footprint2_s        : std_logic_vector(31 downto 0);
    signal footprint3_s        : std_logic_vector(31 downto 0);
    signal footprint4_s        : std_logic_vector(31 downto 0);   
   
     -- signal representing registers 
    signal const_register_s      : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal test_register_s      : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal data_md5_s           : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal start_md5_work_s     : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal md5_work_done_s      : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
    signal start_md5_block_s    : std_logic_vector(AXI_DATA_WIDTH-1 downto 0);
        
    -- signaux permettant la gestion du strobe 
    signal temp_Vect_Strb_1_s : std_logic_vector(AXI_DATA_WIDTH/4 -1 downto 0);
    signal temp_Vect_Strb_2_s : std_logic_vector(AXI_DATA_WIDTH/4 -1 downto 0);
    signal temp_Vect_Strb_3_s : std_logic_vector(AXI_DATA_WIDTH/4 -1 downto 0);
    signal temp_Vect_Strb_4_s : std_logic_vector(AXI_DATA_WIDTH/4 -1 downto 0);
   
    
    -- component declaration
    component Md5Core is
    port( 
        clk_i       : in  std_logic;
        rst_i       : in  std_logic;
        en_i        : in  std_logic;
        valid_i     : in  std_logic;
        wb_i        : in  std_logic_vector (511 downto 0);
        init_val_i  : in  std_logic_vector (127 downto 0);
        valid_o     : out std_logic;
        footprint_o : out std_logic_vector (127 downto 0)
    );
    end component;
    
begin

    reset_s  <= axi_reset_i;

        ------------------------------
    -- Components instanciation --
    ------------------------------
    uut : Md5Core
        port map(clk_i       => axi_clk_i,
                 rst_i       => axi_reset_i,
                 en_i        => en_s,
                 valid_i     => valid_i_s, 
                 wb_i        => wb_s,
                 init_val_i  => init_val_s,--x"67452301EFCDAB8998BADCFE10325476",
                 valid_o     => valid_o_s,
                 footprint_o => footprint_s
        );
   
  
-----------------------------------------------------------
-- Write address channel

    -- GESTION DU STROBE
    -- On récupère les bits voulus en fonction du strobe et on assemble les résultats dans un signal temporaire (axi_wdata_s)
    temp_Vect_Strb_1_s <= axi_wdata_i(7 downto 0) when axi_wstrb_i(0) = '1' else (others => '0'); 
    temp_Vect_Strb_2_s <= axi_wdata_i(15 downto 8) when axi_wstrb_i(1) = '1' else (others => '0');
    temp_Vect_Strb_3_s <= axi_wdata_i(23 downto 16) when axi_wstrb_i(2) = '1' else (others => '0');
    temp_Vect_Strb_4_s <= axi_wdata_i(31 downto 24) when axi_wstrb_i(3) = '1' else (others => '0');
    axi_wdata_s <= temp_Vect_Strb_4_s & temp_Vect_Strb_3_s & temp_Vect_Strb_2_s & temp_Vect_Strb_1_s;
    
    
    process (reset_s, axi_clk_i)
    begin
        if reset_s = '1' then
            axi_awready_s <= '0';
            axi_waddr_mem_s <= (others => '0');
        elsif rising_edge(axi_clk_i) then
            if (axi_awready_s = '0' and axi_awvalid_i = '1')  then --and axi_wvalid_i = '1') then  modif EMI 10juil2018
                -- slave is ready to accept write address when
                -- there is a valid write address
                axi_awready_s <= '1';
                -- Write Address memorizing
                axi_waddr_mem_s <=  axi_awaddr_i(AXI_ADDR_WIDTH-1 downto ADDR_LSB);
            else
                axi_awready_s <= '0';
            end if;
        end if;
    end process;
    axi_awready_o <= axi_awready_s;


-----------------------------------------------------------
-- Write data channel

    -- Implement axi_wready generation
    process (reset_s, axi_clk_i)
    begin
        if reset_s = '1' then
            axi_wready_s    <= '0';
        elsif rising_edge(axi_clk_i) then
            if(axi_wready_s = '0' and axi_wvalid_i = '1') then 
                axi_wready_s <= '1';
            else 
                axi_wready_s <= '0';
            end if;
        end if;
    end process;
    
    -- Assignation de la sortie axi_wready_o
    axi_wready_o <= axi_wready_s;


    --condition to write data. On peut écrire des données quand axi_wready_s et axi_wvalid_i sont actif (Selon documentation axi_lite)
    axi_data_wren_s <= '0' when (axi_wready_s = '1' and axi_wvalid_i = '1') else 
                       '1'; 
    
    
    process (reset_s, axi_clk_i)
        --number address to access 32 or 64 bits data
        variable int_waddr_v : natural;
    begin
        if reset_s = '1' then
            --Assignation des valeurs de base aux registres 
            test_register_s <= (others => '0');
            const_register_s <= x"DEADBEEF";
            md5_work_done_s <= (others => '0');
            data_md5_s <= (others => '0');
            start_md5_work_s <= (others => '0');
            start_md5_block_s <= (others => '0');
            wb_s <= (others => '0');
            init_val_s <= x"67452301EFCDAB8998BADCFE10325476";
            en_s <= '0';
            valid_i_s <= '0';
             
        elsif rising_edge(axi_clk_i) then
            
            -- Dans tous les cas, remise à 0 de valid_i en entrée
            valid_i_s <= '0';
            -- Si on peut écrire
            if axi_data_wren_s = '0' then
            
                int_waddr_v   := to_integer(unsigned(axi_waddr_mem_s));
                case int_waddr_v is
                    when 0   => null; -- constante, on écrit pas dedans
                    when 1   => test_register_s <= axi_wdata_s; -- Test register 
                    when 2   => data_md5_s <= axi_wdata_s; wb_s <= wb_s(479 downto 0) & axi_wdata_s; -- On stocke la valeur reçu dans le registre et on décalle le block pour accueillir la valeur.
                    when 3   => start_md5_work_s <= axi_wdata_s; -- Gestion du start du travail sur un fichier. En remettant à 0, on effectue un reset.
                                if axi_wdata_s(0) = '0' then 
                                    init_val_s <= x"67452301EFCDAB8998BADCFE10325476";
                                    en_s <= '0';
                                    valid_i_s <= '0';
                                    wb_s <= (others => '0');
                                end if;
                    when 4   => md5_work_done_s <= axi_wdata_s; -- Permet de remettre à 0 
                    when 5   => start_md5_block_s <= axi_wdata_s; -- Quand on start un bloq, on met l'enable et le valid à 1 pour le bloc md5
                                if axi_wdata_s(0) = '1' then 
                                    en_s <= '1';
                                    valid_i_s <= '1';
                                end if;
                                
                    when others => null;  --on écrit pas dedans
                end case;
            end if;
            if valid_footprint_s = '1' then     -- Permet d'avoir le valid généré pour la gestion du footprint (Voir bloc read) actif un coups de clock
                valid_footprint_s <= '0';
            end if;
            if valid_o_s = '1' then -- Quand un hash à été généré
                
                -- Remise à 0 du valid et on monte le flage "md5_work_done" à 1.
                valid_i_s <= '0';
                md5_work_done_s <= x"00000001";
                
                --if to_integer(unsigned(start_md5_work_s)) = 1 then 
                --init_val_s <= footprint_s;

                -- Gestion du footprint avec addition et gestion de la nouvelle valeur de l'init
                footprint1_s <= init_val_s(127 downto 96) + footprint_s(127 downto 96); 
                footprint2_s <= init_val_s(95 downto 64) + footprint_s(95 downto 64);
                footprint3_s <= init_val_s(63 downto 32) + footprint_s(63 downto 32);
                footprint4_s <= init_val_s(31 downto 0) + footprint_s(31 downto 0);
                
                init_val_s  <= footprint1_s & footprint2_s & footprint3_s & footprint4_s;
                
                -- Permet la récupération du bon footprint
                valid_footprint_s <= '1';

            end if;
            
        end if;
    end process;
    
-----------------------------------------------------------
-- Write response channel

   process (reset_s , axi_clk_i)
   begin 
    if reset_s = '1' then
        -- Assigniation des valeurs de base pour les signaux du canal réponse de write
        axi_bvalid_s <= '0';
        axi_bresp_s <= "00";
    elsif rising_edge(axi_clk_i) then
        -- Si on peut écrire, on met le signal bvalid_s actif. La réponse sera toujours 00 dans notre cas. Je laisse l'assigniation de 
        -- la réponse ici dans le cas d'une future modification.
        if axi_data_wren_s = '0' then 
            axi_bvalid_s <= '1';
            axi_bresp_s <= "00";
        else
            axi_bvalid_s <= '0';
        end if;
    end if;
   end process;
   
   -- Assignation des signaux de réponses 
   axi_bvalid_o <= axi_bvalid_s;
   axi_bresp_o <= axi_bresp_s;

    

-----------------------------------------------------------
-- Read address channel

    process (reset_s, axi_clk_i)
    begin
        if reset_s = '1' then
           axi_arready_s    <= '0';
           axi_araddr_mem_s <= (others => '1');
        elsif rising_edge(axi_clk_i) then
            if axi_arready_s = '0' and axi_arvalid_i = '1' then
                -- indicates that the slave has acceped the valid read address
                axi_arready_s    <= '1';
                -- Read Address memorizing
                axi_araddr_mem_s <= axi_araddr_i(AXI_ADDR_WIDTH-1 downto ADDR_LSB);
            else
                axi_arready_s    <= '0';
            end if;
        end if;
    end process;
    
    axi_arready_o <= axi_arready_s;

-----------------------------------------------------------
-- Read data channel


    process (reset_s, axi_clk_i)
        --number address to access 32 or 64 bits data
        variable int_raddr_v : natural;
    begin
        if reset_s = '1' then
          footprint_temp_s <= (others => '0');
          axi_rdata_s <= (others => '0');
          axi_rvalid_s    <= '0';
        elsif rising_edge(axi_clk_i) then
            -- Afin de respecter les timings, pour pouvoir remplir le signal axi_rdata_s, on doit s'assurer que axi_arready_s et axi_arvalid_i soit à 1
            -- si c'est le cas, on met axi_rvalid_s actif, on récupère l'adresse recu pour la lecture et on remplit le axi_rdata_s avec le 
            -- contenu du registre correspondant.
            if axi_arready_s = '1' and axi_arvalid_i = '1' then
                axi_rvalid_s <= '1';
                int_raddr_v   := to_integer(unsigned(axi_araddr_mem_s));
                case int_raddr_v is
                    when 0   => axi_rdata_s <= const_register_s; -- constante, on écrit pas dedans
                    when 1   => axi_rdata_s <= test_register_s; -- Test register 
                    when 2   => axi_rdata_s <= footprint_temp_s(127 downto 96); -- Gestion du footprint pour la récupération par lecture
                                footprint_temp_s <= footprint_temp_s(95 downto 0) & x"00000000";
                    when 3   => axi_rdata_s <= start_md5_work_s;
                    when 4   => axi_rdata_s <= md5_work_done_s;
                    when 5   => axi_rdata_s <= start_md5_block_s;
                    when others => null;    -- Dans les autres cas, on ne fait rien
                end case;
            else
                axi_rvalid_s <= '0';
            end if;
            if valid_footprint_s = '1' then -- Permet de récupérer le footprint souhaité 
                footprint_temp_s <= init_val_s;
            end if ;
            
        end if;
    end process;
    
   -- Assignation des sorties des canaux de lectures 
   axi_rvalid_o <= axi_rvalid_s;
   axi_rdata_o <= axi_rdata_s;
   axi_rresp_o <= "00";



end rtl;
