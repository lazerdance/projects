library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.mux4;

entity mux4_tb is
end mux4_tb;

architecture behavioral of mux4_tb is

    constant DATA_WIDTH : integer := 32;
	signal a : std_logic_vector(DATA_WIDTH - 1 downto 0) := (others => 'U');
	signal b : std_logic_vector(DATA_WIDTH - 1 downto 0) := x"FF00FF00";
	signal c : std_logic_vector(DATA_WIDTH - 1 downto 0) := x"00FF00FF";
	signal d : std_logic_vector(DATA_WIDTH - 1 downto 0) := (others => '1');
	signal sel : std_logic_vector(1 downto 0) := "UU";
	signal y, y_ref : std_logic_vector(DATA_WIDTH - 1 downto 0);
	
begin

    MUX_INST: entity work.mux4(behavioral)
    generic map(DATA_WIDTH => DATA_WIDTH)
	port map(a => a, b => b, c => c, d => d,
	         sel => sel, y => y);
    
    MUX_INST_REF: entity ROrgPrSimLib.mux4(behavioral)
    generic map(DATA_WIDTH => DATA_WIDTH)
	port map(a => a, b => b, c => c, d => d,
	         sel => sel, y => y_ref);
	
    TESTBENCH: process
        variable points : integer := 0;
        variable numErrors : integer := 0;
    begin
        
        -- check behavior for invalid select signal
        wait for 5 ns;
        if y /= y_ref then
            report "Falsche Ausgabe des Multiplexers bei uninitialisiertem Select-Signal!" severity error;
        else
            points := points + 1;
        end if;
        wait for 5 ns;
        
        -- check if uninitialized input arrives at output
        sel <= "00";
        wait for 5 ns;
        if y /= y_ref then
            report "Uninitialisiertes Signal wird nicht auf den Ausgang durchgeschaltet!" severity error;
        else
            points := points + 1;
        end if;
        wait for 5 ns;
        
        -- check if mux works correctly under common conditions
        a <= (others => '0');
        for i in 0 to 3 loop
            sel <= std_logic_vector(to_unsigned(i, 2));
            wait for 5 ns;
            if y /= y_ref then
                report "Ausgang des Multiplexers unterscheidet sich vom selektierten Eingang!" severity error;
                numErrors := numErrors + 1;
            end if;
            wait for 5 ns;
        end loop;
        
        if numErrors = 0 then
            points := points + 1;
        end if;
        
        
        -- evaluation
        if points = 3 then
	        report "Der Multiplexer funktioniert einwandfrei! (mux4: 3/3 Punkte)" severity note;
	    else
	        report "Der Multiplexer funktioniert nicht einwandfrei! (mux4: "
	        & integer'image(points) & "/3 Punkte)" severity failure;
	    end if;
	    
	    wait;
    end process;

end behavioral;
