library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mult16x16UnsignedUnsigned is
    port(a : in unsigned(15 downto 0);
         b : in unsigned(15 downto 0);
         y : out unsigned(31 downto 0));
end mult16x16UnsignedUnsigned;

architecture behavioral of mult16x16UnsignedUnsigned is
begin
    y <= a * b;
end behavioral;
