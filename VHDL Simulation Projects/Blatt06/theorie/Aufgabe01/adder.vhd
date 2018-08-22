library ieee;
use ieee.std_logic_1164.all;

entity adder is
	generic( WIDTH : integer := 32 );
	port ( a,b  : in  std_logic_vector( WIDTH-1 downto 0 );
	       sum  : out std_logic_vector( WIDTH-1 downto 0 );
	       cout : out std_logic);
end entity;

architecture behavioral of adder is
signal c : std_logic_vector(WIDTH-1 downto 0);

component fulladd
	port (	a,b,cin : in std_logic;
		sum, cout : out std_logic);
end component;

for all: fulladd use entity work.fulladd(behavioral);

begin
	-- Addierer-Beschreibung hier einfuegen
		fulladder : for i in 1 to WIDTH - 1 generate
		unit : fulladd
                	port map(a=>a(i),
                            	b=>b(i),
                            	cin=>c(i-1),
                            	cout=>c(i),
                            	sum=>sum(i));        
            	end generate;

	u : entity work.fulladd(behavioral)
		port map (a => a(0),
			  b => b(0),
			  cin => '0',
			  cout => c(0),
			  sum => sum(0));

end architecture;
