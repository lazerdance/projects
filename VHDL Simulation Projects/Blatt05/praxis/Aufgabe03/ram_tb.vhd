library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;

entity ram_tb is
end ram_tb;

architecture behavioral of ram_tb is

	signal clk : std_logic := '0';
	signal address : std_logic_vector(LOG2_NUM_RAM_ELEMENTS - 1 downto 0);
	signal writeEn : std_logic := '0';
	signal writeData : std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0);
	signal readEn : std_logic := '0';
	signal readData : std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0);
	signal ramElements_debug : ram_elements_type;
	signal sim_stop : boolean := false;

	constant NUM_TESTCASES : integer := 8;

begin

	CLK_GEN: process
	begin
		if sim_stop then
			wait;
		end if;
		clk <= not clk;
		wait for 5 ns;
	end process;
	
	RAM: entity work.ram(behavioral)
		generic map(
			NUM_ELEMENTS => NUM_RAM_ELEMENTS,
			LOG2_NUM_ELEMENTS => LOG2_NUM_RAM_ELEMENTS,
			ELEMENT_WIDTH => RAM_ELEMENT_WIDTH)
		port map(
			clk => clk,
			address => address,
			writeEn => writeEn,
			writeData => writeData,
			readEn => readEn,
			readData => readData,
			ramElements_debug => ramElements_debug);

	TESTBENCH: process
		variable points : integer := 0;
		variable numErrors : integer := 0;
		variable l : line;
		variable readWorks, writeWorks : boolean := false;
	begin
			-- check writing funtioncality
			wait for 5 ns;
			write(l, time'image(now));
			write(l, string'(": Starte Schreibtest ..."));
			writeline(OUTPUT, l);
			writeEn <= '1';
			readEn <= '0';
			for i in 0 to NUM_RAM_ELEMENTS - 1 loop
				address <= std_logic_vector(to_unsigned(i, LOG2_NUM_RAM_ELEMENTS));
				writeData <= std_logic_vector(to_unsigned(i, RAM_ELEMENT_WIDTH - 2) & "00");
				readEn <= not readEn;
				wait for 10 ns;
				if ramElements_debug(i) /= writeData then
					if numErrors < 4 then
					write(l, time'image(now));
					write(l, string'(": Schreibzugriff an Adresse " & integer'image(i) & " fehlgeschlagen: RAM(" & integer'image(i) & ") = """));
					hwrite(l, ramElements_debug(i));
					write(l, string'(""" (erwartet: """));
					hwrite(l, writeData);
					write(l, string'(""")"));
					writeline(OUTPUT, l);
				elsif numErrors = 4 then
					write(l, time'image(now));
					write(l, string'(": Weitere Schreibfehler werden nicht angezeigt ..."));
					writeline(OUTPUT, l);
				end if;
					
					numErrors := numErrors + 1;
				end if;
			end loop;
			
			write(l, time'image(now));
			if numErrors = 0 then
			write(l, string'(": Schreibtest erfolgreich!"));
				points := points + 1;
				writeWorks := true;
			else
			write(l, string'(": Schreibtest nicht erfolgreich! (" & integer'image(numErrors) & " Fehler)"));
			end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
		-- check reading functionality
		numErrors := 0;
		write(l, time'image(now));
			write(l, string'(": Starte Lesetest ..."));
			writeline(OUTPUT, l);
			writeEn <= '0';
			readEn <= '1';
			for i in 0 to NUM_RAM_ELEMENTS - 1 loop
				address <= std_logic_vector(to_unsigned(i, LOG2_NUM_RAM_ELEMENTS));
				wait for 10 ns;
				if readData /= ramElements_debug(i) then
					if numErrors < 4 then
					write(l, time'image(now));
					write(l, string'(": Lesezugriff an Adresse " & integer'image(i) & " fehlgeschlagen: readData = """));
					hwrite(l, readData);
					write(l, string'(""" (erwartet: """));
					hwrite(l, ramElements_debug(i));
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
			
			write(l, time'image(now));
			if numErrors = 0 then
			write(l, string'(": Lesetest erfolgreich!"));
				points := points + 1;
				readWorks := true;
			else
			write(l, string'(": Lesetest nicht erfolgreich! (" & integer'image(numErrors) & " Fehler)"));
			end if;
		writeline(OUTPUT, l);
		writeline(OUTPUT, l);
		
		if writeWorks and readWorks then
			-- check synchronous behavior
			write(l, time'image(now));
			write(l, string'(": Teste synchrones Verhalten des RAMs ..."));
			writeline(OUTPUT, l);
			wait for 2 ns; -- xxx7ns
			address <= (others => '0');
			writeEn <= '1';
			writeData <= x"AAAAAAAA";
			readEn <= '0';
			wait for 1 ns; -- xxx8ns
			if ramElements_debug(0) = writeData then
				write(l, time'image(now));
				write(l, string'(": Der RAM laesst sich asynchron beschreiben!"));
				writeline(OUTPUT, l);
				writeline(OUTPUT, l);
				wait for 27 ns; -- xxx5ns
			else
				address <= (others => '0');
				writeEn <= '0';
				readEn <= '1';
				wait for 1 ns; -- xxx9ns
				if readData = ramElements_debug(0) then
					write(l, time'image(now));
					write(l, string'(": Der RAM laesst sich asynchron lesen!"));
					writeline(OUTPUT, l);
					writeline(OUTPUT, l);
					wait for 20 ns;
				else
                                        readEn <= '0';
                                        wait for 5 ns;
                                        writeData <= x"BADEAFFE";
                                        writeEn <= '1';
                                        wait for 5 ns;
                                        writeEn <= '0';
                                        if ramElements_debug(0) = writeData then
                                            write(l, time'image(now));
                                            write(l, string'(": Der RAM laesst sich bei fallenden Taktflanken beschreiben!"));
                                            writeline(OUTPUT, l);
                                            writeline(OUTPUT, l);
                                            wait for 10 ns;
                                        else
                                            wait for 3 ns;
                                            readEn <= '1';
                                            wait for 7 ns;
                                            readEn <= '0';
                                            if readData = ramElements_debug(0) then
                                                write(l, time'image(now));
                                                write(l, string'(": Der RAM laesst sich bei fallenden Taktflanken lesen!"));
                                                writeline(OUTPUT, l);
                                                writeline(OUTPUT, l);
                                            else
                                                write(l, time'image(now));
                                                write(l, string'(": Der RAM arbeitet vollstaendig synchron! (Test bestanden)"));
                                                writeline(OUTPUT, l);
                                                writeline(OUTPUT, l);
                                                points := points + 1;
                                            end if;
                                        end if;
				end if;
				wait for 6 ns; -- xxx5ns
			end if;
			
		
			-- check write-first behavior
			wait for 2 ns;
			numErrors := 0;
			write(l, time'image(now));
			write(l, string'(": Starte write-first-Test ..."));
			writeline(OUTPUT, l);
			writeEn <= '1';
			writeData <= (others => '1');
			readEn <= '1';
			wait for 6 ns;
			if readData /= writeData then
				write(l, time'image(now));
				write(l, string'(": Gelesenes Element """));
				hwrite(l, readData);
				write(l, string'(""" entspricht nicht dem geschriebenem Element """));
				hwrite(l, writeData);
				write(l, string'("""."));
				writeline(OUTPUT, l);
				numErrors := 1;
			end if;
			wait for 2 ns;
			write(l, time'image(now));
		if numErrors = 0 then
				write(l, string'(": Write-first-Test erfolgreich!"));
				points := points + 1;
			else
				write(l, string'(": Write-first-Test nicht erfolgreich!"));
			end if;
			writeline(OUTPUT, l);
			writeline(OUTPUT, l);
		else
			write(l, string'("Tests des synchronen Verhaltens und der write-first-Funktionalitaet werden uebersprungen, da Schreib- und Lesezugriffe nicht fehlerfrei moeglich sind!"));
			writeline(OUTPUT, l);
			writeline(OUTPUT, l);
		end if;
		
		wait for 10 ns;
		
		-- evaluation
		if points = 4 then
			report "Der RAM funktioniert einwandfrei! (ram: 4/4 Punkte)" severity note;
		else
			report "Der RAM ist fehlerhaft! (ram: "& integer'image(points) & "/4 Punkte)" severity failure;
		end if;
	
		sim_stop <= true;
		wait;
	end process;
	  
end behavioral;
