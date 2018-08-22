library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.signExtend;

entity signExtend_tb is
end signExtend_tb;

architecture behavioral of signExtend_tb is

	constant INPUT_WIDTH : integer := 4;
	constant OUTPUT_WIDTH : integer := 8;
	constant NUM_TESTCASES : integer := 4;
	signal number : signed(INPUT_WIDTH - 1 downto 0);
	signal signExtNumber, signExtNumber_ref : signed(OUTPUT_WIDTH - 1 downto 0);
	
	type input_array_type is array (0 to NUM_TESTCASES - 1) of signed(INPUT_WIDTH - 1 downto 0);
	constant input_array : input_array_type :=
		("0000", "0111", "1000", "1111");
	
begin

    SIGN_EXTEND: entity work.signExtend(behavioral)
	generic map(INPUT_WIDTH => INPUT_WIDTH, OUTPUT_WIDTH => OUTPUT_WIDTH)
	port map(number => number, signExtNumber => signExtNumber);
	
	SIGN_EXTEND_REF: entity ROrgPrSimLib.signExtend(behavioral)
	generic map(INPUT_WIDTH => INPUT_WIDTH, OUTPUT_WIDTH => OUTPUT_WIDTH)
	port map(number => number, signExtNumber => signExtNumber_ref);


    TESTBENCH: process
        variable points : integer := 0;
        variable numErrors : integer := 0;
        variable l : line;
    begin
        
        for i in 0 to NUM_TESTCASES - 1 loop

			number <= input_array(i);

			wait for 5 ns;

			if signExtNumber /= signExtNumber_ref then
			    write(l, string'(time'image(now) & ": falsches Ergebnis am signExtend-Modul: """));
			    write(l, std_logic_vector(signExtNumber));
			    write(l, string'(""" (erwartet : """));
			    write(l, std_logic_vector(signExtNumber_ref));
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
	        report "Die Vorzeichenerweiterung funktioniert einwandfrei! (signExtend: 2/2 Punkte) (Achtung: unvollstaendiger Test)" severity note;
	    else
	        report "Die Vorzeichenerweiterung funktioniert nicht einwandfrei! (signExtend: "
	        & integer'image(points) & "/2 Punkte)" severity failure;
	    end if;
	    
	    wait;
    end process;

end behavioral;
