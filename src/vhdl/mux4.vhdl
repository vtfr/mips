library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity mux4 is
port (
	a, b, c, d : in  STD_LOGIC_VECTOR(31 downto 0);
	ctrl       : in  STD_LOGIC_VECTOR(1 downto 0);
	s          : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss: in STD_LOGIC
);
end mux4;

architecture behavioral of mux4 is
begin
	with ctrl select s <=
		a when "00",
		b when "01",
		c when "10",
		d when "11";
end;
