library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;
use ROrgPrSimLib.mipsCpu_mc;

entity mipsCpu_mc_tb is
    generic(init_file_name : string := "memcpy.mif");
end mipsCpu_mc_tb;


architecture behavioral of mipsCpu_mc_tb is

	signal clk : std_logic := '0';
	signal rst : std_logic := '0';
	signal sim_stop : boolean := false;
	
	signal ramElements_uut, ramElements_ref : ram_elements_type;
	signal registers_uut, registers_ref : reg_vector_type;
	signal mipsCtrlState_uut, mipsCtrlState_ref : mips_ctrl_state_type;
	signal pc7SegDigits_uut, pc7SegDigits_ref : pc_7seg_digits_type;
	
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
	UUT: entity work.mipsCpu_mc(structural)
	generic map(INIT_FILE_NAME => init_file_name)
	port map(clk => clk,
			 rst => rst,
			 ramElements_debug => ramElements_uut,
			 registers_debug => registers_uut,
			 mipsCtrlState_debug => mipsCtrlState_uut,
			 pc7SegDigits_debug => pc7SegDigits_uut);
	
	-- reference design
	REF: entity ROrgPrSimLib.mipsCpu_mc(structural)
	generic map(INIT_FILE_NAME => init_file_name)
	port map(clk => clk,
			 rst => rst,
			 ramElements_debug => ramElements_ref,
			 registers_debug => registers_ref,
			 mipsCtrlState_debug => mipsCtrlState_ref,
			 pc7SegDigits_debug => pc7SegDigits_ref);
	
	TESTBENCH: process
		variable l : line;
		variable numStateErrors, num7SegErrors, numRegErrors, numMemErrors, points : integer := 0;
	begin
	
		-- full program test
		wait for 2 ns;
		rst <= '1';
		wait for 4 * CLK_PERIOD;
		rst <= '0';
		wait for 1 ns;
		
		write(l, time'image(now));
		write(l, string'(": Teste """ & init_file_name & """ ..."));
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
		while true loop
		
			-- test 7-segment digits during program test
			for n in 0 to 3 loop
				if pc7SegDigits_uut(n) /= pc7SegDigits_ref(n) then
					write(l, string'("    "));
					write(l, time'image(now));
					write(l, string'(": Warnung ... 7-Segment-Anzeige nicht korrekt: "));
					for j in 3 downto 0 loop
						write(l, string'("x"""));
						hwrite(l, "0" & pc7SegDigits_uut(n));
						write(l, string'(""" "));
					end loop;
					writeline(OUTPUT, l);
				
					write(l, string'("                                   erwartet: x"""));
					for j in 3 downto 0 loop
						hwrite(l, "0" & pc7SegDigits_ref(n));
						write(l, string'(""" x"""));
					end loop;
					write(l, string'(""""));
					writeline(OUTPUT, l);
				
					num7SegErrors := num7SegErrors + 1;
					exit;
				end if;
			end loop;
			
			-- test control state during program test
			if mipsCtrlState_uut /= mipsCtrlState_ref then
				write(l, string'("    "));
				write(l, time'image(now));
				write(l, string'(": Warnung ... Prozessorzustand nicht korrekt: " & toString(mipsCtrlState_uut) & " (erwartet : " & toString(mipsCtrlState_ref) & ")"));
				writeline(OUTPUT, l);
			
				numStateErrors := numStateErrors + 1;
			end if;
			
			wait for CLK_PERIOD;
			
			if registers_ref(2) = x"00000001" then
				-- program signaled finished execution
				exit;
			end if;
			
		end loop;
	
		-- compare register contents
		for i in 0 to NUM_REGS - 1 loop
			if registers_uut(i) /= registers_ref(i) then
				numRegErrors := numRegErrors + 1;
				write(l, string'("    "));
				write(l, time'image(now));
				write(l, string'(": Reg[" & integer'image(i) & "] = x"""));
				hwrite(l, registers_uut(i));
				write(l, string'(""" (erwartet: x"""));
				hwrite(l, registers_ref(i));
				write(l, string'(""")"));
				writeline(OUTPUT, l);
			end if;
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
				numMemErrors := numMemErrors + 1;
			end if;
		end loop;
	
	
		-- evaluation
		if numStateErrors = 0 and numRegErrors = 0 and numMemErrors = 0 then
	        report "Das MIPS-Programm wurde korrekt ausgefuehrt!" severity note;
	    else
	        report "Die Ausfuehrung des MIPS-Programms war fehlerhaft!" severity failure;
	    end if;
	    sim_stop <= true;
		wait;
	end process;
	
end behavioral;
