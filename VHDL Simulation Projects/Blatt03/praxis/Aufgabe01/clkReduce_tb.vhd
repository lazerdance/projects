library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clkReduce_tb is
end clkReduce_tb;

architecture behavioral of clkReduce_tb is

	signal clk_10ns : std_logic := '0';
	signal clk_20ns, clk_60ns : std_logic;
  signal simStop : boolean := false;

begin

    REF_CLK_GEN: process
    begin
        if (simStop = true) then
          wait;
        end if;
        clk_10ns <= not clk_10ns;
        wait for 5 ns;
        clk_10ns <= not clk_10ns;
        wait for 5 ns;
    end process;

    CLK_DIV2: entity work.clkReduce(behavioral)
	generic map(divisor => 2)
	port map(clk_in => clk_10ns,
	         clk_out => clk_20ns);
	
	CLK_DIV3: entity work.clkReduce(behavioral)
	generic map(divisor => 3)
	port map(clk_in => clk_20ns,
	         clk_out => clk_60ns);
	
	TESTBENCH: process
	    variable currentTime, refTime1, refTime2 : time;
	    variable points : integer := 0;
	begin
	    -- check for transitions
	    wait until rising_edge(clk_60ns) for 1000 ns;
	    if clk_60ns /= '1' then
	        report "Keine steigende Taktflanke erkannt! (clkReduce: 0/5 Punkte)" severity failure;
	    end if;
	    wait until falling_edge(clk_60ns) for 1000 ns;
	    if clk_60ns /= '0' then
	        report "Keine fallende Taktflanke erkannt! (clkReduce: 0/5 Punkte)" severity failure;
	    end if;
	    points := points + 1;
	    
	    -- check for equal duration of '0' and '1'
	    refTime1 := now;
	    wait until rising_edge(clk_60ns);
	    refTime2 := now;
	    wait until falling_edge(clk_60ns);
	    currentTime := now;
	    
	    if (currentTime - refTime2) /= (refTime2 - refTime1) then
	        report "Die Dauer von '0' und '1' im Taktsignal ist nicht gleich!" severity error;
	    else
	        points := points + 2;
	    end if;
	    
	    -- check for correct period
	    wait until rising_edge(clk_60ns);
	    refTime1 := now;
	    wait until rising_edge(clk_60ns);
	    currentTime := now;
	    
	    if (currentTime - refTime1) /= 60 ns then
	        report "Die Periodendauer des erzeugten Taktsignals ist nicht korrekt: "
	        & time'image(currentTime - refTime1) & " (erwartet: 60 ns)" severity error;
	    else
	        points := points + 2;
	    end if;
	    
	    -- evaluation
	    if points = 5 then
	        report "Der Taktteiler funktioniert einwandfrei! (clkReduce: 5/5 Punkte)" severity note;
	    else
	        report "Der Taktteiler funktioniert nicht einwandfrei! (clkReduce: "
	        & integer'image(points) & "/5 Punkte)" severity failure;
	    end if;
	    
      simStop <= true;
      wait;
    end process;

end behavioral;
