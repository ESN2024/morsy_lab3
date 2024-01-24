library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity morsy_lab3 is port (
        clk : in std_logic := '0' ;
        reset_n : in std_logic :='0';
		  btn : in std_logic :='X';
        SEG1  : out  std_logic_vector(7 downto 0);
		  SEG2  : out  std_logic_vector(7 downto 0);
		  SEG3  : out  std_logic_vector(7 downto 0);
		  SEG4  : out  std_logic_vector(7 downto 0);
		  SEG5  : out  std_logic_vector(7 downto 0);
		  SEG6  : out  std_logic_vector(7 downto 0);
		  SDA	  : inout std_logic;
		  SCL   : inout std_logic ;
		  switch: in std_logic_vector(3 downto 0);
		  led   : out std_logic);
end entity;

architecture arch of morsy_lab3 is    
     component lab3 is
        port (
            clk_clk                             : in    std_logic                    := 'X';             -- clk
            opencores_i2c_0_export_0_scl_pad_io : inout std_logic                    := 'X';             -- scl_pad_io
            opencores_i2c_0_export_0_sda_pad_io : inout std_logic                    := 'X';             -- sda_pad_io
            push_btn_external_connection_export : in    std_logic                    := 'X';             -- export
            reset_reset_n                       : in    std_logic                    := 'X';             -- reset_n
            seg1_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            seg2_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            seg3_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            seg4_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            seg5_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            seg6_external_connection_export     : out   std_logic_vector(3 downto 0);                    -- export
            switch_external_connection_export   : in    std_logic_vector(3 downto 0) := (others => 'X'); -- export
            led_external_connection_export      : out   std_logic                                        -- export
        );
    end component lab3;
	 
	 component BCD_2_7SEG is
			port(
            inp : in std_logic_vector(3 downto 0); -- input 
            outp : out std_logic_vector(7 downto 0) -- output 
        );
	end component BCD_2_7SEG;
	
	component virgule is
			port(
            inp1 : in std_logic_vector(3 downto 0); -- input 
            outp1 : out std_logic_vector(7 downto 0) -- output 
        );
	end component virgule;
	
	component signe_moins is
			port(
            inp2 : in std_logic_vector(3 downto 0); -- input 
            outp2 : out std_logic_vector(7 downto 0) -- output 
        );
	end component signe_moins;
	
signal seg_1 : std_logic_vector(3 downto 0);
signal seg_2 : std_logic_vector(3 downto 0);
signal seg_3 : std_logic_vector(3 downto 0);
signal seg_4 : std_logic_vector(3 downto 0);
signal seg_5 : std_logic_vector(3 downto 0);
signal seg_6 : std_logic_vector(3 downto 0);
begin
    u0 : component lab3
        port map (
            clk_clk                             => clk,                             --                          clk.clk
            reset_reset_n                       => reset_n,                       --                        reset.reset_n
            seg3_external_connection_export     => seg_3,     --     seg3_external_connection.export
            seg2_external_connection_export     => seg_2,     --     seg2_external_connection.export
            seg1_external_connection_export     => seg_1,     --     seg1_external_connection.export
            push_btn_external_connection_export => btn, -- push_btn_external_connection.export
            seg4_external_connection_export     => seg_4,     --     seg4_external_connection.export
            seg5_external_connection_export     => seg_5,     --     seg5_external_connection.export
            seg6_external_connection_export     => seg_6,     --     seg6_external_connection.export
				opencores_i2c_0_export_0_scl_pad_io => SCL, --     opencores_i2c_0_export_0.scl_pad_io
            opencores_i2c_0_export_0_sda_pad_io => SDA,  --                             .sda_pad_io
				switch_external_connection_export   => switch,   --   switch_external_connection.export
            led_external_connection_export      => led       --      led_external_connection.export
        );
		  u1 : component BCD_2_7SEG
            port map (
                inp => seg_1,
                outp => SEG1
            );

        u2 : component BCD_2_7SEG
            port map (
                inp => seg_2,
                outp => SEG2
            );

        u3 : component BCD_2_7SEG
            port map (
                inp => seg_3,
                outp => SEG3
            );
			u4	: component virgule
            port map (
                inp1 => seg_4,
                outp1 => SEG4
            );
			u5 : component BCD_2_7SEG
            port map (
                inp => seg_5,
                outp => SEG5
            );
			u6: component signe_moins
            port map (
                inp2 => seg_6,
                outp2 => SEG6
            );


end architecture;