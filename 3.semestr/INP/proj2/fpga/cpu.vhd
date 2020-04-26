-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

-- signĂˇly pro ÄŤitaÄŤ CNT
signal CNT_inc: std_logic;                      --increment pro CNT
signal CNT_dec: std_logic;                      --decrement pro CNT
signal CNT_out: std_logic_vector(7 downto 0);  --vĂ˝sledek pro CNT

-- signĂˇly pro ÄŤitaÄŤ PC
signal PC_inc: std_logic;                       --increment pro PC
signal PC_dec: std_logic;                       --decrement pro PC
signal PC_out: std_logic_vector(12 downto 0);   --vĂ˝sledek pro PC

-- signĂˇly pro ÄŤitaÄŤ PTR
signal PTR_inc: std_logic;                      --increment pro PTR
signal PTR_dec: std_logic;                      --decrement pro PTR
signal PTR_out: std_logic_vector(12 downto 0) := "1000000000000";   --vysledek pro PTR je v nÄ›m 0x1000

-- rozhodovacĂ­ signĂˇly pro multiplexory
signal sel_MX1: std_logic;                      --Ĺ™Ă­dĂ­cĂ­ signĂˇl MX1
signal sel_MX2: std_logic;                      --Ĺ™Ă­dĂ­cĂ­ signĂˇl MX2
signal sel_MX3: std_logic_vector(1 downto 0);   --Ĺ™Ă­dĂ­cĂ­ signĂˇl M3
signal MX2_MX1: std_logic_vector(12 downto 0);   --vĂ˝stup MX2 a vstup MX1
signal in_MX2: std_logic_vector(12 downto 0) := "1000000000000";   --vstup pro MX2 (vĹľdy 0x1000)

--pomocnĂˇ promnÄ›nĂˇ
signal TMP: std_logic_vector(12 downto 0);      --adresa pomocnĂ© promnÄ›nĂ© je v nÄ›m 0x1000

type desider_state is
  (
    sStart,
    sDecode,
    sPointerInc,
    sPointerDec,
    sFetch,
    sVarIncS, sVarIncE,
    sVarDecS, sVarDecE,
    sWhileBeg, sWhileBeg1, sWhileBeg2, sWhileBeg3,
    sWhileEnd, sWhileEnd1, sWhileEnd2, 
    sPutcharS, sPutcharE,     --Start, End jelikoĹľ RAM mĂˇ jeden CLK zpoĹľdÄ›nĂ­
    sGetchar,
    sSaveTemp,
    sLoadTemp,
    sEEnd,
    sElse
    
  );

signal preState: desider_state;
signal nextState: desider_state;

--type instruction is
--  (
--    pointerInc,
--    pointerDec,
--    varInc,
--    varDec,
--    whileS,
--   whileE,
--    putchar,
--   getchar,
--    saveTemp,
--    loadTemp,
--    iEnd
--  );


begin

 -- zde dopiste vlastni VHDL kod


 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.

