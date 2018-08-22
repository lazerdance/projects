library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.mipsISA.all;
use ROrgPrSimLib.mipsCtrl;

entity mipsCtrl_tb is
end mipsCtrl_tb;

architecture behavioral of mipsCtrl_tb is

	signal op : std_logic_vector(5 downto 0);
	signal outputSignals_uut, outputSignals_ref : std_logic_vector(8 downto 0);
	constant REG_DST_IDX : integer := 8;
	constant BRANCH_IDX : integer := 7;
	constant MEM_READ_IDX : integer := 6;
	constant MEM_TO_REG_IDX : integer := 5;
	-- constant ALU_OP_RANGE : range := 4 downto 3;
	constant MEM_WRITE_IDX : integer := 2;
	constant ALU_SRC_IDX : integer := 1;
	constant REG_WRITE_IDX : integer := 0;
	
	type output_signal_identifiers_type is array (8 downto 0) of string(1 to 8);
	constant output_signal_identifiers : output_signal_identifiers_type :=
		("regDst  ", "branch  ", "memRead ", "memToReg", "aluOp   ", "aluOp   ", "memWrite", "aluSrc  ", "regWrite");
	
	constant NUM_TESTCASES : integer := 5;
	type test_op_array_type is array (0 to NUM_TESTCASES - 1) of std_logic_vector(5 downto 0);
	constant test_op_array : test_op_array_type :=
		(R_FORMAT_OPCODE, LW_OPCODE, SW_OPCODE, BEQ_OPCODE, J_OPCODE);
	
begin

	REF: entity ROrgPrSimLib.mipsCtrl(structural)
		port map(op => op,
				 regDst => outputSignals_ref(REG_DST_IDX),
				 branch => outputSignals_ref(BRANCH_IDX),
				 memRead => outputSignals_ref(MEM_READ_IDX),
				 memToReg => outputSignals_ref(MEM_TO_REG_IDX),
				 aluOp => outputSignals_ref(4 downto 3),
				 memWrite => outputSignals_ref(MEM_WRITE_IDX),
				 aluSrc => outputSignals_ref(ALU_SRC_IDX),
				 regWrite => outputSignals_ref(REG_WRITE_IDX));
				 
	UUT: entity work.mipsCtrl(structural)
		port map(op => op,
				 regDst => outputSignals_uut(REG_DST_IDX),
				 branch => outputSignals_uut(BRANCH_IDX),
				 memRead => outputSignals_uut(MEM_READ_IDX),
				 memToReg => outputSignals_uut(MEM_TO_REG_IDX),
				 aluOp => outputSignals_uut(4 downto 3),
				 memWrite => outputSignals_uut(MEM_WRITE_IDX),
				 aluSrc => outputSignals_uut(ALU_SRC_IDX),
				 regWrite => outputSignals_uut(REG_WRITE_IDX));
	
	
	
	TESTBENCH: process
		variable numErrors : integer := 0;
        variable l : line;
	begin
		for i in 0 to NUM_TESTCASES - 1 loop
			op <= test_op_array(i);
			wait for 5 ns;
			
			if outputSignals_uut /= outputSignals_ref then
				write(l, time'image(now));
				write(l, string'(": Falsches Ergebnis am MipsCtrl-Modul: op = """));
				write(l, op);
				write(l, string'(""""));
				writeline(OUTPUT, l);
				numErrors := numErrors + 1;
				
				for n in outputSignals_ref'HIGH downto outputSignals_ref'LOW loop
					if n = 4 then
						write(l, string'("    aluOp    = """));
						write(l, outputSignals_uut(n downto n-1));
						write(l, string'(""" (erwartet: """));
						write(l, outputSignals_ref(n downto n-1));
						write(l, string'(""")"));
						writeline(OUTPUT, l);
					elsif n = 3 then
					
					else
						write(l, string'("    " & output_signal_identifiers(n) & " = '"));
						write(l, outputSignals_uut(n));
						write(l, string'("'  (erwartet:  '"));
						write(l, outputSignals_ref(n));
						write(l, string'("')"));
						writeline(OUTPUT, l);
					end if;
				end loop;
			end if;
			wait for 5 ns;
		end loop;
	
		-- evaluation
		writeline(OUTPUT, l);
        if numErrors = 0 then
	        report "Das MipsCtrl-Modul funktioniert einwandfrei! (Punktzahl: 2/2)" severity note;
	    else
	        report "Das MipsCtrl-Modul ist fehlerhaft! (Punktzahl: 0/2)" severity failure;
	    end if;
	    
		wait;
	end process;
	
end behavioral;
