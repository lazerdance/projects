library ieee;
use ieee.std_logic_1164.all;

entity adder_1bit is
	port(a : in std_logic;
		 b : in std_logic;
		 cin : in std_logic;
		 sum : out std_logic;
		 cout : out std_logic);
end adder_1bit;

architecture behavioral of adder_1bit is
begin

	sum <= a xor b xor cin;
	cout <= (a and b) or (a and cin) or (b and cin);
	
end behavioral;

architecture carrySelect of adder_1bit is
begin

	-- Beschreibung des Carry-Select-Addierers hier einfügen
	
end carrySelect;
