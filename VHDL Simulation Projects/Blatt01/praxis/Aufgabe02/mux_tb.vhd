library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.mux2;

entity mux_tb is
end mux_tb;

architecture behavioral of mux_tb is

	signal a, b : std_logic;
	signal s : std_logic;
	signal y, y_ref : std_logic;
	
begin

	MUX: entity work.mux2(behavioral)
	port map(a => a, b => b, s => s, y => y);

	MUX_REF: entity ROrgPrSimLib.mux2(behavioral)
	port map(a => a, b => b, s => s, y => y_ref);
	
	process
		variable tmp_i : std_logic_vector(2 downto 0);
		variable numErrors : integer := 0;
	begin

		for i in 0 to 7 loop

			tmp_i := std_logic_vector(to_unsigned(i, 3));
			a <= tmp_i(0);
			b <= tmp_i(1);
			s <= tmp_i(2);

			wait for 5 ns;

			if y /= y_ref
				then report "falsches Ergebnis am Multiplexer: " & std_logic'image(y) & " (erwartet: " & std_logic'image(y_ref) & ")" severity error;
				numErrors := numErrors + 1;
			end if;

			wait for 5 ns;

		end loop;

		if numErrors = 0 then
			report "Der Multiplexer funktioniert einwandfrei!" severity note;
		else
			report "Der Multiplexer ist fehlerhaft!" severity failure;
		end if;

		wait;

	end process;	

end behavioral;
