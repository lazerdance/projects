library IEEE;
use IEEE.std_logic_1164.all;

-- Hier Code einfügen
entity or2 is
    port(   A,B : in std_logic;
            Z : out std_logic);
end entity or2;

architecture simple of or2 is
begin
    Z <= A or B;
end architecture simple;
