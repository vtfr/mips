library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity reg is
port (
	Clk         : in STD_LOGIC;

	Reset       : in STD_LOGIC;
	Data        : in STD_LOGIC_VECTOR(31 downto 0);
	S           : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss : in STD_LOGIC
);
end reg;

architecture behavioral of reg is
begin
	process (Clk) begin
		if rising_edge(Clk) then
			if Reset = '1' then
				S <= X"00000000";
			else
				S <= Data;
			end if;
		end if;
	end process;
end;
