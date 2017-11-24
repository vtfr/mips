library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity signextend is
port (
	A : in STD_LOGIC_VECTOR(15 downto 0);
	S : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss : in STD_LOGIC
);
end signextend;

architecture behavioral of signextend is
begin
	S <= X"FFFF" & A when A(15) else X"0000" & A;
end;
