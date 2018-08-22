library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;
use ROrgPrSimLib.mipsISA.all;
use ROrgPrSimLib.mipsCpu;

entity mipsCpu_tb is
end mipsCpu_tb;

-- Punkte
-- MIPS Ctrl 2
-- instr insert 1
-- RAM insert 1
-- loads 2
-- stores 2
-- ALU instr 4
-- branch 2
-- pc_7seg 2
-- full program 4

architecture behavioral of mipsCpu_tb is

	constant PROG_FILE_NAME : string := "mul_prog.mif";
	constant DATA_FILE_NAME : string := "mul_data.mif";
	
	signal sim_stop : boolean := false;
	
	signal clk : std_logic := '0';
	signal rst : std_logic := '0';
	
	signal testMode : std_logic := '0';
	signal testInstruction : std_logic_vector(31 downto 0);
	
	signal ramInsertMode : std_logic := '0';
	signal ramWriteEn : std_logic;
	signal ramWriteAddr : std_logic_vector(LOG2_NUM_RAM_ELEMENTS - 1 downto 0);
	signal ramWriteData : std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0);
	signal ramElements_uut, ramElements_ref : ram_elements_type;
	
	signal registers_uut, registers_ref : reg_vector_type;
	signal pc_next_uut, pc_next_ref : std_logic_vector(PC_WIDTH - 1 downto 0);
	signal pc7SegDigits_uut, pc7SegDigits_ref : pc_7seg_digits_type;
	
	-- RAM insert
	constant NUM_RAM_INSERTIONS : integer := 4;
	type ram_insert_array_entry is record 
		writeAddr : std_logic_vector(LOG2_NUM_RAM_ELEMENTS - 1 downto 0);
		writeData : std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0);
	end record;
	type ram_insert_array_type is array (0 to NUM_RAM_INSERTIONS - 1) of ram_insert_array_entry;
	constant ram_insertions : ram_insert_array_type :=
		(("1000000000", x"00000001"),
		 ("1000000001", x"00000004"),
		 ("1000000010", x"00000010"),
		 ("1000000011", x"00000040"));
	
	-- testcases
	constant NUM_LW_TESTCASES : integer := 4;
	type lw_instr_array_type is array (0 to NUM_LW_TESTCASES - 1) of std_logic_vector(31 downto 0);
	constant lw_test_instructions : lw_instr_array_type :=
		(LW_OPCODE & "00000" & "00001" & x"0800",
		 LW_OPCODE & "00000" & "00010" & x"0804",
		 LW_OPCODE & "00000" & "00011" & x"0808",
		 LW_OPCODE & "00000" & "00100" & x"080C");
	
	constant NUM_RF_TESTCASES : integer := 10;
	type rf_instr_array_type is array (0 to NUM_RF_TESTCASES - 1) of std_logic_vector(31 downto 0);
	constant rf_test_instructions : rf_instr_array_type :=
		(R_FORMAT_OPCODE & "00011" & "00001" & "01110" & "00000" & SUB_FUNCT, -- REG[14] = 16 - 1
		 R_FORMAT_OPCODE & "00011" & "00100" & "01111" & "00000" & SUB_FUNCT, -- REG[15] = 16 - 64
		 R_FORMAT_OPCODE & "00010" & "00100" & "01100" & "00000" & ADD_FUNCT, -- REG[12] = 4 + 64
		 R_FORMAT_OPCODE & "01111" & "00100" & "01101" & "00000" & ADD_FUNCT, -- REG[13] = -48 + 64
		 R_FORMAT_OPCODE & "01100" & "01110" & "01000" & "00000" & AND_FUNCT, -- REG[ 8] = 68 and 15
		 R_FORMAT_OPCODE & "01110" & "01110" & "01001" & "00000" & AND_FUNCT, -- REG[ 9] = 15 and 15
		 R_FORMAT_OPCODE & "00010" & "00011" & "01010" & "00000" &  OR_FUNCT, -- REG[10] = 4 or 16
		 R_FORMAT_OPCODE & "01010" & "01110" & "01011" & "00000" &  OR_FUNCT, -- REG[11] = 20 or 15
		 R_FORMAT_OPCODE & "00001" & "00010" & "11000" & "00000" & SLT_FUNCT, -- REG[24] = 1 < 16 ? 1 : 0
		 R_FORMAT_OPCODE & "00100" & "00011" & "11100" & "00000" & SLT_FUNCT  -- REG[28] = 64 < 16 ? 1 : 0
		);
	
	constant NUM_BEQ_TESTCASES : integer := 4;
	type beq_instr_array_type is array (0 to NUM_BEQ_TESTCASES - 1) of std_logic_vector(31 downto 0);
	constant beq_test_instructions : lw_instr_array_type :=
		(BEQ_OPCODE & "00010" & "00010" & x"0004",
		 BEQ_OPCODE & "00010" & "00011" & x"fffC",
		 BEQ_OPCODE & "00010" & "00010" & x"0040",
		 BEQ_OPCODE & "00010" & "00011" & x"ffC0");
	
	constant NUM_SW_TESTCASES : integer := 4;
	type sw_instr_array_type is array (0 to NUM_SW_TESTCASES - 1) of std_logic_vector(31 downto 0);
	constant sw_test_instructions : sw_instr_array_type :=
		(SW_OPCODE & "00000" & "00000" & x"0300",
		 SW_OPCODE & "00000" & "00000" & x"0301",
		 SW_OPCODE & "00000" & "00000" & x"0302",
		 SW_OPCODE & "00000" & "00000" & x"0303");
	
	constant NOP : std_logic_vector(31 downto 0) := R_FORMAT_OPCODE & "00000" & "00000" & "00000" & "00000" & AND_FUNCT;
	
