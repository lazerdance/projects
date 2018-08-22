library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.alu_1bit;

entity alu_1bit_tb is
	generic (
		carrySelect : boolean := true
	);
end alu_1bit_tb;

architecture behavioral of alu_1bit_tb is

	signal operation : std_logic_vector(1 downto 0);
	signal a, b : std_logic;
	signal aInvert, bInvert : std_logic;
	signal less : std_logic;
	signal carryIn, carryOut_ref, carryOut_uut : std_logic;
	signal result_ref, result_uut : std_logic;
	signal set_ref, set_uut : std_logic;
	
	type stdLogicConvArray_type is array (0 to 3) of std_logic;
	constant stdLogicConvArray : stdLogicConvArray_type := ('0', '1', 'U', 'X');
	
begin

	REF: entity ROrgPrSimLib.alu_1bit(structural)
	    generic map(carrySelect => carrySelect)
		port map(operation => operation,
				 a => a,
				 aInvert => aInvert,
				 b => b,
				 bInvert => bInvert,
				 carryIn => carryIn,
				 less => less,
				 result => result_ref,
				 carryOut => carryOut_ref,
				 set => set_ref);
	
	UUT: entity work.alu_1bit(structural)
	    generic map(carrySelect => carrySelect)
		port map(operation => operation,
				 a => a,
				 aInvert => aInvert,
				 b => b,
				 bInvert => bInvert,
				 carryIn => carryIn,
				 less => less,
				 result => result_uut,
				 carryOut => carryOut_uut,
				 set => set_uut);
	
	TESTBENCH: process
		variable points, numErrors, numResultErrors, numCarryErrors, numSetErrors, numErrorPropErrors : integer := 0;
        variable l : line;
		variable tmp : std_logic_vector(15 downto 0);
		variable errorPropagationTestcase : boolean;
		variable resultError, carryError, setError : boolean;
	begin
		for i in 0 to 65535 loop
			tmp := std_logic_vector(to_unsigned(i, 16));
			operation(1) <= stdLogicConvArray(to_integer(unsigned(tmp(15 downto 14))));
			operation(0) <= stdLogicConvArray(to_integer(unsigned(tmp(13 downto 12))));
			a <= stdLogicConvArray(to_integer(unsigned(tmp(11 downto 10))));
			aInvert <= stdLogicConvArray(to_integer(unsigned(tmp(9 downto 8))));
			b <= stdLogicConvArray(to_integer(unsigned(tmp(7 downto 6))));
			bInvert <= stdLogicConvArray(to_integer(unsigned(tmp(5 downto 4))));
			carryIn <= stdLogicConvArray(to_integer(unsigned(tmp(3 downto 2))));
			less <= stdLogicConvArray(to_integer(unsigned(tmp(1 downto 0))));
			
			errorPropagationTestcase :=
				(tmp(15) = '1' or tmp(13) = '1') or -- invalid operation
				(tmp(11) = '1' or tmp(9) = '1' or tmp(7) = '1' or tmp(5) = '1') or -- invalid a and b (always used for carry)
				(tmp(3) = '1') or -- invalid carryIn (always used)
				(operation = "11" and tmp(1) = '1'); -- invalid less for the corresponding operation
			
			wait for 5 ns;
			
			resultError := result_uut /= result_ref;
			carryError := carryOut_uut /= carryOut_ref;
			setError := set_uut /= set_ref;
			
			if resultError or carryError or setError then
				if numErrors < 4 then
					write(l, time'image(now));
					write(l, string'(": Falsches Ergebnis an der 1-Bit-ALU:"));
					writeline(OUTPUT, l);
				elsif numErrors = 4 then
					write(l, time'image(now));
					write(l, string'(": Weitere Fehler werden nicht angezeigt ..."));
					writeline(OUTPUT, l);
				end if;
				
				if resultError then
					if numErrors < 4 then
						write(l, string'("    result = '"));
						write(l, result_uut);
						write(l, string'("' (erwartet: '"));
						write(l, result_ref);
						write(l, string'("')"));
						writeline(OUTPUT, l);
					end if;
					if not errorPropagationTestcase then
						numResultErrors := numResultErrors + 1;
					end if;
				end if;
				
				if carryError then
					if numErrors < 4 then
						write(l, string'("    carryOut = '"));
						write(l, carryOut_uut);
						write(l, string'("' (erwartet: '"));
						write(l, carryOut_ref);
						write(l, string'("')"));
						writeline(OUTPUT, l);
					end if;
					if not errorPropagationTestcase then
						numCarryErrors := numCarryErrors + 1;
					end if;
				end if;
				
				if setError then
					if numErrors < 4 then
						write(l, string'("    set = '"));
						write(l, set_uut);
						write(l, string'("' (erwartet: '"));
						write(l, set_ref);
						write(l, string'("')"));
						writeline(OUTPUT, l);
					end if;
					if not errorPropagationTestcase then
						numSetErrors := numSetErrors + 1;
					end if;
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
		
		if numCarryErrors = 0 then
			write(l, string'("  ALU-carryOut:            fehlerfrei"));
			points := points + 1;
		else
			write(l, string'("  ALU-carryOut:            fehlerhaft (" & integer'image(numCarryErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numSetErrors = 0 then
			write(l, string'("  ALU-set:                 fehlerfrei"));
			points := points + 1;
		else
			write(l, string'("  ALU-set:                 fehlerhaft (" & integer'image(numCarryErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numErrorPropErrors = 0 then
			write(l, string'("  ALU-Fehlerfortpflanzung: fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  ALU-Fehlerfortpflanzung: fehlerhaft (" & integer'image(numErrorPropErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
        if numErrors = 0 then
	        report "Die 1-Bit-ALU funktioniert einwandfrei! (alu_1bit: 3.0/3 Punkte)" severity note;
	    else
	        report "Die 1-Bit-ALU ist fehlerhaft! (alu_1bit: " & integer'image(points/2) & "." & integer'image(5*(points mod 2)) & "/3 Punkte)" severity failure;
	    end if;
	    
		wait;
	end process;
	
end behavioral;
