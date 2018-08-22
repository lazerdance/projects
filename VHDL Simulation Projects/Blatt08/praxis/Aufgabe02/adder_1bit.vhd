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
signal sumsignal1, sumsignal2, coutsignal1, coutsignal2 : std_logic;
begin

	u1 : entity adder_1bit(behavioral)
		port map(
			a => a,
			b => b,
			cin => '1',
			cout => coutsignal1,
			sum => sumsignal1
			);

	u2 : entity adder_1bit(behavioral)
		port map(
			a => a,
			b => b,
			cin => '0',
			cout => coutsignal2,
			sum => sumsignal2
			);
	
	error : process(sumsignal1, sumsignal2, coutsignal1, coutsignal2, cin)
	begin
            case cin is
                when '1' =>
                    sum <= sumsignal1;
                    cout <= coutsignal1;
                when '0' =>
                    sum <= sumsignal2;
                    cout <= coutsignal2;
                when others =>
                    sum <= 'X';
                    cout <= 'X';
            end case;
	end process;
	
end carrySelect;
