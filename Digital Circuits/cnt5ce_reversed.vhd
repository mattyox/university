library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity cnt5ce is
	port(
		clk : in std_logic;
		ce : in std_logic;
		clrn : in std_logic;
		Q : out std_logic_vector(4 downto 0)
		);
end entity cnt5ce;

architecture dfl of cnt5ce is 
	signal state : std_logic_vector(4 downto 0) := "00000";
	signal rev_state : std_logic_vector(4 downto 0) := "00000";
begin
	process(clk, clrn)
	begin
		if rising_edge(clk) then
			if ce = '1' then
				state <= state + 1;
			end if;
		end if;
		if clrn = '0' then
			state <= (others => '0');
		end if;
	end process;
	
	rev_state <= state(0) & state(1) & state(2) & state(3) & state(4);
	Q <= rev_state;
	
	


end architecture dfl;