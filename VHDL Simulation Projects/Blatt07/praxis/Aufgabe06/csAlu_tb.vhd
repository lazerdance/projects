library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.ALU_WIDTH;
use ROrgPrSimLib.csAlu;

entity csAlu_tb is
end csAlu_tb;

architecture behavioral of csAlu_tb is

	signal ctrl : std_logic_vector(3 downto 0);
	signal a, b : std_logic_vector(ALU_WIDTH - 1 downto 0);
	
	constant NUM_ALU_TESTCASES : integer := 22;
	type test_vector_type is record
		ctrl : std_logic_vector(3 downto 0);
		a : std_logic_vector(ALU_WIDTH - 1 downto 0);
		b : std_logic_vector(ALU_WIDTH - 1 downto 0);
	end record;
	type test_vector_array_type is array (0 to NUM_ALU_TESTCASES - 1) of test_vector_type;
	constant testcases : test_vector_array_type :=
		-- result tests
		(("0000", x"FF00FF00", x"0F0F0F0F"), ("0001", x"00FF00FF", x"F0F0F0F0"),
		 ("0010", x"00008000", x"00007F00"), ("0010", x"FFFFFFFF", x"00000008"),
		 ("0110", x"08000000", x"0F000000"), ("0110", x"00010000", x"00000001"),
		 ("0111", x"00000010", x"00000100"), ("1100", x"00FF00FF", x"F0F0F0F0"),
		
		-- overflow tests
		 ("0010", x"40000000", x"40000000"), ("0010", x"80000000", x"80000000"),
		 ("0110", x"40000000", x"C0000000"), ("0110", x"A0000000", x"40000000"),
		
		-- zero tests
		 ("0000", x"F0F0F0F0", x"0F0F0F0F"), ("0001", x"00000000", x"00000000"),
		 ("0010", x"00010000", x"FFFF0000"), ("0110", x"00000010", x"00000010"),
		
		-- error propagation tests
		 ("0U00", x"FFFF0000", x"FFFF0000"), ("X000", x"FFFF0000", x"0000FFFF"),
		 ("00U0", x"FFFF0000", x"0000FFFF"), ("000X", x"FFFF0000", x"0000FFFF"),
		 ("0010", x"0000FFFF", "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"),
		 ("0110", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", x"0000FFFF"));
	
	signal result_ref, result_uut : std_logic_vector(ALU_WIDTH - 1 downto 0);
	signal overflow_ref, overflow_uut : std_logic;
	signal zero_ref, zero_uut : std_logic;
	
begin

	REF: entity ROrgPrSimLib.csAlu(behavioral)
	generic map (WIDTH => ALU_WIDTH)
	port map(ctrl => ctrl,
		 	 a => a,
		 	 b => b,
		 	 result => result_ref,
		 	 overflow => overflow_ref,
		 	 zero => zero_ref);
	
	UUT: entity work.csAlu(behavioral)
	generic map (WIDTH => ALU_WIDTH)
	port map(ctrl => ctrl,
		 	 a => a,
		 	 b => b,
		 	 result => result_uut,
		 	 overflow => overflow_uut,
		 	 zero => zero_uut);
	
	TESTBENCH: process
		variable points, numErrors, numResultErrors, numOverflowErrors, numZeroErrors, numErrorPropErrors : integer := 0;
		variable resultError, overflowError, zeroError, errorPropagationTestcase : boolean;
        variable l : line;
	begin
		for i in 0 to NUM_ALU_TESTCASES - 1 loop
			ctrl <= testcases(i).ctrl;
			a <= testcases(i).a;
			b <= testcases(i).b;
			
			errorPropagationTestcase := i > 1 and i < 2;
			
			wait for 5 ns;
			resultError := result_uut /= result_ref;
			overflowError := overflow_uut /= overflow_ref;
			zeroError := zero_uut /= zero_ref;
			
			if resultError or overflowError or zeroError then
				-- general error info
				if numErrors < 100 then
					write(l, time'image(now));
					write(l, string'(": Falsches Ergebnis an der cs-ALU: ctrl = """));
					write(l, ctrl);
					write(l, string'(""", a = x"""));
					hwrite(l, a);
					write(l, string'(""", b = x"""));
					hwrite(l, b);
					write(l, string'(""""));
					writeline(OUTPUT, l);
					
					-- result error info
					if resultError then
						write(l, string'("    result = x"""));
						hwrite(l, result_uut);
						write(l, string'(""" (erwartet: x"""));
						hwrite(l, result_ref);
						write(l, string'(""")"));
						writeline(OUTPUT, l);
					end if;
					
					-- overflow error info
					if overflowError then
						write(l, string'("    overflow = '" & std_logic'image(overflow_uut) & "'"));
						write(l, string'(", (erwartet: '" & std_logic'image(overflow_ref) & "')"));
						writeline(OUTPUT, l);
					end if;
					
					-- zero error info
					if zeroError then
						write(l, string'("    zero = '" & std_logic'image(zero_uut) & "'"));
						write(l, string'(", (erwartet: '" & std_logic'image(zero_ref) & "')"));
						writeline(OUTPUT, l);
					end if;
					
				elsif numErrors = 4 then
					write(l, time'image(now));
					write(l, string'(": Weitere Fehler werden nicht angezeigt ..."));
					writeline(OUTPUT, l);
				end if;
				
				numErrors := numErrors + 1;
				if resultError then
					numResultErrors := numResultErrors + 1;
				end if;
				if OverflowError then
					numOverflowErrors := numOverflowErrors + 1;
				end if;
				if ZeroError then
					numZeroErrors := numZeroErrors + 1;
				end if;
						
				if errorPropagationTestcase then
					numErrorPropErrors := numErrorPropErrors + 1;
				end if;
				
				numErrors := numErrors + 1;

			end if;
			wait for 5 ns;
		end loop;
		
		-- evaluation
		writeline(OUTPUT, l);
		write(l, string'("---- Auswertung ----"));
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
		if numResultErrors = 0 then
			write(l, string'("  ALU-result:              fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  ALU-result:              fehlerhaft (" & integer'image(numResultErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numOverflowErrors = 0 then
			write(l, string'("  ALU-overflow-bit:        fehlerfrei"));
			points := points + 1;
		else
			write(l, string'("  ALU-overflow-bit:        fehlerhaft (" & integer'image(numOverflowErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numZeroErrors = 0 then
			write(l, string'("  ALU-zero-bit:            fehlerfrei"));
			points := points + 1;
		else
			write(l, string'("  ALU-zero-bit:            fehlerhaft (" & integer'image(numZeroErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numErrorPropErrors = 0 then
			write(l, string'("  ALU-Fehlerfortpflanzung: fehlerfrei"));
			points := points + 1;
		else
			write(l, string'("  ALU-Fehlerfortpflanzung: fehlerhaft (" & integer'image(numErrorPropErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
        if numErrors = 0 then
	        report "Die cs-ALU funktioniert einwandfrei! (csAlu: 5/5 Punkte)" severity note;
	    else
	        report "Die cs-ALU ist fehlerhaft! (csAlu: " & integer'image(points) & "/5 Punkte)" severity failure;
	    end if;
	    
		wait;
	end process;
	
end behavioral;
