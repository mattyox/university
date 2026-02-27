library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity subcarrier_demapper is
  generic (
    NFFT      : integer := 1024;
    NEG_START : integer := 178;
    NEG_END   : integer := 511;
    POS_START : integer := 513;
    POS_END   : integer := 845
  );
  port (
    aclk    : in  std_logic;
    aresetn : in  std_logic;

    -- AXI4-Stream input
    s_axis_tvalid : in  std_logic;
    s_axis_tready : out std_logic;
    s_axis_tdata  : in  std_logic_vector(31 downto 0);
    s_axis_tlast  : in  std_logic;

    -- AXI4-Stream output
    m_axis_tvalid : out std_logic;
    m_axis_tready : in  std_logic;
    m_axis_tdata  : out std_logic_vector(31 downto 0);
    m_axis_tlast  : out std_logic
  );
end entity subcarrier_demapper;

architecture rtl of subcarrier_demapper is

  -- Internal handshake
  signal ready_int  : std_logic := '0';
  signal valid_int  : std_logic := '0';

  -- Storage
  signal in_reg    : std_logic_vector(31 downto 0);
  signal out_reg   : std_logic_vector(31 downto 0);
  signal last_reg  : std_logic;

  -- Counter
  signal subcarrier_index : integer range 0 to NFFT-1 := 0;
  
  -- External handshake
  signal valid_out  : std_logic := '0';

begin

  -- AXI4-Stream ready
  ready_int <= not valid_int;
  s_axis_tready <= ready_int;

  -- Input
  process(aclk)
  begin
    if rising_edge(aclk) then

      -- Reset
      if aresetn = '0' then
        valid_int <= '0';
        subcarrier_index <= 0;

      else

        -- Capture input
        if (s_axis_tvalid = '1' and ready_int = '1') then
          in_reg  <= s_axis_tdata;

          subcarrier_index <= subcarrier_index + 1;
          
          last_reg  <= s_axis_tlast;
          valid_int <= '1';
        end if;

        -- Output accepted by downstream
        if (valid_int = '1' and m_axis_tready = '1') then
          valid_int <= '0';
          
          -- Demap carriers
          if subcarrier_index < NEG_START then
            valid_out <= '0';
          elsif subcarrier_index < NEG_END then
            out_reg <= in_reg;
            valid_out <= '1';
          elsif subcarrier_index < POS_START then
            valid_out <= '0';
          elsif subcarrier_index < POS_END then
            out_reg <= in_reg;
            valid_out <= '1';
          else
            valid_out <= '0';
          end if;
          
          -- End of symbol
          if subcarrier_index = NFFT-1 then
            last_reg <= '1';
            subcarrier_index <= 0;
          else
            last_reg <= '0';
          end if;
          
        end if;

      end if;
    end if;
  end process;

  -- Output
  m_axis_tvalid <= valid_int;
  m_axis_tdata  <= out_reg;
  m_axis_tlast  <= last_reg;

end architecture;