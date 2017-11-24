library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity regfile is
port (
	Clk         : in STD_LOGIC;
	
	WriteEnable : in STD_LOGIC;
	A1, A2, A3  : in unsigned(4 downto 0);
	Data        : in STD_LOGIC_VECTOR(31 downto 0);
	RD1, RD2    : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss : in STD_LOGIC
);
end regfile;

architecture behavioral of regfile is
	type Mem is array (31 downto 0) of STD_LOGIC_VECTOR(31 downto 0);
	signal Memory: Mem;
begin
	process (A1, A2, Memory) begin
		if A1 = "00000" then
			RD1 <= x"00000000";
		else
			RD1 <= Memory(A1);
		end if;

		if A2 = "00000" then		
			RD2 <= x"00000000";
		else
			RD2 <= Memory(A2);
		end if;
	end process;
	
	process (Clk) begin
		if rising_edge(Clk) and WriteEnable = '1' then
			Memory(A3) <= Data;
		end if;
	end process;
end;
