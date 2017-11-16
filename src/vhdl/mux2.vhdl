library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity mux2 is
port (
	a, b   : in  STD_LOGIC_VECTOR(31 downto 0);
	ctrl   : in  STD_LOGIC;
	s      : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss: in STD_LOGIC
);
end mux2;

architecture behavioral of mux2 is
begin
	s <= b when ctrl else a;
end;
