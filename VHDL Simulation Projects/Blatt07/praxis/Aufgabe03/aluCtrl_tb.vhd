-- benötigte Bibliotheken hier einfügen
library std;
use std.textio.all;

library IEEE;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library RorgPrSimLib;
use RorgPrSimLib.all;

library work;
use work.all;

entity aluCtrl_tb is
end aluCtrl_tb;

architecture behavioral of aluCtrl_tb is
signal test_f: 		std_logic_vector(5 downto 0);
signal test_aluOp:	std_logic_vector(1 downto 0);
signal test_operation:	std_logic_vector(3 downto 0);

type test_array_f is array (3 downto 0) of std_logic_vector (5 downto 0);
type test_array_aluOp is array (3 downto 0) of std_logic_vector (1 downto 0);
type test_result is array (3 downto 0) of std_logic_vector (3 downto 0);
signal t_f : test_array_f;
signal t_alu : test_array_aluOp;
signal t_result : test_result;

begin

    -- ALU-Ctrl-Testbench hier implementieren
	dut: entity work.aluCtrl(behavioral)
		port map(
			f => test_f,
			aluOp => test_aluOp,
			operation => test_operation
			);

	t_f(0) <= "000000";
	t_f(1) <= "000001";
	t_f(2) <= "000011";
	t_f(3) <= "000101";

	t_alu(0) <= "00";
	t_alu(1) <= "01";
	t_alu(2) <= "10";
	t_alu(3) <= "11";

	t_result(0) <= "0010";
	t_result(1) <= "0110";
	t_result(2) <= "0111";
	t_result(3) <= "0101";
    
	testbench : process

	variable error_count: integer := 0;
	variable l : line;
	
	begin
		for i in 0 to 3 loop	
			test_f <= t_f(i);
			wait for 5 ns;
			test_aluOp <= t_alu(i);
			wait for 5 ns;
			if (test_operation /= t_result(i)) then
				write(l, time'image(now));
				write(l, string'(": Falsches Ergebnis am Ausgang: "));
				write(l, test_operation);
				write(l, string'(", erwartet: "));
				write(l, t_result(i));
				write(l, string'("."));
				writeline(OUTPUT, l);
				error_count := error_count + 1;  
			end if;
		end loop;
		if (error_count > 0) then
			report "Die AluCtrl funktioniert nicht einwandfrei! (Punkte: " & integer'image(4 - error_count) & "/4)." severity error;
		else 
			report "Die AluCtrl funktioniert einwandfrei! (Punkte: 4/4).";
		end if;
	wait;
	end process;

end behavioral;
