library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library std;
use std.standard.all;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;

entity ram is
	generic(NUM_ELEMENTS : integer;
			LOG2_NUM_ELEMENTS : integer;
			ELEMENT_WIDTH : integer;
			INIT_FILE_NAME : string);
	port(clk : in std_logic;
		 address : in std_logic_vector(LOG2_NUM_ELEMENTS - 1 downto 0);
		 writeEn : in std_logic;
		 writeData : in std_logic_vector(ELEMENT_WIDTH - 1 downto 0);
		 readEn : in std_logic;
		 readData : out std_logic_vector(ELEMENT_WIDTH - 1 downto 0);
		 ramElements_debug : out ram_elements_type);
end ram;

architecture behavioral of ram is

begin

	process(clk)
	
		variable ramElements : ram_elements_type := (others => (others => '0'));
		
		-- synthesis translate_off
		variable flashed : boolean := false;
		file dataFile : text open read_mode is INIT_FILE_NAME;
		variable lineBuffer : line;
		variable dataBuffer : bit_vector(ELEMENT_WIDTH - 1 downto 0);
		variable i : integer := 0;
		-- synthesis translate_on
		
	begin
	
		-- synthesis translate_off
		if not flashed then
			report "flashing RAM ...";
			while not endfile(dataFile) loop
				readline(dataFile, lineBuffer);
				read(lineBuffer, dataBuffer);
				ramElements(i) := to_stdlogicvector(dataBuffer);
				i := i + 1;
			end loop;
			flashed := true;
			report "RAM flashed!";

		end if;
		-- synthesis translate_on
		
		if rising_edge(clk) then
			-- write
			if writeEn = '1' then
				ramElements(to_integer(unsigned(address))) := writeData;
			end if;

			-- read
			if readEn = '1' then
				if writeEn = '1' then
					readData <= writeData;
				else
					readData <= ramElements(to_integer(unsigned(address)));
				end if;
			end if;
		end if;
		
		ramElements_debug <= ramElements;
		
	end process;

end behavioral;
