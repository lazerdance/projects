library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library RorgPrSimLib;
use RorgPrSimLib.all;
use RorgPrSimLib.proc_config.all;

library work;
use work.all;

entity regFile is
	generic(NUM_REGS : integer;
			LOG2_NUM_REGS : integer;
			REG_WIDTH : integer);
	port(clk : in std_logic;
		 rst : in std_logic;
		 readAddr1 : in std_logic_vector(LOG2_NUM_REGS - 1 downto 0);
		 readData1 : out std_logic_vector(REG_WIDTH - 1 downto 0);
		 readAddr2 : in std_logic_vector(LOG2_NUM_REGS - 1 downto 0);
		 readData2 : out std_logic_vector(REG_WIDTH - 1 downto 0);
		 writeEn : in std_logic;
		 writeAddr : in std_logic_vector(LOG2_NUM_REGS - 1 downto 0);
		 writeData : in std_logic_vector(REG_WIDTH - 1 downto 0);
		 reg_vect_debug : out reg_vector_type);
end regFile;

architecture structural of regFile is

	-- Array, das alle Einzelregister enthält
	signal reg_vect : reg_vector_type;
	signal dec_and : std_logic_vector(NUM_REGS - 1 downto 0);
	signal writeSignal : std_logic_vector(NUM_REGS - 1 downto 0);

	component reg
		generic (WIDTH : integer := REG_WIDTH);
		port(	clk : in std_logic;
			rst : in std_logic;
			en  : in std_logic;
			D   : in std_logic_vector(REG_WIDTH - 1 downto 0);
			Q   : out std_logic_vector(REG_WIDTH - 1 downto 0));
	end component;

for all: reg use entity work.reg(behavioral);

begin

	-- Beschreibung des Registerspeichers hier einfügen
	u1 : entity work.addrDecoder(behavioral)
		generic map (ADDR_WIDTH => LOG2_NUM_REGS,
				POW2_ADDR_WIDTH => NUM_REGS)
		port map (	address => writeAddr,
				bitmask => dec_and);

	register1 : for i in 0 to NUM_REGS - 1 generate
	u2 : reg
		port map(
			clk => clk,
			rst => rst,
			en => writeSignal(i),
			D => writeData,
			Q => reg_vect(i));
	end generate;

	mux : process(dec_and, writeEn)
	begin
		for i in 0 to NUM_REGS - 1 loop
			writeSignal(i) <= dec_and(i) and writeEn;
		end loop;
	end process;

	readData1 <= reg_vect(to_integer(unsigned(readAddr1)));
	readData2 <= reg_vect(to_integer(unsigned(readAddr2)));

	-- Inhalt des Registerspeichers über den Debug-Port nach außen führen
	reg_vect_debug <= reg_vect;

end structural;
