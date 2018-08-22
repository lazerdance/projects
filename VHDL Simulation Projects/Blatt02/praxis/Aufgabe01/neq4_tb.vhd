library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.neq4;

entity neq4_tb is
end neq4_tb;

architecture behavioral of neq4_tb is

    type neq4Data is
    record
        a, b    : std_logic_vector(3 downto 0);
        y       : std_logic;
    end record;
    
    signal logic   : neq4data;
    signal netlist : neq4data;
	signal ref     : neq4data;
	constant NUM_TESTCASES : integer := 256;

begin

	NEQ4_LOGIC_INST: entity work.neq4(logic)
	port map(a => logic.a, b => logic.b, y => logic.y);

	NEQ4_NETLIST_INST: entity work.neq4(netlist)
	port map(a => netlist.a, b => netlist.b, y => netlist.y);

	NEQ4_INST_REF: entity ROrgPrSimLib.neq4(logic)
	port map(a => ref.a, b => ref.b, y => ref.y);

	process
		variable numLogicErrors, numNetlistErrors : integer := 0;
		variable tmp_i : std_logic_vector(7 downto 0);
	begin

		for i in 0 to NUM_TESTCASES - 1 loop
			
			tmp_i := std_logic_vector(to_unsigned(i, 8));
			ref.a <= tmp_i(7 downto 4);
			ref.b <= tmp_i(3 downto 0);
			logic.a <= tmp_i(7 downto 4);
			logic.b <= tmp_i(3 downto 0);
			netlist.a <= tmp_i(7 downto 4);
			netlist.b <= tmp_i(3 downto 0);

			wait for 5 ns;

			if logic.y /= ref.y then
			    report "falsches Ergebnis am neq4-Modul(logic) (a = """ &
			    integer'image(to_integer(unsigned'("" & ref.a(3)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(2)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(1)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(0)))) &
		        """, b = """ &
		        integer'image(to_integer(unsigned'("" & ref.b(3)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(2)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(1)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(0)))) &
			    """): " & std_logic'image(logic.y) & " (erwartet: " & std_logic'image(ref.y) & ")" severity error;
				numLogicErrors := numLogicErrors + 1;
			end if;

			if netlist.y /= ref.y then
			    report "falsches Ergebnis am neq4-Modul(netlist) (a = """ &
			    integer'image(to_integer(unsigned'("" & ref.a(3)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(2)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(1)))) &
		        integer'image(to_integer(unsigned'("" & ref.a(0)))) &
		        """, b = """ &
		        integer'image(to_integer(unsigned'("" & ref.b(3)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(2)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(1)))) &
		        integer'image(to_integer(unsigned'("" & ref.b(0)))) &
			    """): " & std_logic'image(netlist.y) & " (erwartet: " & std_logic'image(ref.y) & ")" severity error;
				numNetlistErrors := numNetlistErrors + 1;
			end if;

			wait for 5 ns;

		end loop;

		if numLogicErrors = 0 then
			report "Das neq4-Modul(logic) funktioniert einwandfrei!" severity note;
        end if;
        if numNetlistErrors = 0 then
			report "Das neq4-Modul(netlist) funktioniert einwandfrei!" severity note;
        end if;
        
        if numLogicErrors > 0 and numNetlistErrors > 0 then
			report "Das neq4-Modul(logic) funktioniert nicht einwandfrei! (" & integer'image(numLogicErrors) & " von " & integer'image(NUM_TESTCASES) & " Tests fehlerhaft)" & lf &
			"Das neq4-Modul(netlist) funktioniert nicht einwandfrei! (" & integer'image(numNetlistErrors) & " von " & integer'image(NUM_TESTCASES) & " Tests fehlerhaft)" severity failure;
		end if;

        if numLogicErrors > 0 then
			report "Das neq4-Modul(logic) funktioniert nicht einwandfrei! (" & integer'image(numLogicErrors) & " von " & integer'image(NUM_TESTCASES) & " Tests fehlerhaft)" severity failure;
		end if;

        if numNetlistErrors > 0 then
            report "Das neq4-Modul(netlist) funktioniert nicht einwandfrei! (" & integer'image(numNetlistErrors) & " von " & integer'image(NUM_TESTCASES) & " Tests fehlerhaft)" severity failure;
		end if;

		wait;

	end process;	

end behavioral;