--ÄŤitaÄŤ CNT
CNT_citac: process(CLK, RESET, CNT_inc, CNT_dec, CNT_out)
begin
  if(RESET = '1') then
    CNT_out <= (others => '0');
  elsif(CLK'event and CLK = '1') then
    if(CNT_inc = '1') then
      CNT_out <= CNT_out + 1;
    elsif(CNT_dec = '1') then
      CNT_out <= CNT_out - 1;
    end if;
  end if;
end process;

--ÄŤitaÄŤ PC
PC_citac: process(CLK, RESET, PC_inc, PC_dec, PC_out)
begin
  if(RESET = '1') then
    PC_out <= (others => '0');
  elsif(CLK'event and CLK = '1') then
    if(PC_inc = '1') then
      PC_out <= PC_out + 1;
    elsif(PC_dec = '1') then
      PC_out <= PC_out - 1;
    end if;
  end if;
end process;

--ÄŤitaÄŤ PTR
PTR_citac: process(CLK, RESET, PTR_inc, PTR_dec, PTR_out)
begin
  if(RESET = '1') then
    PTR_out <= (others => '0');
  elsif(CLK'event and CLK = '1') then
    if(PTR_inc = '1') then
      PTR_out <= PTR_out + 1;
    elsif(PTR_dec = '1') then
      PTR_out <= PTR_out - 1;
    end if;
  end if;
end process;

--Multiplexor 1
multiplex1: process(PC_out, MX2_MX1, sel_MX1)
begin
  if(sel_MX1 = '0') then
    DATA_ADDR <= PTR_out;
  else
    DATA_ADDR <= MX1_MX2;
  end if;
end process;

--Multiplexor 2
multiplex2: process(PTR_out, TMP, sel_MX2)
begin
  if(sel_MX2 = '0') then
    MX2_MX1 <= PTR_out;
  else
    MX2_MX1 <= TMP;
  end if;
end process;

--multiplexor 3
multiplex3: process(IN_DATA, DATA_RDATA, sel_MX3)
begin
  case(sel_MX3) is
    when "00" => DATA_WDATA <= IN_DATA;
    when "01" => DATA_WDATA <= DATA_RDATA + 1;
    when "10" => DATA_WDATA <= DATA_RDATA - 1;
    when "11" => DATA_WDATA <= DATA_RDATA;
	 when others =>
  end case;
end process;

--posouvaÄŤ stavĹŻ
state_Changer: process(CLK, RESET, EN)
begin
  if(RESET = '1') then
    preState <= sStart;
  elsif(CLK'event and CLK = '1') then
    if(EN = '1') then
      preState <= nextState;
    end if;
  end if;
end process;

automat_Desider: process(OUT_BUSY, sel_MX1, sel_MX2, sel_MX3, IN_VLD, CNT_out, OUT_BUSY, preState, DATA_RDATA)
begin

  IN_REQ <= '0';
  DATA_EN <= '0';
  OUT_WE <= '0';

  CNT_inc <= '0';
  CNT_dec <= '0';

  PC_inc <= '0';
  PC_dec <= '0';

  PTR_inc <= '0';
  PTR_dec <= '0';

  sel_MX3 <= "11";

  case preState is
    when sStart =>
      DATA_EN <= '1';
      DATA_RDWR <= '0';
      sel_MX1 <= '1';
      nextState <= sDecode;

    when sDecode =>
      case (DATA_RDATA) is
        when X"3E" => nextState <= sPointerInc;
        when X"3C" => nextState <= sPointerDec;
        when X"2B" => nextState <= sVarIncS;
        when X"2D" => nextState <= sVarDecS;
        when X"5B" => nextState <= sWhileBeg;
        when X"5D" => nextState <= sWhileEnd;
        when X"2E" => nextState <= sPutcharS;
        when X"2C" => nextState <= sGetchar;
        when X"24" => nextState <= sSaveTemp;
        when X"21" => nextState <= sLoadTemp;
        when X"00" => nextState <= sEEnd;
        when others => nextState <= sElse;
      end case;
      
    when sPointerInc =>
      PC_inc <= '1';
      PTR_inc <= '1';
      nextState <= sStart;

    when sPointerDec =>
      PC_inc <= '1';
      PTR_dec <= '1';
      nextState <= sStart;

    when sVarIncS =>
		DATA_EN <= '1';
		DATA_RDWR <= '0';
      sel_MX1 <= '0';
      nextState <= sVarIncE;

    when sVarIncE =>
      PC_inc <= '1';
      DATA_EN <= '1';
      DATA_RDWR <= '1';
      sel_MX1 <= '0';
      sel_MX3 <= "01";
      nextState <= sStart;
    
     when sVarDecS =>
       DATA_EN <= '1';
       DATA_RDWR <= '0';
       sel_MX1 <= '0';
       nextState <= sVarDecE;
      
    when sVarDecE =>
      PC_inc <= '1';
      DATA_EN <= '1';
      DATA_RDWR <= '1';
      sel_MX1 <= '0';
      sel_MX3 <= "01";
      nextState <= sStart;

    when sPutcharS => 
      DATA_EN <= '1';
      DATA_RDWR <= '0';
      sel_MX1 <= '0';
      nextState <= SPutcharE;
   

    when sPutcharE =>
      nextState <= sPutcharE;
      if(OUT_BUSY = '0') then
        OUT_WE <= '1';
        OUT_DATA <= DATA_RDATA;
        PC_inc <= '1';
        nextState <= sStart;
      end if;

    when sGetchar =>
      IN_REQ <= '1';
      if(IN_VLD /= '1') then    --linka je obsazena
        nextState <= sGetchar;
      else
        PC_inc <= '1';
        sel_MX3 <= "00";
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        nextState <= sStart;
      end if;

    when sWhileBeg =>
      PC_inc <= '1';
      DATA_EN <= '1';
      DATA_RDWR <= '0';
      nextState <= sWhileBeg1;

    when sWhileBeg1 =>
      if(DATA_RDATA = "00000000") then
        PC_inc <= '1';
        CNT_inc <= '1';               --moĹľnĂˇ zmnÄ›nit na CNT_out <= 1 (v binarce)
        nextState <= sWhileBeg2;
      else
        nextState <= sStart;
      end if;

    when sWhileBeg2 =>
      if(CNT_out = "00000000") then
        nextState <= sStart;
      else
        DATA_EN <= '1';
        nextState <= sWhileBeg3;
      end if;

    when sWhileBeg3 =>
      if(PC_out = X"5B") then
        CNT_inc <= '1';
      elsif(PC_out = X"5D") then
        CNT_dec <= '1';
      end if;
      PC_inc <= '1';
      nextState <= sWhileBeg2;

    when sWhileEnd =>
      DATA_EN <= '1';
      DATA_RDWR <= '1';
      nextState <= sWhileEnd1;

    when sWhileEnd1 =>
      PC_inc <= '1';
      if(DATA_RDATA /= "00000000") then
        nextState <= sStart;
      else
        CNT_inc <= '1';
        PC_dec <= '1';
        nextState <= sWhileEnd2;
      end if;

    when sWhileEnd2 =>
      if(CNT_out /= "00000000") then
        nextState <= sStart;
      end if;
      if(DATA_RDATA = X"5D") then
        CNT_inc <= '1';
      elsif(DATA_RDATA = X"5B") then
        CNT_dec <= '1';
      end if;
      if(CNT_out = "00000000") then
        PC_inc <= '1';
      else
        PC_dec <= '1';
      end if;

    when sSaveTemp =>
      TMP <= PTR_out;
      PC_inc <= '1';
      nextState <= sStart;   

    when sLoadTemp =>
      PTR_out <= TMP;
      PC_inc <= '1';

    when sEEnd =>
      nextState <= sEEnd;

    when sElse =>
      PC_inc <= '1';
		nextState <= sStart;
	 
	 when others =>

  end case;
end process;



 
end behavioral;
 
