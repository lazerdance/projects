library ieee;
use ieee.std_logic_1164.all;

entity mipsCtrl is
	port(op : in std_logic_vector(5 downto 0);
		 regDst : out std_logic;
		 branch : out std_logic;
		 memRead : out std_logic;
		 memToReg : out std_logic;
		 aluOp : out std_logic_vector(1 downto 0);
		 memWrite : out std_logic;
		 aluSrc : out std_logic;
		 regWrite : out std_logic);
end mipsCtrl;

architecture structural of mipsCtrl is	
signal zwischenergebnisse : std_logic_vector(3 downto 0);
begin

	-- Beschreibung der mipsCtrl hier einfügen
    zwischenergebnisse(0)<=(not op(0)) and (not op(1)) and (not op(2)) and (not op(3)) and (not op(4)) and (not op(5));
    zwischenergebnisse(1)<=op(0) and op(1) and (not op(2)) and (not op(3)) and (not op(4)) and op(5);
    zwischenergebnisse(2)<=op(0) and op(1) and (not op(2)) and op(3) and (not op(4)) and op(5);
    zwischenergebnisse(3)<=(not op(0)) and (not op(1)) and op(2) and (not op(3)) and (not op(4)) and (not op(5));
    regDst<=zwischenergebnisse(0);
    aluSrc<=zwischenergebnisse(1) or zwischenergebnisse(2);
    memToReg<=zwischenergebnisse(1);
    regWrite<=zwischenergebnisse(0) or zwischenergebnisse(1);
    memRead<=zwischenergebnisse(1);
    memWrite<=zwischenergebnisse(2);
    branch<=zwischenergebnisse(3);
    aluOp(1)<=zwischenergebnisse(0);
    aluOp(0)<=zwischenergebnisse(3);
	
end structural;
