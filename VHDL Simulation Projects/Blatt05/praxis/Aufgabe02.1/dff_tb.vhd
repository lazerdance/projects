library ieee;
use ieee.std_logic_1164.all;

library std;
use std.textio.all;

entity dff_tb is
end dff_tb;

architecture behavioral of dff_tb is

	signal clk :std_logic := '0';
	signal D : std_logic := '0';
        signal Q : std_logic;
	signal sim_stop : boolean := false;
begin

	CLK_GEN: process
	begin
		if sim_stop
		then
			wait;
		end if;
		clk <= not clk;
		wait for 5 ns;
	end process;

	D_FLIP_FLOP: entity work.dff(behavioral)
	port map(
		clk => clk,
		D   => D,
		Q   => Q);

	TESTBENCH: process
		variable points : integer := 2;
		variable l : line;
	begin

		-- check if flip flop is not updated without rising clock edge
		wait for 3 ns; -- 3ns
		D <= '1';
		wait for 5 ns; -- 8ns
		if Q = D then
			write(l, time'image(now));
			write(l, string'(": D-Flip-Flop Zustand hat sich ohne steigende Taktflanke geaendert."));
			writeline(OUTPUT, l);
			points := 0;
		else
			-- check if flip flop is updated on rising clock edge
			wait for 3 ns; -- 11ns
			
			if Q /= '1' then
				write(l, time'image(now));
				write(l, string'(": D-Flip-Flop wird bei steigender Taktflanke nicht aktualisiert"));
				writeline(OUTPUT, l);
				points := 0;
			end if;
		end if;
		
		-- evaluation
		wait for 40 ns - now;
		if points = 2 then
			report "Das D-Flip-Flop funktioniert einwandfrei! (dff: 2/2 Punkte)" severity note;
		else
			report "Das D-Flip-Flop funktioniert nicht einwandfrei! (dff: "
			& integer'image(points) & "/2 Punkte)" severity failure;
		end if;
		
		sim_stop <= true;
		wait;
	end process;
	
end behavioral;
