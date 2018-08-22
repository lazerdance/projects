library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity reg is
	generic(
		WIDTH : integer);
	port(
		clk : in  std_logic;
		rst : in  std_logic;
		en  : in  std_logic;
		D   : in  std_logic_vector( WIDTH - 1 downto 0 );
		Q   : out std_logic_vector( WIDTH - 1 downto 0 ));
end reg;

architecture behavioral of reg is
begin
	
	-- Register-Beschreibung hier einfügen
	arbeite : process(clk)
	begin  
            if(clk'EVENT and clk = '1' and rst = '1') then
                Q<=(others=>'0');
            elsif(clk'EVENT and clk = '1' and en = '1') then
                Q<=D;
            end if;
        end process;
	
end behavioral;
