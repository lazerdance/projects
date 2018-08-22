library ieee;
use ieee.std_logic_1164.all;

library std;
use std.textio.all;

entity reg_tb is
end reg_tb;

architecture behavioral of reg_tb is

	signal clk, en, rst :std_logic := '0';
	constant WIDTH : integer := 32;
	signal D : std_logic_vector(WIDTH - 1 downto 0);
	signal Q : std_logic_vector(WIDTH - 1 downto 0);
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
	
	REG: entity work.reg(behavioral)
		generic map(
			WIDTH => WIDTH)
		port map(
			clk => clk,
			rst => rst,
			en => en,
			D => D,
			Q => Q);

	TESTBENCH: process
		variable points : integer := 2;
		variable resetNotWorking : boolean := false;
		variable edgeNotWorking : boolean := false;
		variable l : line;
	begin
	
		-- check for synchronous reset functionality
		wait for 2 ns; -- 2ns
		rst <= '1';
		wait for 2 ns; -- 4ns
		if Q = x"00000000" then
			write(l, time'image(now));
			write(l, string'(": Das Register hat ein asynchrones reset-Signal."));
			writeline(OUTPUT, l);
			points := points - 1;
		else
			wait for 7 ns; -- 11ns
			if Q /= x"00000000" then
				write(l, time'image(now));
				write(l, string'(": Das reset-Signal hat keinen Einfluss auf das Register."));
				writeline(OUTPUT, l);
				points := points - 1;
				resetNotWorking := true;
			end if;
		end if;
		wait for 1 ns;
		rst <= '0';
		wait for 13 ns - now; -- 13ns
        
        	-- Check for correct edge detection
		D <= x"DEADBEEF";
		en <= '1';
		wait for 5 ns; -- 18ns
                if Q /= x"00000000" then
			write(l, time'image(now));
			write(l, string'(": Der Inhalt des Registers hat sich ohne steigende Taktflanke geändert."));
			writeline(OUTPUT, l);
			points := points - 1;
			edgeNotWorking := true;
                    end if;
		wait for 7 ns; -- 25ns
		
		-- check functionality of the enable signal
		if points > 0 then
                    en <= '0';
                    D <= (others => '1');
                    wait for 6 ns; -- 31ns
                    if Q = x"FFFFFFFF" then
                            write(l, time'image(now));
                            write(l, string'(": Trotz nichtgesetztem enable-Signal wird das Register beschrieben."));
                            writeline(OUTPUT, l);
                            points := points - 1;
                    else
                            en <= '1';
                            wait for 10 ns; -- 41ns
                            if Q /= x"FFFFFFFF" then
                                    write(l, time'image(now));
                                    write(l, string'(": Das Register wird bei gesetztem enable-Signal nicht korrekt beschrieben."));
                                    writeline(OUTPUT, l);
                                    points := points - 1;
                            end if;
                    end if;
                    wait for 45 ns - now;
                end if;
		
		-- check if reset has higher priority than common write
		if points > 0 then
			rst <= '1';
			en <= '1';
			D <= x"AAAA5555";
			wait for 6 ns; -- 51ns
			if Q /= x"00000000" then
				write(l, time'image(now));
				write(l, string'(": Das reset-Signal hat eine niedrigere Prioritaet als ein gewoehnlicher Schreibzugriff."));
				writeline(OUTPUT, l);
				if points > 0 then
					points := points - 1;
				end if;
			else
				rst <= '0';
				wait for 10 ns; -- 61ns
				if Q /= x"AAAA5555" then
					write(l, time'image(now));
					write(l, string'(": Gewoehnlicher Schreibzugriff fehlgeschlagen."));
					writeline(OUTPUT, l);
					if points > 0 then
						points := points - 1;
					end if;
				end if;
			end if;
		end if;
		rst <= '0';
		en <= '0';
		
		-- evaluation
		wait for 70 ns - now;
		
		if points = 2 then
			report "Das Register funktioniert einwandfrei! (reg: 2/2 Punkte)" severity note;
		else
			report "Das Register funktioniert nicht einwandfrei! (reg: "& integer'image(points) & "/2 Punkte)" severity failure;
		end if;
		
		sim_stop <= true;
		wait;
	end process;
	
end behavioral;
