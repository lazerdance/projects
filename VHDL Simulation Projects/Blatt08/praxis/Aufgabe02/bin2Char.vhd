library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bin2Char is
	port(bin : in std_logic_vector(3 downto 0);
		 bitmask : out std_logic_vector(6 downto 0));
end bin2Char;

architecture behavioral of bin2Char is   
begin
	
	-- bin2Char-Beschreibung hier einfügen
	decode:process(bin)
	begin
        case bin is
            when "0000" => bitmask <= "0111111";-- 0 ok
            when "0001" => bitmask <= "0000110";-- 1 ok
            when "0010" => bitmask <= "1011011";-- 2 ok
            when "0011" => bitmask <= "1001111";-- 3 ok
            when "0100" => bitmask <= "1100110";-- 4 ok
            when "0101" => bitmask <= "1101101";-- 5 ok
            when "0110" => bitmask <= "1111101";-- 6 ok
            when "0111" => bitmask <= "0000111";-- 7 ok
            when "1000" => bitmask <= "1111111";-- 8 ok
            when "1001" => bitmask <= "1101111";-- 9 ok
            when "1010" => bitmask <= "1110111";-- A 
            when "1011" => bitmask <= "1111100";-- b 
            when "1100" => bitmask <= "0111001";-- C 
            when "1101" => bitmask <= "1011110";-- d 
            when "1110" => bitmask <= "1111001";-- E 
            when "1111" => bitmask <= "1110001";-- F 
            when others => bitmask <= "0000000";-- ERROR
        end case;
	end process;
	 
end behavioral;
