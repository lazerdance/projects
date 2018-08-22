library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity signExtend is
	generic(
		INPUT_WIDTH 	: integer;
		OUTPUT_WIDTH 	: integer
	);
	port(
		number 			: in signed(INPUT_WIDTH - 1 downto 0);
		signExtNumber 	: out signed(OUTPUT_WIDTH - 1 downto 0)
	);
end signExtend;

architecture behavioral of signExtend is
begin
	extend: process	(number)
	begin
		for i in 0 to OUTPUT_WIDTH-1 loop
                        if (i < INPUT_WIDTH) then
                                signExtNumber(i) <= number(i);
                        elsif (number(INPUT_WIDTH-1) = '0') then
                                signExtNumber(i) <= '0';
                        else
                                signExtNumber(i) <= '1';
                        end if;
		end loop;
	end process;
end behavioral;
