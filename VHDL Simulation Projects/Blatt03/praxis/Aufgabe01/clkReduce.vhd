library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clkReduce is
	generic( divisor : integer := 1 );
	port( clk_in  : in  std_logic;
	      clk_out : out std_logic );
end clkReduce;

architecture behavioral of clkReduce is
begin
    clock: process (clk_in)
    variable counter: integer := 0;
    variable zustand: integer := 0;
        begin
        counter := counter + 1;
        if (counter = divisor) then
            if (zustand = 0) then
                clk_out <= '0';
                zustand := 1;
            else
                clk_out <= '1';
                zustand := 0;
            end if;
            counter := 0;
        end if;    
    end process;

end behavioral;
