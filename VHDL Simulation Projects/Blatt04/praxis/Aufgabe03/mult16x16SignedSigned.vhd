library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mult16x16SignedSigned is
    port(a : in signed(15 downto 0);
         b : in signed(15 downto 0);
         y : out signed(31 downto 0));
end mult16x16SignedSigned;

architecture behavioral of mult16x16SignedSigned is
begin
    y <= a * b;
end behavioral;
