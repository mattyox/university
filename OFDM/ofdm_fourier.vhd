library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ofdm is
  Port ( clk        : in std_logic;
         reset      : in std_logic;
         data_in    : in std_logic_vector(3 downto 0);
         data_out   : out std_logic_vector(3 downto 0));
end ofdm;

architecture rtl of ofdm is

  -- Internal signals connecting modulator -> demodulator
--  signal i_signal : signed(15 downto 0);
--  signal q_signal : signed(15 downto 0);

  -- AXI4-Stream bus connections
  signal stage_one_valid_signal : std_logic;
  signal stage_one_ready_signal : std_logic;
  signal stage_one_last_signal  : std_logic;
  signal stage_one_iq_signal    : std_logic_vector(31 downto 0);
  
  signal stage_two_valid_signal : std_logic;
  signal stage_two_ready_signal : std_logic;
  signal stage_two_last_signal  : std_logic;
  signal stage_two_iq_signal    : std_logic_vector(31 downto 0);
  
  signal stage_three_valid_signal : std_logic;
  signal stage_three_ready_signal : std_logic;
  signal stage_three_last_signal  : std_logic;
  signal stage_three_iq_signal    : std_logic_vector(31 downto 0);
  
  signal stage_four_valid_signal : std_logic;
  signal stage_four_ready_signal : std_logic;
  signal stage_four_last_signal  : std_logic;
  signal stage_four_iq_signal    : std_logic_vector(31 downto 0);
  
  signal stage_five_valid_signal : std_logic;
  signal stage_five_ready_signal : std_logic;
  signal stage_five_last_signal  : std_logic;
  signal stage_five_iq_signal    : std_logic_vector(31 downto 0);

begin

  -- 16-QAM Modulator
--  modulator_instance : entity work.qam16_modulator
--    port map (
--      bits_in => data_in,
--      i_out   => i_signal,
--      q_out   => q_signal
--    );
    
    -- 16-QAM Modulator
  modulator_instance : entity work.qam16_modulator_axi
    port map (
      aclk          => clk,
      aresetn       => not reset,
      s_axis_tvalid => '1',
      -- s_axis_tready -- Unused
      s_axis_tdata  => data_in,
      s_axis_tlast  => '0',
      m_axis_tvalid => stage_one_valid_signal,
      m_axis_tready => stage_one_ready_signal,
      m_axis_tdata  => stage_one_iq_signal,
      m_axis_tlast  => stage_one_last_signal
    );
    
    -- Subcarrier mapper
  mapper_instance : entity work.subcarrier_mapper
    port map (
      aclk           => clk,
      aresetn        => not reset,
      s_axis_tvalid  => stage_one_valid_signal,
      s_axis_tready  => stage_one_ready_signal,
      s_axis_tdata   => stage_one_iq_signal,
      s_axis_tlast   => stage_one_last_signal,
      m_axis_tvalid  => stage_two_valid_signal,
      m_axis_tready  => stage_two_ready_signal,
      m_axis_tdata   => stage_two_iq_signal,
      m_axis_tlast   => stage_two_last_signal
    );
    
    -- IFFT
    ifft_instance : entity work.ifft
      port map (
        aclk           => clk,
        aresetn        => not reset,
        s_axis_data_tvalid  => stage_two_valid_signal,
        s_axis_data_tready  => stage_two_ready_signal,
        s_axis_data_tdata   => stage_two_iq_signal,
        s_axis_data_tlast   => stage_two_last_signal,
        
        s_axis_config_tdata => "01010000100011100000000110001110",
        s_axis_config_tvalid => '1',
        
        m_axis_data_tvalid  => stage_three_valid_signal,
        m_axis_data_tready  => stage_three_ready_signal,
        m_axis_data_tdata   => stage_three_iq_signal,
        m_axis_data_tlast   => stage_three_last_signal
      );
    
    -- FFT
    fft_instance : entity work.fft
      port map (
        aclk           => clk,
        aresetn        => not reset,
        s_axis_data_tvalid  => stage_three_valid_signal,
        s_axis_data_tready  => stage_three_ready_signal,
        s_axis_data_tdata   => stage_three_iq_signal,
        s_axis_data_tlast   => stage_two_last_signal,
        
        s_axis_config_tdata => "01010000100011110000000110001110",
        s_axis_config_tvalid => '1',
        
        m_axis_data_tvalid  => stage_four_valid_signal,
        m_axis_data_tready  => stage_four_ready_signal,
        m_axis_data_tdata   => stage_four_iq_signal,
        m_axis_data_tlast   => stage_four_last_signal
      );
    
    -- Subcarrier mapper
  demapper_instance : entity work.subcarrier_demapper
    port map (
      aclk           => clk,
      aresetn        => not reset,
      s_axis_tvalid  => stage_four_valid_signal,
      s_axis_tready  => stage_four_ready_signal,
      s_axis_tdata   => stage_four_iq_signal,
      s_axis_tlast   => stage_four_last_signal,
      m_axis_tvalid  => stage_five_valid_signal,
      m_axis_tready  => stage_five_ready_signal,
      m_axis_tdata   => stage_five_iq_signal,
      m_axis_tlast   => stage_five_last_signal
    );
  
  -- 16-QAM Demodulator
--  demodulator_instance : entity work.qam16_demodulator
--    port map (
--      i_in     => i_signal,
--      q_in     => q_signal,
--      bits_out => data_out
--    );

    -- 16-QAM Demodulator
  demodulator_instance : entity work.qam16_demodulator_axi
    port map (
      aclk    => clk,
      aresetn => not reset,
      s_axis_tvalid => stage_five_valid_signal,
      s_axis_tready => stage_five_ready_signal,
      s_axis_tdata  => stage_five_iq_signal,
      s_axis_tlast  => stage_five_last_signal,
      -- m_axis_tvalid -- Unused
      m_axis_tready => '1',
      m_axis_tdata  => data_out
      -- m_axis_tlast -- Unused
    );

end rtl;
