library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity qam16_modulator_axi is
  port (
    aclk    : in  std_logic;
    aresetn : in  std_logic;

    -- AXI4-Stream input
    s_axis_tvalid : in  std_logic;
    s_axis_tready : out std_logic;
    s_axis_tdata  : in  std_logic_vector(3 downto 0);
    s_axis_tlast  : in  std_logic;

    -- AXI4-Stream output
    m_axis_tvalid : out std_logic;
    m_axis_tready : in  std_logic;
    m_axis_tdata  : out std_logic_vector(31 downto 0);
    m_axis_tlast  : out std_logic
  );
end entity;

architecture rtl of qam16_modulator_axi is

  -- Internal handshake
  signal ready_int  : std_logic := '0';
  signal valid_int  : std_logic := '0';

  -- Storage
  signal bits_reg   : std_logic_vector(3 downto 0);
  signal last_reg   : std_logic;

  -- IQ output
  signal i, q   : signed(15 downto 0);

begin

  -- AXI4-Stream ready
  s_axis_tready <= not valid_int;

  -- Input
  process(aclk)
  begin
    if rising_edge(aclk) then

      -- Reset
      if aresetn = '0' then
        valid_int <= '0';

      else

        -- Capture input
        if (s_axis_tvalid = '1' and valid_int = '0') then
          bits_reg  <= s_axis_tdata;
          last_reg  <= s_axis_tlast;
          valid_int <= '1';
        end if;

        -- Output accepted by downstream
        if (valid_int = '1' and m_axis_tready = '1') then
          valid_int <= '0';
        end if;

      end if;
    end if;
  end process;

  -- 16-QAM Modulator
  qam16_modulator : entity work.qam16_modulator
    port map (
      bits_in => bits_reg,
      i_out   => i,
      q_out   => q
    );

  -- Output
  m_axis_tvalid <= valid_int;
  m_axis_tdata  <= std_logic_vector(q & i);
  m_axis_tlast  <= last_reg;

end architecture;
