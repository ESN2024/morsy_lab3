library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity virgule is
		port(
            inp1 : in std_logic_vector(3 downto 0); -- input 
            outp1 : out std_logic_vector(7 downto 0) -- output 
        );
end entity;


architecture arch of virgule is

begin
    outp1 <= "01000000" when inp1 = "0000" else 
            "01111001" when inp1 = "0001" else 
            "00100100" when inp1 = "0010" else 
            "00110000" when inp1 = "0011" else 
            "00011001" when inp1 = "0100" else 
            "00010010" when inp1 = "0101" else 
            "00000010" when inp1 = "0110" else 
            "01111000" when inp1 = "0111" else 
            "00000000" when inp1 = "1000" else 
            "00010000" when inp1 = "1001" else
            "01111111";

end architecture;