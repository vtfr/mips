library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity regfile is
port (
	Clk         : in STD_LOGIC;
	
	WriteEnable : in STD_LOGIC;
	A1, A2, A3  : in unsigned(4 downto 0);
	Data        : in STD_LOGIC_VECTOR(31 downto 0);
	RD1, RD2    : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss : in STD_LOGI
);
end regfile;

architecture behavioral of regfile is
	signal Memory: array (31 downto 0) of STD_LOGIC_VECTOR (31 downto 0);
begin
	process(A1, A2, Memory) begin
		RD1 <= x"00000000" when A1 = "00000" else mem(A1);
		RD2 <= x"00000000" when A2 = "00000" else mem(A2);
	end process;

	process (Clk) begin
		if rising_edge(Clk) and WriteEnable = '1' then
			mem(A3) <= Data;
		end if;
	end process;
end;
