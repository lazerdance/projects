library ieee;
use ieee.std_logic_1164.all;

library RorgPrSimLib;
use RorgPrSimLib.all;

library work;
use work.all;

entity alu_1bit is
	generic(carrySelect : boolean := false);
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
signal sumsignal : std_logic;
signal asignal : std_logic;
signal bsignal : std_logic;
begin

	unit : entity work.adder_1bit(behavioral)
		port map(
			a => asignal,
			b => bsignal,
			cin => carryIn,
			cout => carryOut,
			sum => sumsignal
			);

	alu : process(less, aInvert, bInvert, carryIn, operation, sumsignal, asignal, bsignal)
	begin
		if (operation = "00") then
			result <= asignal and bsignal;
		elsif (operation = "01") then
			result <= asignal or bsignal;
		elsif (operation = "10") then
			result <= sumsignal;
		elsif (operation = "11") then
			result <= less;
		end if;

		if ((a = '0' and aInvert = '0') or (a = '1' and aInvert = '1')) then
			asignal <= '0';
		elsif ((a = '1' and aInvert = '0') or (a = '0' and aInvert = '1')) then
			asignal <= '1';
		elsif (a = 'U' and (aInvert = '0' or aInvert = '1')) then
			asignal <= 'U';
		else
			asignal <= 'X';
		end if;

		if ((b = '0' and bInvert = '0') or (b = '1' and bInvert = '1')) then
			bsignal <= '0';
		elsif ((b = '1' and bInvert = '0') or (b = '0' and bInvert = '1')) then
			bsignal <= '1';
		elsif (b = 'U' and (bInvert = '0' or bInvert = '1')) then
			bsignal <= 'U';
		else
			bsignal <= 'X';
		end if; 
	end process;

	set <= sumsignal;

end structural;
