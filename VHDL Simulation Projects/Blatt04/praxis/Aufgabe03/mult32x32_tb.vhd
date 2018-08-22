library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.mult32x32;
 
entity mult32x32_tb is
end mult32x32_tb;
 
architecture behavioral of mult32x32_tb is 

	signal a : std_logic_vector(31 downto 0);
	signal b : std_logic_vector(31 downto 0);
	signal y, y_ref : std_logic_vector(63 downto 0);
	constant NUM_TESTCASES : integer := 8;

	type test_vector_array_type is array (0 to NUM_TESTCASES - 1) of integer;
	constant test_vector_array_a : test_vector_array_type :=
		(3,   0, 42,    13, -1000, -2147483648, 12345678, -87654321);
	constant test_vector_array_b : test_vector_array_type :=
		(7, -37,  0, -1988,    69,  2147483647, 87654321, -12345678);

begin

	MULT_32X32: entity work.mult32x32(structural)
	port map(a => a,
			 b => b,
			 y => y);

	MULT_32X32_REF: entity ROrgPrSimLib.mult32x32(structural)
	port map(a => a,
			 b => b,
			 y => y_ref);
	
	
    TESTBENCH: process
        variable points : integer := 0;
        variable numErrors : integer := 0;
        variable l : line;
        
        procedure printSigned64(num : signed(63 downto 0)) is
          variable shift : signed(63 downto 0) := abs(num);
          variable i : integer := 1;
          variable output : string(21 downto 1);
        begin
          for i in 0 to 63 loop
            if (num(i) /= '1') and (num(i) /= '0') then
              write(l, "< Unprintable Number, containing " & std_logic'Image(num(i)) & " at bit " & integer'Image(i) & " >");
              return;
            end if;
          end loop;
          if shift = (63 downto 0 => '0') then
            write(l, 0);
          else
            while shift /= (63 downto 0 => '0') loop
              output(i) := Integer'image(to_integer(shift mod 10))(1);
              shift := shift / 10;
              i := i+1;
            end loop;
          
            if num < 0 then
              output(i) := '-';
              i := i+1;
            end if;
          
            for j in i downto 1 loop
              write(l, output(j));
            end loop;
          end if;
        end procedure printSigned64;
    
    begin
        
        for i in 0 to NUM_TESTCASES - 1 loop

			a <= std_logic_vector(to_signed(test_vector_array_a(i), 32));
			b <= std_logic_vector(to_signed(test_vector_array_b(i), 32));

			wait for 5 ns;

			if y /= y_ref then
			    write(l, string'(time'image(now) & ": falsches Ergebnis am Multiplizierer: """));
			    printSigned64(signed(y));
			    write(l, string'(""" (erwartet : """));
			    printSigned64(signed(y_ref));
			    write(l, string'(""")"));
			    writeline(OUTPUT, l);
			    numErrors := numErrors + 1;
			end if;

			wait for 5 ns;

		end loop;

        if numErrors < 6 then
            points := 6 - numErrors;
		else
			points := 0;
        end if;
        
        
        -- evaluation
        if numErrors = 0 then
	        report "Der Multiplizierer funktioniert einwandfrei! (mult32x32: 6/6 Punkte) (Achtung: unvollstaendiger Test)" severity note;
	    else
	        report "Der Multiplizierer funktioniert nicht einwandfrei! (mult32x32: "
	        & integer'image(points) & "/6 Punkte)" severity failure;
	    end if;
	    
	    wait;
    end process;

end behavioral;
