library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity signe_moins is
		port(
            inp2 : in std_logic_vector(3 downto 0); -- input 
            outp2 : out std_logic_vector(7 downto 0) -- output 
        );
end entity;


architecture arch of signe_moins is

begin
    outp2 <= "11111111" when inp2 = "0000" else 
            "10111111" when inp2 = "0001" else 
            "11000000";

end architecture;