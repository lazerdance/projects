library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mult32x32 is
    port(
		a 	: in std_logic_vector(31 downto 0);
		b 	: in std_logic_vector(31 downto 0);
		y 	: out std_logic_vector(63 downto 0)
	);
end mult32x32;

architecture structural of mult32x32 is
    signal s1,s3,s4 : signed(31 downto 0);
    signal s2 : unsigned(31 downto 0);
    signal s5,s9 : signed(63 downto 0);
    signal s6,s7,s8 : signed(32 downto 0);
    signal bu,au : unsigned(15 downto 0);
    signal bs,as : signed(15 downto 0);
begin
    
    -- Strukturbeschreibung hier einfügen
    editsignals : process(a,b,s8,s3,s4,s2,s1)
    begin
        for i in 0 to 63 loop
            if (i<16) then
                s9(i)<='0';
                as(i)<=a(16+i);
                au(i)<=a(i);
                bs(i)<=b(16+i);
                bu(i)<=b(i);
            elsif (i<48) then
                s9(i)<=s8(i-16);
            else 
                s9(i)<=s8(32);
            end if;
            if (i<32) then
                s6(i)<=s3(i);
                s7(i)<=s4(i);
                s5(i)<=s2(i);
            else
                s5(i)<=s1(i-32);
            end if;
            
        end loop;
        -- s5 <= s1 & s2; 
        s6(32)<=s3(31);
        s7(32)<=s4(31);
    end process;
    
    
    u1:entity work.mult16x16SignedSigned(behavioral)
        port map(a => as,
                b => bs,
                y => s1);
    u2:entity work.mult16x16UnsignedUnsigned(behavioral)
        port map(a => au,
                b => bu,
                y => s2);
    u3:entity work.mult16x16UnsignedSigned(behavioral)
        port map(a => bu,
                b => as,
                y => s3);
    u4:entity work.mult16x16UnsignedSigned(behavioral)
        port map(a => au,
                b => bs,
                y => s4);
    s8<=s6+s7;
    
    y<=std_logic_vector(s5+s9);
        

end structural;
