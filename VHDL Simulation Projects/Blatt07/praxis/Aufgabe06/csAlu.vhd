library ieee;
use ieee.std_logic_1164.all;

library RorgPrSimLib;
use RorgPrSimLib.all;

library work;
use work.all;

entity csAlu is
	generic(WIDTH : integer);
	port(ctrl : in std_logic_vector(3 downto 0);
		 a : in std_logic_vector(WIDTH - 1 downto 0);
		 b : in std_logic_vector(WIDTH - 1 downto 0);
		 result : out std_logic_vector(WIDTH - 1 downto 0);
		 overflow : out std_logic;
		 zero : out std_logic);
end csAlu;

architecture behavioral of csAlu is
signal carry, resultsignal : std_logic_vector (WIDTH - 1 downto 0);
signal overflowsignal : std_logic_vector (1 downto 0);
signal setless : std_logic;
begin

	chain : for i in 1 to WIDTH - 2 generate
		unit : entity work.alu_1bit(structural)
			port map(
				operation => ctrl(1 downto 0),
				a => a(i),
				b => b(i),
				aInvert => ctrl(3),
				bInvert => ctrl(2),
				carryIn => carry(i-1),
				carryOut => carry(i),
				less => '0',
				set => open,
				result => resultsignal(i)
				);
		end generate;

	firstunit : entity work.alu_1bit(structural)
		port map(
			operation => ctrl(1 downto 0),
			a => a(WIDTH-1),
			b => b(WIDTH-1),
			aInvert => ctrl(3),
			bInvert => ctrl(2),
			carryIn => carry(WIDTH-2),
			carryOut => carry(WIDTH-1),
			less => '0',
			set => setless,
			result => resultsignal(WIDTH-1)
			);

	lastunit : entity work.alu_1bit(structural)
		port map(
			operation => ctrl(1 downto 0),
			a => a(0),
			b => b(0),
			aInvert => ctrl(3),
			bInvert => ctrl(2),
			carryIn => ctrl(2),
			carryOut => carry(0),
			less => setless,
			set => open,
			result => resultsignal(0)
			);

	zeroflag : process(resultsignal, overflowsignal)
	variable counter : integer := 0;
	begin
		for i in 0 to WIDTH - 1 loop
			if(resultsignal(i) /= '0') then
				counter := counter + 1;
			end if;
		end loop;

		case counter is
            when 0 => zero <= '1';
            when others => zero <= '0';
        end case;

		counter := 0;
		
		case overflowsignal is
            when "10" => overflow <= '1';
            when "01" => overflow <= '1';
            when others => overflow <= '0';
        end case;
	end process;
	overflowsignal(1) <= carry(WIDTH-1);
	overflowsignal(0) <= carry(WIDTH-2);
	result <= resultsignal;
    
end behavioral;
