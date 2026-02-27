library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity qam16_demodulator_axi is
  port (
    aclk    : in  std_logic;
    aresetn : in  std_logic;

    -- AXI4-Stream input
    s_axis_tvalid : in  std_logic;
    s_axis_tready : out std_logic;
    s_axis_tdata  : in  std_logic_vector(31 downto 0); -- [15:0] = I, [31:16] = Q
    s_axis_tlast  : in  std_logic;

    -- AXI4-Stream output
    m_axis_tvalid : out std_logic;
    m_axis_tready : in  std_logic;
    m_axis_tdata  : out std_logic_vector(3 downto 0);
    m_axis_tlast  : out std_logic
  );
end entity;

architecture rtl of qam16_demodulator_axi is

  -- Internal handshake
  signal last_reg   : std_logic := '0';
  signal valid_reg  : std_logic := '0';

  -- Storage
  signal i, q : signed(15 downto 0) := (others=>'0');

  -- Bits output
  signal bits_out : std_logic_vector(3 downto 0);

begin

  -- AXI4-Stream ready
  s_axis_tready <= not valid_reg;

  -- Input
  process(aclk)
  begin
    if rising_edge(aclk) then

      -- Reset
      if aresetn = '0' then
        i      <= (others=>'0');
        q      <= (others=>'0');
        last_reg   <= '0';
        valid_reg  <= '0';
      
      else

        -- Capture input
        if s_axis_tvalid = '1' and valid_reg = '0' then
          i     <= signed(s_axis_tdata(15 downto 0));
          q     <= signed(s_axis_tdata(31 downto 16));
          last_reg  <= s_axis_tlast;
          valid_reg <= '1';
        end if;

        -- Output accepted by downstream
        if valid_reg = '1' and m_axis_tready = '1' then
          valid_reg <= '0';
        end if;
      
      end if;
    end if;
  end process;

  -- 16-QAM Demodulator
  qam16_demodulator : entity work.qam16_demodulator
    port map (
      i_in     => i,
      q_in     => q,
      bits_out => bits_out
    );

  -- Output
  m_axis_tdata  <= bits_out;
  m_axis_tvalid <= valid_reg;
  m_axis_tlast  <= last_reg;

end architecture;