begin

	-- clock generation
	CLK_GEN: process
	begin
	    if sim_stop then
	        wait;
	    end if;
		clk <= not clk;
		wait for CLK_PERIOD / 2;
	end process;
	
	-- unit under test
	UUT: entity work.mipsCpu(structural)
	generic map(PROG_FILE_NAME => PROG_FILE_NAME,
				DATA_FILE_NAME => DATA_FILE_NAME)
	port map(clk => clk,
			 rst => rst,
			 testMode_debug => testMode,
			 testInstruction_debug => testInstruction,
			 ramInsertMode_debug => ramInsertMode,
			 ramWriteEn_debug => ramWriteEn,
			 ramWriteAddr_debug => ramWriteAddr,
			 ramWriteData_debug => ramWriteData,
			 ramElements_debug => ramElements_uut,
			 registers_debug => registers_uut,
			 pc_next_debug => pc_next_uut,
			 pc7SegDigits_debug => pc7SegDigits_uut);
	
	-- reference design
	REF: entity ROrgPrSimLib.mipsCpu(structural)
	generic map(PROG_FILE_NAME => PROG_FILE_NAME,
				DATA_FILE_NAME => DATA_FILE_NAME)
	port map(clk => clk,
			 rst => rst,
			 testMode_debug => testMode,
			 testInstruction_debug => testInstruction,
			 ramInsertMode_debug => ramInsertMode,
			 ramWriteEn_debug => ramWriteEn,
			 ramWriteAddr_debug => ramWriteAddr,
			 ramWriteData_debug => ramWriteData,
			 ramElements_debug => ramElements_ref,
			 registers_debug => registers_ref,
			 pc_next_debug => pc_next_ref,
			 pc7SegDigits_debug => pc7SegDigits_ref);
	
	TESTBENCH: process
		variable l : line;
		variable numLwErrors, numSwErrors, numRfErrors, numBeqErrors : integer := 0;
		variable numRamInsertErrors, numRegErrors, num7SegTestcases, num7SegErrors, points : integer := 0;
		variable dstRegIdx : integer;
		variable pc_reg_uut, pc_reg_ref : std_logic_vector(PC_WIDTH - 1 downto 0);
	begin
	
		-- reset
		wait for 2 ns;
		rst <= '1';
		testMode <= '1';
		testInstruction <= NOP;
		wait for 4 * CLK_PERIOD;
		rst <= '0';
		
		-- write some immediate values to data memory
		writeline(OUTPUT, l);
		write(l, time'image(now));
		write(l, string'(": Schreibe Daten in den RAM (per Debug-Port) ..."));
		writeline(OUTPUT, l);
		ramInsertMode <= '1';
		ramWriteEn <= '1';
		for i in 0 to NUM_RAM_INSERTIONS - 1 loop
			ramWriteAddr <= ram_insertions(i).writeAddr;
			ramWriteData <= ram_insertions(i).writeData;
			wait for CLK_PERIOD;
		end loop;
		ramInsertMode <= '0';
		ramWriteEn <= '0';
			
			-- check if RAM insertion worked
			for i in 0 to NUM_RAM_ELEMENTS - 1 loop
				if ramElements_uut(i) /= ramElements_ref(i) then
					write(l, string'("    "));
					write(l, string'(": RAM[" & integer'image(i) & "] = x"""));
					hwrite(l, ramElements_uut(i));
					write(l, string'(""" (erwartet: x"""));
					hwrite(l, ramElements_ref(i));
					write(l, string'(""")"));
					writeline(OUTPUT, l);
					numRamInsertErrors := numRamInsertErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if numRamInsertErrors = 0 then
				write(l, string'(": Schreiben von Daten in den RAM per Debug-Port funktioniert fehlerfrei!"));
			else
				write(l, string'(": Schreiben von Daten in den RAM per Debug-Port fehlgeschlagen!"));
				write(l, string'("       Es werden keine weiteren Tests durchgefuehrt!"));
			end if;
			writeline(OUTPUT, l); writeline(OUTPUT, l);
			ramInsertMode <= '0';
			wait for 4 * CLK_PERIOD;
		
	if numRamInsertErrors = 0 then
		-- test load instructions
		write(l, time'image(now));
		write(l, string'(": Teste 'load word'-Instruktionen ..."));
		writeline(OUTPUT, l);
		for i in 0 to NUM_LW_TESTCASES - 1 loop
			testInstruction <= lw_test_instructions(i);
			wait for CLK_PERIOD;
		end loop;
		
			-- compare register contents
			for i in 0 to NUM_REGS - 1 loop
				if registers_uut(i) /= registers_ref(i) then
					write(l, string'("    "));
					write(l, string'(": Regs[" & integer'image(i) & "] = x"""));
					hwrite(l, registers_uut(i));
					write(l, string'(""" (erwartet: x"""));
					hwrite(l, registers_ref(i));
					write(l, string'(""")"));
					writeline(OUTPUT, l);
					numLwErrors := numLwErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if numLwErrors = 0 then
				write(l, string'(": 'load word'-Instruktionen funktionieren fehlerfrei!"));
			else
				write(l, string'(": 'load word'-Instruktionen fehlgeschlagen!"));
			end if;
			writeline(OUTPUT, l); writeline(OUTPUT, l);
			wait for 4 * CLK_PERIOD;
		
		-- test rFormat instructions
		write(l, time'image(now));
		write(l, string'(": Teste 'r-Format'-Instruktionen ..."));
		writeline(OUTPUT, l);
		for i in 0 to NUM_RF_TESTCASES - 1 loop
			testInstruction <= rf_test_instructions(i);
			wait for CLK_PERIOD;
			dstRegIdx := to_integer(unsigned(rf_test_instructions(i)(15 downto 11)));
			if registers_uut(dstRegIdx) /= registers_ref(dstRegIdx) then
				numRfErrors := numRfErrors + 1;
				write(l, string'("    "));
				write(l, time'image(now));
				write(l, string'(": Instruktion = x"""));
				hwrite(l, rf_test_instructions(i));
				write(l, string'(""" -> Reg[" & integer'image(dstRegIdx) & "] = x"""));
				hwrite(l, registers_uut(dstRegIdx));
				write(l, string'(""" (erwartet: "));
				hwrite(l, registers_ref(dstRegIdx));
				write(l, string'(""")"));
				writeline(OUTPUT, l);
			end if;
		end loop;
		
			write(l, time'image(now));
			if numRfErrors = 0 then
				write(l, string'(": 'r-Format'-Instruktionen funktionieren fehlerfrei!"));
			else
				write(l, string'(": 'r-Format'-Instruktionen fehlgeschlagen!"));
			end if;
			writeline(OUTPUT, l); writeline(OUTPUT, l);
			wait for 4 * CLK_PERIOD;
		
		-- test branches
		write(l, time'image(now));
		write(l, string'(": Teste 'branch if equal'-Instruktionen ..."));
		writeline(OUTPUT, l);
		for i in 0 to NUM_BEQ_TESTCASES - 1 loop
			testInstruction <= beq_test_instructions(i);
			wait for CLK_PERIOD / 2;
			if pc_next_uut /= pc_next_ref then
		        write(l, string'("    "));
			    write(l, time'image(now));
		        write(l, string'(": Fehler - PC = x"""));
		        hwrite(l, pc_next_uut);
		        write(l, string'(""" (erwartet: x"""));
		        hwrite(l, pc_next_ref);
		        write(l, string'(""")"));
			    writeline(OUTPUT, l);
			    numBeqErrors := numBeqErrors + 1;
			end if;
			wait for CLK_PERIOD / 2;
		end loop;
		
			write(l, time'image(now));
			if numBeqErrors = 0 then
				write(l, string'(": 'branch if equal'-Instruktionen funktionieren fehlerfrei!"));
			else
				write(l, string'(": 'branch if equal'-Instruktionen fehlgeschlagen!"));
			end if;
			writeline(OUTPUT, l); writeline(OUTPUT, l);
			wait for 4 * CLK_PERIOD;
		
		-- test store instructions
		write(l, time'image(now));
		write(l, string'(": Teste 'store word'-Instruktionen ..."));
		writeline(OUTPUT, l);
		for i in 0 to NUM_SW_TESTCASES - 1 loop
			testInstruction <= sw_test_instructions(i);
			wait for CLK_PERIOD;
		end loop;
		
		    -- compare RAM contents
			for i in 0 to NUM_RAM_ELEMENTS - 1 loop
				if ramElements_uut(i) /= ramElements_ref(i) then
					write(l, string'("    "));
					write(l, string'(": RAM[" & integer'image(i) & "] = x"""));
					hwrite(l, ramElements_uut(i));
					write(l, string'(""" (erwartet: x"""));
					hwrite(l, ramElements_ref(i));
					write(l, string'(""")"));
					writeline(OUTPUT, l);
					numSwErrors := numSwErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if numSwErrors = 0 then
				write(l, string'(": 'store word'-Instruktionen funktionieren fehlerfrei!"));
			else
				write(l, string'(": 'store word'-Instruktionen fehlgeschlagen!"));
			end if;
			writeline(OUTPUT, l); writeline(OUTPUT, l);
			wait for 4 * CLK_PERIOD;
		
		-- full program tests
		write(l, time'image(now));
		write(l, string'(": Teste vollstaendiges MIPS-Programm ..."));
		writeline(OUTPUT, l);
		rst <= '1';
		testMode <= '0';
		wait for 4 * CLK_PERIOD;
		rst <= '0';
		
			-- test 7-segment digits during program test
			pc_reg_ref := pc_next_ref;
			pc_reg_uut := pc_next_uut;
			for i in 0 to 25 loop
				if pc_reg_uut = pc_reg_ref then
					num7SegTestcases := num7SegTestcases + 1;
					for n in 0 to 3 loop
						if pc7SegDigits_uut(n) /= pc7SegDigits_ref(n) then
							write(l, string'("    "));
							write(l, time'image(now));
							write(l, string'(": 7-Segment-Anzeige nicht korrekt: x"""));
							for j in 3 downto 0 loop
								hwrite(l, "0" & pc7SegDigits_uut(j));
								write(l, string'(""" x"""));
							end loop;
							write(l, string'(""""));
							writeline(OUTPUT, l);
							
							write(l, string'("                                   erwartet: x"""));
							for j in 3 downto 0 loop
								hwrite(l, "0" & pc7SegDigits_ref(j));
								write(l, string'(""" x"""));
							end loop;
							write(l, string'(""""));
							writeline(OUTPUT, l);
							
							num7SegErrors := num7SegErrors + 1;
						end if;
					end loop;
				end if;
			
				pc_reg_ref := pc_next_ref;
				pc_reg_uut := pc_next_uut;
				wait for CLK_PERIOD;
			end loop;
			
			write(l, string'("    "));
			write(l, time'image(now));
			if num7SegErrors = 0 and num7SegTestcases > 15 then
				write(l, string'(": 7-Segment-Anzeige funktioniert fehlerfrei!"));
			elsif num7SegTestcases <= 15 then
				write(l, string'(": 7-Segment-Anzeige konnte nicht ausreichend getestet werden, da das MIPS-Programm nicht korrekt funktioniert!"));
			else
				write(l, string'(": 7-Segment-Anzeige ist fehlerhaft!"));
			end if;
			writeline(OUTPUT, l);
			
		    -- compare processor state (registers, pc_next)
		    for i in 0 to NUM_REGS - 1 loop
				if registers_uut(i) /= registers_ref(i) then
					numRegErrors := numRegErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if pc_next_uut = pc_next_ref and numRegErrors = 0 then
				write(l, string'(": Das MIPS-Programm wurde korrekt ausgefuehrt!"));
			else
				write(l, string'(": Das MIPS-Programm wurde nicht korrekt ausgefuehrt!"));
			end if;
			writeline(OUTPUT, l);
			writeline(OUTPUT, l);
	end if;
			
		-- evaluation
		writeline(OUTPUT, l);
		write(l, string'("---- Auswertung ----"));
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
	if numRamInsertErrors > 0 then
		write(l, string'("  Es wurde keine Tests durchgefuehrt, weil Schreiben von Werten in den RAM nicht funktioniert!"));
		writeline(OUTPUT, l);
	else
		if numLwErrors = 0 then
			write(l, string'("  'load word'-Instruktionen:        fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  'load word'-Instruktionen:        fehlerhaft (" & integer'image(numLwErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numRfErrors = 0 then
			write(l, string'("  'r-Format'-Instruktionen:         fehlerfrei"));
			points := points + 4;
		else
			if numRfErrors = 1 then points := points + 3;
			elsif numRfErrors = 2 then points := points + 2;
			elsif numRfErrors = 3 then points := points + 1;
			end if;
			write(l, string'("  'r-Format'-Instruktionen:         fehlerhaft (" & integer'image(numRfErrors) & " Fehler -> " & integer'image(points) & "/4 Punkte)"));
		end if;
		writeline(OUTPUT, l);
		
		if numBeqErrors = 0 then
			write(l, string'("  'branch if equal'-Instruktionen:  fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  'branch if equal'-Instruktionen:  fehlerhaft (" & integer'image(numBeqErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if numSwErrors = 0 then
			write(l, string'("  'store word'-Instruktionen:       fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  'store word'-Instruktionen:       fehlerhaft (" & integer'image(numSwErrors) & " Fehler)"));
		end if;
		writeline(OUTPUT, l);
		
		if pc_next_uut = pc_next_ref and numRegErrors = 0 then
			write(l, string'("  MIPS-Programm:                    fehlerfrei"));
			points := points + 4;
		else
			write(l, string'("  MIPS-Programm:                    fehlerhaft"));
		end if;
		writeline(OUTPUT, l);
		
		if num7SegErrors = 0 and num7SegTestcases > 15 then
			write(l, string'("  7-Segment-Anzeige:                fehlerfrei"));
			points := points + 2;
		else
			write(l, string'("  7-Segment-Anzeige:                fehlerhaft"));
		end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
	end if;
		
        if points = 16 then
	        report "Der MIPS-Prozessor funktioniert einwandfrei! (Punktzahl: " & integer'image(points) & "/16)" severity note;
	    else
	        report "Der MIPS-Prozessor ist fehlerhaft! (Punktzahl: " & integer'image(points) & "/16)" severity failure;
	    end if;
	    
	    sim_stop <= true;
		wait;
	end process;
	
end behavioral;
