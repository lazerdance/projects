library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fulladd is
	port ( a, b, cin : in  std_logic;
	       sum, cout : out std_logic);
end entity fulladd;

architecture behavioral of fulladd is
	signal sum1, cout1, cout2 : std_logic;
begin
	-- block Elemente dienen der Hervorhebung von Gruppierungen,
	-- sie sind quasi eine architecture in einer architecture
	halfadd1 : block
	begin
		sum1  <= a xor b;
		cout1 <= a and b;
	end block;

	halfadd2 : block
	begin
		sum  <= sum1 xor cin;
		cout2 <= sum1 and cin;
	end block;

	cout <= cout1 or cout2;
end architecture;