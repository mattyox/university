library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity qam16_modulator is
  port (
    bits_in : in  std_logic_vector(3 downto 0);
    i_out   : out signed(15 downto 0);
    q_out   : out signed(15 downto 0)
  );
end entity;

architecture rtl of qam16_modulator is

  -- Normalized amplitude of 1
  -- 1 / sqrt(10) * 2^14
  constant M1 : signed(15 downto 0) := to_signed( 5182, 16);
  -- Normalized amplitude of 3
  -- 3 / sqrt(10) * 2^14
  constant M3 : signed(15 downto 0) := to_signed(15544, 16);

begin

  process(bits_in)
  begin
    
      -- I
      case bits_in(3 downto 2) is
        when "00" => i_out <= -M3;
        when "01" => i_out <= -M1;
        when "11" => i_out <=  M1;
        when "10" => i_out <=  M3;
        when others => i_out <= (others => '0');
      end case;

      -- Q
      case bits_in(1 downto 0) is
        when "00" => q_out <= -M3;
        when "01" => q_out <= -M1;
        when "11" => q_out <=  M1;
        when "10" => q_out <=  M3;
        when others => q_out <= (others => '0');
      end case;
      
  end process;

end architecture;
