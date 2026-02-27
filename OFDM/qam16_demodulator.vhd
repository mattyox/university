library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity qam16_demodulator is
  port (
    i_in     : in  signed(15 downto 0);
    q_in     : in  signed(15 downto 0);
    bits_out : out std_logic_vector(3 downto 0)
  );
end entity;

architecture rtl of qam16_demodulator is

  -- Midpoints of magnitues
  -- (5182 + 15544) / 2 = 10363
  constant M2 : signed(15 downto 0) := to_signed(10363, 16);

  signal i_bits, q_bits : std_logic_vector(1 downto 0);

begin

  process(i_in, q_in)
  begin

      -- I
      if    i_in < -M2 then
        i_bits <= "00";       -- -M3
      elsif i_in < 0   then
        i_bits <= "01";       -- -M1
      elsif i_in < M2  then
        i_bits <= "11";       -- +M1
      else
        i_bits <= "10";       -- +M3
      end if;

      -- Q
      if    q_in < -M2 then
        q_bits <= "00";       -- -M3
      elsif q_in < 0   then
        q_bits <= "01";       -- -M1
      elsif q_in < M2  then
        q_bits <= "11";       -- +M1
      else
        q_bits <= "10";       -- +M3
      end if;

  end process;
  
  -- Combine into 4-bit output
  bits_out <= i_bits & q_bits;

end architecture;
