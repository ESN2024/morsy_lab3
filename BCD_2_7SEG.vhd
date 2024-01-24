library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity BCD_2_7SEG is
		port(
            inp : in std_logic_vector(3 downto 0); -- input 
            outp : out std_logic_vector(7 downto 0) -- output 
        );
end entity;


architecture arch of BCD_2_7SEG is

begin
    outp <= "11000000" when inp = "0000" else 
            "11111001" when inp = "0001" else 
            "10100100" when inp = "0010" else 
            "10110000" when inp = "0011" else 
            "10011001" when inp = "0100" else 
            "10010010" when inp = "0101" else 
            "10000010" when inp = "0110" else 
            "11111000" when inp = "0111" else 
            "10000000" when inp = "1000" else 
            "10010000" when inp = "1001" else
            "11111111";

end architecture;