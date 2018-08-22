library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;

entity ram is
	generic(
		NUM_ELEMENTS      : integer;
		LOG2_NUM_ELEMENTS : integer;
		ELEMENT_WIDTH     : integer);
	port(
		clk               : in  std_logic;
		address           : in  std_logic_vector( LOG2_NUM_ELEMENTS - 1 downto 0 );
		writeEn           : in  std_logic;
		writeData         : in  std_logic_vector( ELEMENT_WIDTH - 1 downto 0 );
		readEn            : in  std_logic;
		readData          : out std_logic_vector( ELEMENT_WIDTH - 1 downto 0 );
		ramElements_debug : out ram_elements_type);
end ram;

architecture behavioral of ram is

	-- Array, das alle Elemente des RAMs enthält
	signal ramElements : ram_elements_type := (others => (others => '0'));

begin

	-- RAM-Beschreibung hier einfügen
	
	ram : process(clk, writeEn, readEn)
	begin
                
		if (clk'EVENT and clk = '1' and writeEn = '1' and readEn = '1') then
			for i in 0 to ELEMENT_WIDTH - 1 loop
				ramElements(to_integer(unsigned(address)))(i) <= writeData(i);
				readData(i) <= writeData(i);
			end loop;
		elsif (clk'EVENT and clk = '1' and writeEn = '1') then
			for i in 0 to ELEMENT_WIDTH - 1 loop
				ramElements(to_integer(unsigned(address)))(i) <= writeData(i);
			end loop;
		elsif (clk'EVENT and clk = '1' and readEn = '1') then
			for i in 0 to ELEMENT_WIDTH - 1 loop
				readData(i) <= ramElements(to_integer(unsigned(address)))(i);
			end loop;
		end if;
	end process;

	-- RAM-Inhalt über den Debug-Port nach außen führen
	ramElements_debug <= ramElements;

end behavioral;
