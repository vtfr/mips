library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity reg is
port (
	clk    : in  STD_LOGIC;
	reset  : in  STD_LOGIC;
	enable : in  STD_LOGIC;
	d      : in  STD_LOGIC_VECTOR(31 downto 0);
	q      : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss: in STD_LOGIC
);
end reg;

architecture behavioral of reg is
begin
	-- Apenas modifica o valor do registrador caso seja clock de subida
	process(clk) begin
		if rising_edge(clk) then
			-- Caso reset
			if reset = '1' then
				q <= 0;
			-- Caso enable
			elsif enable = '1' then
				q <= d;
			end if;
		end if;
	end process;
end;
