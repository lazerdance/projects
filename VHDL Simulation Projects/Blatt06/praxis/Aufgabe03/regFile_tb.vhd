library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;

entity regFile_tb is
end regFile_tb;

architecture behavioral of regFile_tb is

	signal clk : std_logic := '0';
	signal rst : std_logic := '1';
	type read_addr_array_type is array (1 to 2) of std_logic_vector(LOG2_NUM_REGS - 1 downto 0);
	signal readAddr : read_addr_array_type := (others => (others => '0'));
	type read_data_array_type is array (1 to 2) of std_logic_vector(REG_WIDTH - 1 downto 0);
	signal readData : read_data_array_type;
	signal writeEn : std_logic := '0';
	signal writeAddr : std_logic_vector(LOG2_NUM_REGS - 1 downto 0) := (others => '0');
	signal writeData : std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0) := (others => '0');
	signal reg_vector_debug : reg_vector_type;
	
	constant NULL_CONST : std_logic_vector(REG_WIDTH - 1 downto 0) := (others => '0');
	signal sim_stop : boolean := false;
	
begin

	CLK_GEN: process
	begin
		if sim_stop then
			wait;
		end if;
		clk <= not clk;
		wait for 5 ns;
	end process;
	
    REG_FILE: entity work.regFile(structural)
	generic map(NUM_REGS => NUM_REGS,
				LOG2_NUM_REGS => LOG2_NUM_REGS,
				REG_WIDTH => REG_WIDTH)
	port map(clk => clk,
			 rst => rst,
			 readAddr1 => readAddr(1),
			 readData1 => readData(1),
			 readAddr2 => readAddr(2),
			 readData2 => readData(2),
			 writeEn => writeEn,
			 writeAddr => writeAddr,
			 writeData => writeData,
			 reg_vect_debug => reg_vector_debug);

    TESTBENCH: process
        variable points : integer := 0;
        variable numErrors : integer := 0;
        variable l : line;
        variable readWorks, writeWorks : boolean := false;
        variable reg_vect_tmp : reg_vector_type := (others => (others => '1'));

    begin
    
    	-- check writing functionality
    	wait for 15 ns;
    	rst <= '0';
    	numErrors := 0;
    	write(l, time'image(now));
    	write(l, string'(": Starte Schreibtest ..."));
    	writeline(OUTPUT, l);
    	writeEn <= '1';
    	for i in 0 to NUM_REGS - 1 loop
    		writeAddr <= std_logic_vector(to_unsigned(i, LOG2_NUM_REGS));
    		writeData <= std_logic_vector(to_unsigned(i, REG_WIDTH - 2) & "10");
    		reg_vect_tmp(i) := std_logic_vector(to_unsigned(i, REG_WIDTH - 2) & "10");
    		wait for 10 ns;
    		test: for j in 0 to i loop
                if reg_vector_debug(j) /= reg_vect_tmp(j) then
                    if numErrors < 4 then
                        if j = i then
                            write(l, time'image(now));
                            write(l, string'(": Schreibzugriff auf Register " & integer'image(i) & " fehlgeschlagen: Reg(" & integer'image(i) & ") = x"""));
                            hwrite(l, reg_vector_debug(i));
                            write(l, string'(""" (erwartet: x"""));
                            hwrite(l, writeData);
                            write(l, string'(""")"));
                            writeline(OUTPUT, l);
                        else
                            write(l, time'image(now));
                            write(l, string'(": Schreibzugriff auf Register " & integer'image(i) & " ueberschreibt auch Register(" & integer'image(j) & ") = x"""));
                            hwrite(l, reg_vector_debug(j));
                            write(l, string'(""" (erwartet: x"""));
                            hwrite(l, reg_vect_tmp(j));
                            write(l, string'(""")"));
                            writeline(OUTPUT, l);
                        end if;    
                    elsif numErrors = 4 then
                        write(l, time'image(now));
                        write(l, string'(": Weitere Schreibfehler werden nicht angezeigt ..."));
                        writeline(OUTPUT, l);
                    end if;

                    numErrors := numErrors + 1;
                end if;
            end loop;
    	end loop;
    	
    	write(l, time'image(now));
    	if numErrors = 0 then
			write(l, string'(": Schreibtest erfolgreich!"));
    		points := points + 2;
    		writeWorks := true;
    	else
			write(l, string'(": Schreibtest nicht erfolgreich! (" & integer'image(numErrors) & " Fehler)"));
    	end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
		-- check reading functionality
		if writeWorks then
			numErrors := 0;
			write(l, time'image(now));
			write(l, string'(": Starte Lesetest ..."));
			writeline(OUTPUT, l);
			writeEn <= '0';
			for i in 0 to (NUM_REGS / 2) - 1 loop
                wait for 1 ns;
				readAddr(1) <= std_logic_vector(to_unsigned(2*i, LOG2_NUM_REGS));
				readAddr(2) <= std_logic_vector(to_unsigned(2*i + 1, LOG2_NUM_REGS));
				-- In behavioral simulation output should work without clk transaction
				wait for 3 ns;
				for n in 1 to 2 loop
					if readData(n) /= reg_vector_debug(2*i + n - 1) then
						if numErrors < 4 then
							write(l, time'image(now));
							write(l, string'(": Lesezugriff an Adresse " & integer'image(i) & " fehlgeschlagen: readData = x"""));
							hwrite(l, readData(n));
							write(l, string'(""" (erwartet: x"""));
							hwrite(l, reg_vector_debug(2*i + n - 1));
							write(l, string'(""")"));
							writeline(OUTPUT, l);
						elsif numErrors = 4 then
							write(l, time'image(now));
							write(l, string'(": Weitere Lesefehler werden nicht angezeigt ..."));
							writeline(OUTPUT, l);
						end if;
					
						numErrors := numErrors + 1;
					end if;
				end loop;
				wait for 6 ns;
			end loop;
			
			write(l, time'image(now));
			if numErrors = 0 then
				write(l, string'(": Lesetest erfolgreich!"));
				points := points + 2;
				readWorks := true;
			else
				write(l, string'(": Lesetest nicht erfolgreich! (" & integer'image(numErrors) & " Fehler)"));
			end if;
			writeline(OUTPUT, l);
		else
			write(l, time'image(now));
			write(l, string'(": Lesetest wird nicht durchgefuehrt, da der Registerspeicher nicht beschrieben werden kann!"));
			writeline(OUTPUT, l);
		end if;
		writeline(OUTPUT, l);
		
		-- check reset functionality
		if writeWorks then
			numErrors := 0;
			write(l, time'image(now));
			write(l, string'(": Teste reset-Verhalten des Registerspeichers ..."));
			writeline(OUTPUT, l);
			rst <= '1';
			wait for 10 ns;
			rst <= '0';
			for i in 0 to NUM_REGS - 1 loop
				if reg_vector_debug(i) /= NULL_CONST then
					if numErrors < 4 then
						write(l, time'image(now));
						write(l, string'(": Register " & integer'image(i) & " wurde nicht zurueckgesetzt!"));
						writeline(OUTPUT, l);
					elsif numErrors = 4 then
						write(l, time'image(now));
						write(l, string'(": Weitere nicht zurueckgesetzte Register werden nicht angezeigt ..."));
						writeline(OUTPUT, l);
					end if;
					
					numErrors := numErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if numErrors = 0 then
				write(l, string'(": Der Registerspeicher laesst sich korrekt zuruecksetzen!"));
				writeline(OUTPUT, l);
				points := points + 1;
			else
				write(l, string'(": Der Registerspeicher laesst sich nicht korrekt zuruecksetzen!"));
				writeline(OUTPUT, l);
			end if;
		else
			write(l, time'image(now));
			write(l, string'(": Reset-Test wird nicht durchgefuehrt, da der Registerspeicher nicht beschrieben werden kann!"));
			writeline(OUTPUT, l);
		end if;
		writeline(OUTPUT, l);
		
		-- evaluation
        if points = 5 then
	        report "Der Registerspeicher funktioniert einwandfrei! (regFile: 5/5 Punkte)" severity note;
	    else
	        report "Der Registerspeicher ist fehlerhaft! (regFile: "
	        & integer'image(points) & "/5 Punkte)" severity failure;
	    end if;
		
		sim_stop <= true;
		wait;
    end process;
    
end behavioral;
