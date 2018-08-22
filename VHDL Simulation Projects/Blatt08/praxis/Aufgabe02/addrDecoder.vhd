library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity addrDecoder is
	generic(ADDR_WIDTH : integer;
			POW2_ADDR_WIDTH : integer);
	port(address : in std_logic_vector(ADDR_WIDTH - 1 downto 0);
		 bitmask : out std_logic_vector(POW2_ADDR_WIDTH - 1 downto 0));
end addrDecoder;

architecture behavioral of addrDecoder is
begin

	-- Beschreibung des Adressdekoders hier einfügen
	decoder : process(address)
	begin
		for i in 0 to POW2_ADDR_WIDTH - 1 loop
			if (i = to_integer(unsigned(address))) then
				bitmask(i) <= '1';
			else 
				bitmask(i) <= '0';
			end if;
		end loop;
	end process;
	
end behavioral;
