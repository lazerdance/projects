library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.and2;
use ROrgPrSimLib.xor2;
use ROrgPrSimLib.xnor2;

entity logic_tb is
end logic_tb;

use work.all;

architecture behavioral of logic_tb is

	signal a, b : std_logic;
	signal and_res, and_ref : std_logic;
	signal xor_res, xor_ref : std_logic;
	signal xnor_res, xnor_ref : std_logic;

	component and2 is
		port(
			a, b : in  std_logic;
			y    : out std_logic
		);
	end component;

	component xor2 is
		port(
			a, b : in  std_logic;
			y    : out std_logic
		);
	end component;

	component xnor2 is
		port(
			a, b : in  std_logic;
			y    : out std_logic
		);
	end component;

begin

	AND2_VHD: and2 port map(a => a, b => b, y => and_res);

	AND_GATE_REF: entity ROrgPrSimLib.and2(behavioral)
	port map(a => a, b => b, y => and_ref);
	
	XOR2_VHD: xor2 port map(a => a, b => b, y => xor_res);
	
	XOR_GATE_REF: entity ROrgPrSimLib.xor2(behavioral)
	port map(a => a, b => b, y => xor_ref);
	
	XNOR2_VHD: xnor2 port map(a => a, b => b, y => xnor_res);

	XNOR_GATE_REF: entity ROrgPrSimLib.xnor2(behavioral)
	port map(a => a, b => b, y => xnor_ref);
	
	process
		variable tmp_i : std_logic_vector(1 downto 0);
		variable numErrors : integer := 0;
	begin

		for i in 0 to 3 loop
			
			tmp_i := std_logic_vector(to_unsigned(i, 2));
			a <= tmp_i(0);
			b <= tmp_i(1);

			wait for 5 ns;

			if and_res /= and_ref then
				report "falsches Ergebnis am and-Gatter: " & std_logic'image(and_res) & " (erwartet: " & std_logic'image(and_ref) & ")" severity error;
				numErrors := numErrors + 1;
			end if;

			if xor_res /= xor_ref then
				report "falsches Ergebnis am xor-Gatter: " & std_logic'image(xor_res) & " (erwartet: " & std_logic'image(xor_ref) & ")" severity error;
				numErrors := numErrors + 1;
			end if;

			if xnor_res /= xnor_ref then
				report "falsches Ergebnis am xnor-Gatter: " & std_logic'image(xnor_res) & " (erwartet: " & std_logic'image(xnor_ref) & ")" severity error;
				numErrors := numErrors + 1;
			end if;

			wait for 5 ns;

		end loop;

		if numErrors = 0 then
			report "Alle Gatter funktionieren einwandfrei!" severity note;
		else
			report "Nicht alle Gatter funktionieren einwandfrei!" severity failure;
		end if;

		wait;

	end process;	

end behavioral;
