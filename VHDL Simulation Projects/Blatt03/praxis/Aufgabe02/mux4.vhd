library ieee;
use ieee.std_logic_1164.all;

entity mux4 is
    generic(DATA_WIDTH : integer := 32);
	port( a, b, c, d : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
	      sel        : in  std_logic_vector(1 downto 0);
	      y          : out std_logic_vector(DATA_WIDTH - 1 downto 0) );
end mux4;

architecture behavioral of mux4 is
begin

	-- Multiplexer-Beschreibung hier einfügen
	y <= a when sel = "00" else
        b when sel = "01" else
        c when sel = "10" else
        d when sel = "11" else
        (others => 'X');

end behavioral;
