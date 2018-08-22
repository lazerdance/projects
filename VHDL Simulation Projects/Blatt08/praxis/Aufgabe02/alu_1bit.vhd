library ieee;
use ieee.std_logic_1164.all;

library RorgPrSimLib;
use RorgPrSimLib.all;

library work;
use work.all;

entity alu_1bit is
	generic(carrySelect : boolean := true);
	port(operation : in std_logic_vector(1 downto 0);
		 a : in std_logic;
		 aInvert : in std_logic;
		 b : in std_logic;
		 bInvert : in std_logic;
		 carryIn : in std_logic;
		 less : in std_logic;
		 result : out std_logic;
		 carryOut : out std_logic;
		 set : out std_logic);
end alu_1bit;

architecture structural of alu_1bit is
signal sumsignal, asignal, bsignal : std_logic;
begin

	csadder : if carrySelect = true generate
		unit1 : entity work.adder_1bit(carrySelect)
			port map(
				a => asignal,
				b => bsignal,
				cin => carryIn,
				cout => carryOut,
				sum => sumsignal
				);
	end generate;

	adder1bit : if carrySelect = false generate
		unit2 : entity work.adder_1bit(behavioral)
			port map(
				a => asignal,
				b => bsignal,
				cin => carryIn,
				cout => carryOut,
				sum => sumsignal
				);
	end generate;

	alu : process(less, operation, sumsignal, asignal, bsignal, aInvert, bInvert, a, b)
	begin
            case operation is
                when "00" => result <= asignal and bsignal;
                when "01" => result <= asignal or bsignal;
                when "10" => result <= sumsignal;
                when "11" => result <= less;
                when others => result <= 'X';
            end case;
            if (aInvert = '0' or aInvert = '1') then
                if (a = '0' or a = '1')then
                    asignal <= a XOR aInvert;
                elsif (a = 'U')then
                    asignal <= 'U';
                else
                    asignal <= 'X';
                end if;
            else
                asignal <= 'X';
            end if;
            if (bInvert = '0' or bInvert = '1') then
                if (b = '0' or b = '1')then
                    bsignal <= b XOR bInvert;
                elsif (b = 'U')then
                    bsignal <= 'U';
                else
                    bsignal <= 'X';
                end if;
            else
                bsignal <= 'X';
            end if;
	end process;

	set <= sumsignal;

end structural;
