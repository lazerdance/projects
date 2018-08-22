library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.leftShifter;

entity leftShifter_tb is
end leftShifter_tb;

architecture behavioral of leftShifter_tb is

	constant WIDTH : integer := 8;
	constant NUM_TESTCASES : integer := 4;
	signal number : std_logic_vector(WIDTH - 1 downto 0);
	signal shiftedNumber_tmp, shiftedNumber_tmp_ref : std_logic_vector(WIDTH - 1 downto 0);
	signal shiftedNumber, shiftedNumber_ref : std_logic_vector(WIDTH - 1 downto 0);
	
	type input_number_array_type is array (0 to NUM_TESTCASES - 1) of std_logic_vector(WIDTH - 1 downto 0);
	constant input_number_array : input_number_array_type :=
		("00000000", "00000001", "10001000", "10101010");
	
begin

    LEFT_SHIFT_1: entity work.leftShifter(behavioral)
	generic map(WIDTH => WIDTH,
	            SHIFT_AMOUNT => 1)
	port map(number => number,
			 shiftedNumber => shiftedNumber_tmp);
	
	LEFT_SHIFT_4: entity work.leftShifter(behavioral)
	generic map(WIDTH => WIDTH,
	            SHIFT_AMOUNT => 4)
	port map(number => shiftedNumber_tmp,
			 shiftedNumber => shiftedNumber);

	LEFT_SHIFT_1_REF: entity ROrgPrSimLib.leftShifter(behavioral)
	generic map(WIDTH => WIDTH,
	            SHIFT_AMOUNT => 1)
	port map(number => number,
			 shiftedNumber => shiftedNumber_tmp_ref);
	
	LEFT_SHIFT_4_REF: entity ROrgPrSimLib.leftShifter(behavioral)
	generic map(WIDTH => WIDTH,
	            SHIFT_AMOUNT => 4)
	port map(number => shiftedNumber_tmp_ref,
			 shiftedNumber => shiftedNumber_ref);
	
	
    TESTBENCH: process
        variable points : integer := 0;
        variable numErrors : integer := 0;
        variable l : line;
    begin
        
        for i in 0 to NUM_TESTCASES - 1 loop

			number <= input_number_array(i);

			wait for 5 ns;

			if shiftedNumber /= shiftedNumber_ref then
			    write(l, string'(time'image(now) & ": falsches Ergebnis am leftShifter-Modul: """));
			    write(l, shiftedNumber);
			    write(l, string'(""" (erwartet: """));
			    write(l, shiftedNumber_ref);
			    write(l, string'(""")"));
			    writeline(OUTPUT, l);
				numErrors := numErrors + 1;
			end if;

			wait for 5 ns;

		end loop;

        if numErrors < 2 then
            points := 2 - numErrors;
		else
			points := 0;
        end if;
        
        
        -- evaluation
        if numErrors = 0 then
	        report "Der Shifter funktioniert einwandfrei! (leftShifter: 2/2 Punkte) (Achtung: unvollstaendiger Test)" severity note;
	    else
	        report "Der Shifter funktioniert nicht einwandfrei! (leftShifter: "
	        & integer'image(points) & "/2 Punkte)" severity failure;
	    end if;
	    
	    wait;
    end process;

end behavioral;
