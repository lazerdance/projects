library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity leftShifter is
	generic(
		WIDTH 			: integer;
		SHIFT_AMOUNT 	: integer
	);
	port(
		number 			: in std_logic_vector(WIDTH - 1 downto 0);
		shiftedNumber 	: out std_logic_vector(WIDTH - 1 downto 0)
	);
end leftShifter;

architecture behavioral of leftShifter is
begin

	-- Beschreibung hier einfügen
	shifter:process(number)
	begin
            for i in 0 to WIDTH-1 loop
                if (i>SHIFT_AMOUNT-1) then
                    shiftedNumber(i)<=number(i-SHIFT_AMOUNT);
                else
                    shiftedNumber(i)<='0';
                end if;
            end loop;
        end process;
	
end behavioral;
