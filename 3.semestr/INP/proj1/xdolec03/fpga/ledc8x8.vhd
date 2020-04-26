-- Autor reseni: Václav Doleček xdolec03

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.
    RESET            : in std_logic;
    SMCLK            : in std_logic;

    ROW              : out std_logic_vector(0 to 7);
    LED              : out std_logic_vector(7 downto 0)
      
);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.
    signal pocitadlo : std_logic_vector(11 downto 0) := (others => '0');    --pocidadlo
    signal svetlo_tma_pocitadlo : std_logic_vector(22 downto 0) := (others => '0');
    signal svetlo_tma : std_logic;                                          --uruÄŤuje zda svĂ­tit nebo ne
    signal stav : std_logic;                                                
    signal ledky : std_logic_vector(7 downto 0) := (others => '1');
    signal radky : std_logic_vector(7 downto 0) := (others => '0');
	signal cyklo_zastavovac : std_logic;

begin

    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.

    
    citac: process(SMCLK, RESET, svetlo_tma_pocitadlo, pocitadlo)
        begin
            if RESET = '1' then
                pocitadlo <= (others => '0');               
            elsif SMCLK'event and SMCLK = '1' then
                pocitadlo <= pocitadlo + 1; 
            end if;
        end process citac;
    stav <= '1' when pocitadlo = "111000010000" else '0';

    radko_rotovac: process(RESET, stav, radky, SMCLK)
        begin
            if RESET = '1' then
                radky <= "10000000";
            elsif (SMCLK'event and SMCLK = '1' and stav = '1') then
                radky <= radky(0) & radky(7 downto 1);
            end if;
        end process radko_rotovac;
    
    citac_on_off_on: process(SMCLK, RESET, svetlo_tma_pocitadlo, cyklo_zastavovac)
        begin
            if RESET = '1' then
                svetlo_tma <= '0';
					 svetlo_tma_pocitadlo <= (others => '0');
					 cyklo_zastavovac <= '0';
			   elsif SMCLK'event and SMCLK = '1' and cyklo_zastavovac = '0' then
						svetlo_tma_pocitadlo <= svetlo_tma_pocitadlo + '1';
					if  	svetlo_tma_pocitadlo = "01110000100000000000000" then    ---cc3,5mega
							svetlo_tma <= '1';
					elsif svetlo_tma_pocitadlo = "11100001000000000000000" then     ----cca7mega
							svetlo_tma <= '0';
							cyklo_zastavovac <= '1';
					end if;
            end if;
        end process citac_on_off_on;

    dekoder: process(radky, svetlo_tma, ledky)
        begin
            if svetlo_tma = '0' then
                case radky is
                    when "10000000" => ledky <= "11101110";
                    when "01000000" => ledky <= "11101110";
                    when "00100000" => ledky <= "11110101";
                    when "00010000" => ledky <= "10000101";
                    when "00001000" => ledky <= "01101011";
                    when "00000100" => ledky <= "01101111";
                    when "00000010" => ledky <= "01101111";
                    when "00000001" => ledky <= "10001111";
                    when others =>     ledky <= "11111111";
                end case;
            else           
                case radky is
                    when "10000000" => ledky <= "11111111";
                    when "01000000" => ledky <= "11111111";
                    when "00100000" => ledky <= "11111111";
                    when "00010000" => ledky <= "11111111";
                    when "00001000" => ledky <= "11111111";
                    when "00000100" => ledky <= "11111111";
                    when "00000010" => ledky <= "11111111";
                    when "00000001" => ledky <= "11111111";
                    when others =>     ledky <= "11111111";
                end case;
            end if;
            ROW <= radky;
            LED <= ledky;
        end process dekoder;

    

end main;




-- ISID: 75579
